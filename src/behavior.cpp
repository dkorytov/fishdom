#include "behavior.hpp"
#include "fish.hpp"
#include "world.hpp"
#include <cmath>
#include <random>


Vect Behavior::get_planned_force(){
  return planned_force;
}
void GradientWalker::sense_world(Fish* fish, World* world,float dt){
  Vect vel = get_planned_force();
  if(vel.is_zero()){
    float angle = 2*M_PI*uniform_random();
    planned_force.set_angle(angle);
  }
  else{
    // float angle = 0.2*M_PI*(normal_random())*sqrt(dt);
    // planned_force.set_angle(angle*1.1+planned_force.angle());
    // planned_force=planned_force*1.0;
    //int i,j;
    //world->plant_ij_from_pos(fish->get_pos(),i,j);

    Vect grad_plant = world->get_cic_grad_plant(fish->get_pos());
    //Vect grad_plant = world->get_grad_plant(fish->get_pos());
    planned_force+=unit(grad_plant)*10000000;
    //const std::map<int,Fish*>* fishes = world->get_all_fish();
    // auto it = fishes->begin();
    // if((int)world->time%20> 10 || false)
    //   for(size_t i =0;i<fishes->size();++i){
	
	
    // 	//int key = (*it).first;
    // 	const Fish* fish1 = (*it).second;
	
    // 	if(fish1 != fish && !fish1->is_dead()){
    // 	  Vect dist = world->dist(fish1->get_pos(),fish->get_pos());
    // 	  float mag = vabs(dist)+15;
    // 	  Vect f = unit(dist)/(mag*mag*mag);
    // 	  //std::cout<<f<<std::endl;
    // 	  if(mag<500){
    // 	    planned_force+= f*-100000.;

    // 	    planned_force+= unit(dist)/(mag)*10;
    // 	  }
    // 	  if(mag<200){
    // 	    planned_force+= unit(fish1->get_vel())*.5;
    // 	  }
    // 	}
    // 	++it;
    //   }
    float vlim = 10.0;
    if(vabs(planned_force) > vlim){
      //std::cout<<planned_force<<" ";
      planned_force = unit(planned_force)*vlim;
    }
    fish->set_force(planned_force);
  }
}
void GradientWalker::grow_fish(Fish* fish, World* world, float dt){
  float fat, muscle, mouth;
  fat = fish->get_fat_mass();
  muscle = fish->get_muscle_mass();
  mouth = fish->get_structure_mass();
  while(fish->get_fat_mass()> 100){
    Fish* baby = fish->reproduce(world->get_fish_id(), 5.0f,5.0f,10.0f,this);
    world->add_fish(baby);
  }
  // std::cout<<"fat: "<<fat<<" muscle: "<<muscle<<" mouth: "<<mouth<<std::endl;
  float extra = fat*1.0 -( muscle+mouth);
  float extra_ratio = extra/(fat+muscle+mouth);
  // std::cout<<"\nExtra: "<<extra<<std::endl;
  float dn = 1.01;
  if(extra_ratio > 0.5){
    float diff_m = mouth-muscle;
    if(diff_m>extra){
      //std::cout<<"a"<<extra<<" "<<diff_m<<std::endl; 
      fish->convert_energy_to_muscle(extra*dt*dn);
    }
    else if(diff_m< -extra){
      //std::cout<<"b"<<extra<<" "<<diff_m<<std::endl; 
      fish->convert_energy_to_struct(extra*dt*dn);
    }
    else{
      float alpha = mouth/(muscle+mouth);
      //std::cout<<"c"<<extra<<" "<<diff_m<<" "<<alpha<<std::endl;
      fish->convert_energy_to_muscle(alpha*extra*dt*dn);
      fish->convert_energy_to_struct((1.0-alpha)*extra*dt*dn);
    }
  }
  else if(extra_ratio < -1){
    float ratio = muscle/(mouth+muscle);
    fish->convert_muscle_to_energy(ratio*-extra*dt*dn);
    fish->convert_struct_to_energy((1-ratio)*-extra*dt*dn);
  }
}
bool GradientWalker::is_herbivore(){
  return true;
}
bool GradientWalker::is_predator(){
  return false;
}

bool RandomWalker::is_herbivore(){
  return true;
}
bool RandomWalker::is_predator(){
  return false;
}
void RandomWalker::grow_fish(Fish* fish,World* world, float dt){
  float fat, muscle, mouth;
  fat = fish->get_fat_mass();
  muscle = fish->get_muscle_mass();
  mouth = fish->get_structure_mass();
  while(fish->get_fat_mass()> 100){
   //  Fish* baby = fish->reproduce(world->get_fish_id(), 5.0f,5.0f,10.0f,this);
   // world->add_fish(baby);
  }
  // std::cout<<"fat: "<<fat<<" muscle: "<<muscle<<" mouth: "<<mouth<<std::endl;
  float extra = fat*1.0 -( muscle+mouth);
  float extra_ratio = extra/(fat+muscle+mouth);
  // std::cout<<"\nExtra: "<<extra<<std::endl;
  float dn = 1.01;
  if(extra_ratio > 0.5){
    float diff_m = mouth-muscle;
    if(diff_m>extra){
      //std::cout<<"a"<<extra<<" "<<diff_m<<std::endl; 
      fish->convert_energy_to_muscle(extra*dt*dn);
    }
    else if(diff_m< -extra){
      //std::cout<<"b"<<extra<<" "<<diff_m<<std::endl; 
      fish->convert_energy_to_struct(extra*dt*dn);
    }
    else{
      float alpha = mouth/(muscle+mouth);
      //std::cout<<"c"<<extra<<" "<<diff_m<<" "<<alpha<<std::endl;
      fish->convert_energy_to_muscle(alpha*extra*dt*dn);
      fish->convert_energy_to_struct((1.0-alpha)*extra*dt*dn);
    }
  }
  else if(extra_ratio < -1){
    float ratio = muscle/(mouth+muscle);
    fish->convert_muscle_to_energy(ratio*-extra*dt*dn);
    fish->convert_struct_to_energy((1-ratio)*-extra*dt*dn);
  }
}
void RandomWalker::sense_world(Fish* fish, World* world, float dt){
    Vect vel = get_planned_force();
  if(vel.is_zero()){
    float angle = 2*M_PI*uniform_random();
    planned_force.set_angle(angle);
  }
  else{
    float angle = 0.2*M_PI*(normal_random())*sqrt(dt);
    planned_force.set_angle(angle*1.1+planned_force.angle());
    planned_force=planned_force*100.0; 
    //Vect grad_plant = world->get_cic_grad_plant(fish->get_pos());
    //Vect grad_plant = world->get_grad_plant(fish->get_pos());
    //planned_force+=grad_plant*1;
    float vlim = 10.0;
    if(vabs(planned_force) > vlim){
      //std::cout<<planned_force<<" ";
      planned_force = unit(planned_force)*vlim;
    }
    fish->set_force(planned_force);
  }
}

void Predator::sense_world(Fish* fish, World* world, float dt){
  const std::map<int,Fish*>* fishes = world->get_all_fish();
  auto it = fishes->begin();
  float min_dist=-1;
  Vect planned_force_hunt;
  for(size_t i =0;i<fishes->size();++i){
    const Fish* fish1 = (*it).second;
    if(fish1 != fish){
      if(!fish1->is_dead() && fish->can_eat_by_size(fish1) && ~fish->is_predator()){
	Vect dist = world->dist(fish1->get_pos(),fish->get_pos());
	float dist_val = vabs(dist);
	if(min_dist == -1 || dist_val < min_dist ){
	  min_dist = dist_val;
	  Vect f = unit(dist);
	  planned_force_hunt = f*1.1;
	}
      }
      if(fish->is_predator()){
	Vect dist = world->dist(fish1->get_pos(),fish->get_pos());
	Vect f = unit(dist);
	planned_force -= f*1.1;
	
      }
    }
    planned_force +=planned_force_hunt;
    ++it;
  }
  float vlim = 10.0;
  if(vabs(planned_force) > vlim){
    //std::cout<<planned_force<<" ";
    planned_force = unit(planned_force)*vlim;
  }

  fish->set_force(planned_force);
}
void Predator::grow_fish(Fish* fish,World* world, float dt){
  float fat, muscle, mouth;
  fat = fish->get_fat_mass();
  muscle = fish->get_muscle_mass();
  mouth = fish->get_structure_mass();
  // while(fish->get_fat_mass()> 100){
  //   // std::cout<<"============="<<std::endl;
  //   // Fish* baby = fish->reproduce(world->get_fish_id(), 5.0f,5.0f,10.0f,new Predator());
  //   // world->add_fish(baby);
    
  // }

  // while(fish->get_fat_mass()> 100){
  //  // Fish* baby = fish->reproduce(5.0f,5.0f,10.0f,this);
  //  // world->add_fish(baby);
  // }
  // std::cout<<"fat: "<<fat<<" muscle: "<<muscle<<" mouth: "<<mouth<<std::endl;
  float extra = fat*1.0 -( muscle+mouth);
  float extra_ratio = extra/(fat+muscle+mouth);
  // std::cout<<"\nExtra: "<<extra<<std::endl;
  float dn = 1.01;
  if(extra_ratio > 0.5){
    float diff_m = mouth-muscle;
    if(diff_m>extra){
      //std::cout<<"a"<<extra<<" "<<diff_m<<std::endl; 
      fish->convert_energy_to_muscle(extra*dt*dn);
    }
    else if(diff_m< -extra){
      //std::cout<<"b"<<extra<<" "<<diff_m<<std::endl; 
      fish->convert_energy_to_struct(extra*dt*dn);
    }
    else{
      float alpha = mouth/(muscle+mouth);
      //std::cout<<"c"<<extra<<" "<<diff_m<<" "<<alpha<<std::endl;
      fish->convert_energy_to_muscle(alpha*extra*dt*dn);
      fish->convert_energy_to_struct((1.0-alpha)*extra*dt*dn);
    }
  }
  else if(extra_ratio < -1){
    float ratio = muscle/(mouth+muscle);
    fish->convert_muscle_to_energy(ratio*-extra*dt*dn);
    fish->convert_struct_to_energy((1-ratio)*-extra*dt*dn);
  }
}
bool Predator::is_herbivore(){
  return false;
}
bool Predator::is_predator(){
  return true;
}
