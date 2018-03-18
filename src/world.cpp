#include "world.hpp"
#include <iostream>
#include "behavior.hpp"

int wrap_i(int i,int max_i){
 return ((i%max_i)+max_i)%max_i;
}
World::World(const WorldConsts& wc):wc(wc),
				    time(0),
				    time_step(0){
  float ij_product = wc.grid_x_size*wc.grid_y_size;
  plant_grid.resize(wc.grid_x_size);
  del_plant_grid.resize(wc.grid_x_size);
  for(uint i =0;i<plant_grid.size();++i){
    plant_grid[i].resize(wc.grid_y_size);
    del_plant_grid[i].resize(wc.grid_y_size);
    for(uint j =0;j<plant_grid[i].size();++j){
      plant_grid[i][j]=wc.max_plant_density*i*j/ij_product;
    }
  }
}

void World::add_fish(int num){
  std::cout<<"adding fish"<<std::endl;
  for(int i =0;i<num;++i){
    std::cout<<"\t"<<i<<std::endl;
    Fish* fish = new Fish(i,wc);
    fish->set_behavior(new RandomWalk());
    //fish->move_to(i*70,i*80);
    fish->move_to(wc.world_x_len*uniform_random(),wc.world_y_len*uniform_random());
    FishState fs(fish);
    //std::cout<<world_consts.world_x_len<<"->"<<fs.pos.x<<" "<<world_consts.world_y_len<<"->"<<fs.pos.y<<std::endl;
    all_fish.emplace(fish_num++,fish);
  }
}
void World::add_pred(int num){
  std::cout<<"adding pred"<<std::endl;
  for(int i =0;i<num;++i){
    std::cout<<"\t"<<i<<std::endl;
    FishState fs;
    fs.struct_mass = 10.0;
    fs.muscle_mass = 20.0;
    fs.fat_mass = 10.0;
    Fish* fish = new Fish(i,fs,wc);
    fish->set_behavior(new Predator());
    //fish->move_to(i*70,i*80);
    fish->move_to(wc.world_x_len*uniform_random(),wc.world_y_len*uniform_random());
    //std::cout<<world_consts.world_x_len<<"->"<<fs.pos.x<<" "<<world_consts.world_y_len<<"->"<<fs.pos.y<<std::endl;
    all_fish.emplace(fish_num++,fish);
  }
}
void World::add_fish(Fish* fish){
  if(fish!=NULL){
    std::cout<<"adding fish2"<<fish->get_pos()<<std::endl;
    all_fish.emplace(fish_num++,fish);
  }
}
void World::remove_fish(Fish* fish){
  all_fish.erase(fish->get_id());
}
std::vector<FishState> World::get_fish_states(){
  std::vector<FishState> fish_states;
  fish_states.resize(all_fish.size());
  auto it = all_fish.begin();
  int i =0;
  while(it != all_fish.end()){
    fish_states[i].set(it->second);
    ++it;
    ++i;
  }
  return fish_states;
}

void World::step(float dt){
  time+=dt;
  time_step +=1;
  step_defuse_plants(dt);
  step_grow_plants(dt);
  calc_del_plant_grid();
  step_fish_sense(dt);
  step_move_fish(dt);
  step_interact_fish(dt);
  step_grow_fish(dt);
}
void World::step_defuse_plants(float dt){
  for(int i =0;i<(int)plant_grid.size();++i){
    for(int j =0;j<(int)plant_grid[i].size();++j){
      int i2 = i+1;
      int j2 = j;
      wrap_plant_ij(i2,j2);
      float dplant = (plant_grid[i][j]-plant_grid[i2][j2])*dt*wc.grid_dy*
	wc.plant_diffusion_coef;
      plant_grid[i][j] -=dplant;
      plant_grid[i2][j2] +=dplant;
      i2 = i;
      j2 = j+1;
      wrap_plant_ij(i2,j2);
      dplant = (plant_grid[i][j]-plant_grid[i2][j2])*dt*wc.grid_dx*
	wc.plant_diffusion_coef;
      plant_grid[i][j] -=dplant;
      plant_grid[i2][j2] +=dplant;
      
    }
 }

}
void World::step_grow_plants(float dt){
  for(int i =0;i<(int)plant_grid.size();++i){
    for(int j =0;j<(int)plant_grid[i].size();++j){
      float val = plant_grid[i][j];
      val = val-val*val/wc.max_plant_density;
      plant_grid[i][j]+=val*wc.plant_growth_exp*dt + wc.plant_growth_linear*dt;
      if(plant_grid[i][j]>wc.max_plant_density)
	plant_grid[i][j]=wc.max_plant_density;
    }
  }
}
void World::step_fish_sense(float dt){
  for(auto it = all_fish.begin();
      it != all_fish.end();
      ++it){
    Fish* fish = it->second;
    if(!fish->is_dead() && fish->get_behavior() != NULL){
      fish->get_behavior()->sense_world(fish,this,dt);
    }
  }
}
void World::step_move_fish(float dt){
  for(auto it = all_fish.begin(); it != all_fish.end();  ++it){
    Fish* fish = it->second;
    if(fish->get_behavior() == NULL){
      fish->move(Vect(10,0),dt);
      fish->periodic_bound(1000,1000);
    }
    else{
      Vect fc = fish->get_force()*fish->get_muscle_mass()*wc.muscle_efficiency;
      Vect vel = fish->get_vel();
      int iter_num = 10;
      float ddt = dt/iter_num;
      for(int i =0;i<iter_num;++i){
	// std::cout<<"---"<<std::endl;
	// std::cout<<vel.abs()<<" "<<vel.abs()*wc.linear_drag<<std::endl;
	// std::cout<<(vel*vel.abs()).abs()<<" "<<(vel*vel.abs()).abs()*wc.quad_drag<<std::endl;
	Vect drag = vel*wc.linear_drag + vel*vel.abs()*wc.quad_drag;
	vel += fc*ddt/fish->get_mass()-drag*ddt/fish->get_mass();
	//Vect d_vel = fc*dt;//-drag*dt;
      }
      fish->move(vel*dt,dt);
      fish->periodic_bound(1000,1000);
      //std::cout<<vel<<"+"<<d_vel<<"-"<<drag<<"="<<(vel+d_vel-drag)<<std::endl;
    }
  }
}
void World::step_interact_fish(float dt){
  auto it = all_fish.begin();
  while(it != all_fish.end()){
    int i,j;
    Fish* f = it->second;
    if(!f->is_dead()){
      //if alive eat
      if(f->is_herbivore()){ //plants & motabilize
	Vect pos = f->get_pos();
	plant_ij_from_pos(pos,i,j);
	f->eat_plant(plant_grid[i][j],dt);
	f->motabilise_energy(dt);
	f->recalculate_mass();
      }
      if(f->is_predator()){
	auto it2 = all_fish.begin();
	while(it2 != all_fish.end()){
	  Fish* f2 = it2->second;
	  if(f != f2){ //don't eat your self.
	    float reach = wc.mass_to_reach * f->get_mass();
	    //float max_mass = wc->struct_to_max_fish_mass * f->get_struct_mass();
	    if(vabs(dist(f->get_pos(),f2->get_pos())) < reach &&
	       f->can_eat_by_size(f2)){
	      f->eat_fish(f2);
	      remove_fish(f2);
	    }
	  }
	  ++it2;
	}
      }
    }
    
    ++it;
  }
}
void World::step_grow_fish(float dt){
  auto it = all_fish.begin();
  while(it != all_fish.end()){
    Fish* f = it->second;
    f->get_behavior()->grow_fish(f,this,dt);
    ++it;
  }
}
void World::plant_ij_from_pos(Vect pos, int& i, int& j){
  float wx = wc.world_x_len;
  float wy = wc.world_y_len;
  float x = pos.x;
  float y = pos.y;
  //std::cout<<"x,y "<<x<<" "<<y<<" "<<wx<<" "<<wy<<std::endl;
  x = std::fmod(std::fmod(x,wx)+wx,wx);
  y = std::fmod(std::fmod(y,wy)+wy,wy);
  //std::cout<<"x,y "<<x<<" "<<y<<" "<<wx<<" "<<wy<<std::endl;
  i = (int) x*wc.grid_x_size/wc.world_x_len;
  j = (int) y*wc.grid_y_size/wc.world_y_len;
}
void World::plant_cic_ij_from_pos(Vect pos,
				  int& i1,int& j1, float& val1,
				  int& i2,int& j2, float& val2,
				  int& i3,int& j3, float& val3,
				  int& i4,int& j4, float& val4){
  float wx = wc.world_x_len;
  float wy = wc.world_y_len;
  float x = pos.x;
  float y = pos.y;
  x = std::fmod(std::fmod(x,wx)+wx,wx);
  y = std::fmod(std::fmod(y,wy)+wy,wy);
  float dxdi = wc.world_x_len/wc.grid_x_size;
  float dydj = wc.world_y_len/wc.grid_y_size;
  float i = (int)( x/dxdi-0.5);
  float j = (int)( y/dydj-0.5);
  float dx_n = (x/dxdi-(i)-0.5);
  float dy_n = (y/dydj-(j)-0.5);
  //std::cout<<"**"<<dxdi<<" "<<dydj<<std::endl;
  //std::cout<<x/dxdi<<" "<<i<<" "<<dx_n<<std::endl;
  //std::cout<<y/dydj<<" "<<j<<" "<<dy_n<<std::endl;
  val1 = (1-dx_n)*(1-dy_n);
  i1 = wrap_i(i,wc.grid_x_size);
  j1 = wrap_i(j,wc.grid_y_size);

  val2 = (dx_n)*(1-dy_n);
  i2 = wrap_i(i+1,wc.grid_x_size);
  j2 = wrap_i(j,wc.grid_y_size);

  val3 = (1-dx_n)*(dy_n);
  i3 = wrap_i(i,wc.grid_x_size);
  j3 = wrap_i(j+1,wc.grid_y_size);

  val4 = (dx_n)*(dy_n);
  i4 = wrap_i(i+1,wc.grid_x_size);
  j4 = wrap_i(j+1,wc.grid_y_size);
  // std::cout<<i1<<" "<<j1<<" "<<val1<<std::endl;
  // std::cout<<i2<<" "<<j2<<" "<<val2<<std::endl;
  // std::cout<<i3<<" "<<j3<<" "<<val3<<std::endl;
  // std::cout<<i4<<" "<<j4<<" "<<val4<<std::endl;
 
  // std::cout<<"\t"<<val1+val2+val3+val4<<std::endl;
}
float World::get_plant(int i, int j){
  i = wrap_i(i,wc.grid_x_size);
  j = wrap_i(j,wc.grid_y_size);
  return plant_grid[i][j];
}
Vect World::get_grad_plant(int i, int j){
  i = wrap_i(i,wc.grid_x_size);
  j = wrap_i(j,wc.grid_y_size);
  return del_plant_grid[i][j];
}
Vect World::get_grad_plant(Vect pos){
  int i,j;
  plant_ij_from_pos(pos,i,j);
  return get_grad_plant(i,j);
}
Vect World::get_cic_grad_plant(Vect pos){
  int i1,i2,i3,i4,j1,j2,j3,j4;
  float val1,val2,val3,val4;
  plant_cic_ij_from_pos(pos,
			i1,j1,val1,
			i2,j2,val2,
			i3,j3,val3,
			i4,j4,val4);
  Vect result = del_plant_grid[i1][j1]*val1
    +del_plant_grid[i2][j2]*val2
    +del_plant_grid[i3][j3]*val3
    +del_plant_grid[i4][j4]*val4;
  return result;
}
void World::wrap_plant_ij(int& i, int& j){
  i = wrap_i(i,wc.grid_x_size);
  j = wrap_i(j,wc.grid_y_size);
}
float five_pnt_gradient(float dx2n, float dx1n, float dxn1, float dxn2, float dx){
  return (-dxn2+8*dxn1-8*dx1n+dx2n)/(12*dx);
}
void World::calc_del_plant_grid(){
  float dx = wc.world_x_len/wc.grid_x_size;
  float dy = wc.world_y_len/wc.grid_y_size;
  for(int i =0;i<wc.grid_x_size;++i){
    for(int j=0;j<wc.grid_y_size;++j){
      float dx2n, dx1n, dxn1, dxn2;
      float dy2n, dy1n, dyn1, dyn2;
      dx2n = get_plant(i-2,j);
      dx1n = get_plant(i-1,j);
      dxn1 = get_plant(i+1,j);
      dxn2 = get_plant(i+2,j);
      dy2n = get_plant(i,j-2);
      dy1n = get_plant(i,j-1);
      dyn1 = get_plant(i,j+1);
      dyn2 = get_plant(i,j+2);
      del_plant_grid[i][j].x = five_pnt_gradient(dx2n,dx1n,dxn1,dxn2,dx);
      del_plant_grid[i][j].y = five_pnt_gradient(dy2n,dy1n,dyn1,dyn2,dy);
      
    }
  }
};
const std::map<int,Fish*>* World::get_all_fish(){
  return &all_fish;
}
Vect World::dist(Vect v1, Vect v2){
  Vect result = v1-v2;
  if(result.x > wc.world_x_len/2.0){
    result.x -=wc.world_x_len;
  }
  else if(result.x< -wc.world_x_len/2.0){
    result.x+=wc.world_x_len;
  }
  if(result.y>wc.world_y_len/2.0)
    result.y-=wc.world_y_len;
  else if(result.y<-wc.world_y_len/2.0)
    result.y+=wc.world_y_len;
  return result;
}
bool World::cat_eat_by_pos(Fish* pred, Fish* prey){
  float distance = vabs(dist(pred->get_pos(),prey->get_pos()));
  return distance < pred->get_mass() * wc.mass_to_reach;
}
