#include "fish.hpp"
#include <iostream>

FishState::FishState(Fish* fish){
  set(fish);
}
FishState::FishState(){}

void FishState::set(Fish* fish){
  fish_id=fish->fish_id;
  struct_mass=fish->structure_mass;
  muscle_mass=fish->muscle_mass;
  fat_mass=fish->fat_mass;
  mass=fish->mass;
  pos.x=fish->pos.x;
  pos.y=fish->pos.y;
  vel=fish->vel;
  force=fish->get_force();
  dead = fish->is_dead();
  is_predator = fish->is_predator();
  return;
}


Fish::Fish(int id,const WorldConsts& wc):fish_id(id),
					  wc(wc){
  muscle_mass = 1.0;
  structure_mass = 1.0;
  fat_mass = 2.0;
  mass = muscle_mass+structure_mass+fat_mass;
  pos.x = 10;
  pos.y = 10;
  vel.x = 0;
  vel.y = 0;
  force.x = 0;
  force.y = 0;
  bhvr = NULL;
  dead = false;
}

Fish::Fish(int id, FishState& fs,const WorldConsts& wc):fish_id(id),wc(wc){
  fish_id = fs.fish_id;
  structure_mass = fs.struct_mass;
  muscle_mass = fs.muscle_mass;
  fat_mass = fs.fat_mass;
  pos = fs.pos;
  vel = fs.vel;
  force = fs.force;
  bhvr = fs.bhvr;
  recalculate_mass();
}
void Fish::eat_plant(float& plant,float dt){
  float max_plant =  std::min(wc.max_plant_density_for_eating,plant);
  float amount = std::min(max_plant * wc.struct_to_plant_eating * structure_mass * dt,
			  wc.struct_to_max_plant_eating * structure_mass * dt);
  
  amount = std::min(amount,plant);
  plant -= amount;
  fat_mass +=amount;
}
void Fish::eat_fish(Fish* f){
  float energy_fat = f->get_fat_mass() * wc.fat_mass_to_energy;
  float energy_struct = f->get_structure_mass() * wc.struct_mass_to_energy;
  float energy_muscle = f->get_muscle_mass() * wc.muscle_mass_to_energy;
  float total_energy = (energy_fat + energy_struct + energy_muscle) * wc.fish_to_energy_efficiency;
  std::cout<<"total energy:"<<total_energy<<std::endl;
  this->fat_mass += total_energy * wc.energy_to_fat_mass;
  f->kill();
}
void Fish::motabilise_energy(float dt){
  float cost = wc.daily_muscle_cost*muscle_mass*dt+
    wc.daily_struct_cost*structure_mass*dt+
    wc.daily_fat_cost*fat_mass*dt;
  fat_mass -= cost;
  if(fat_mass<0){
    muscle_mass-= -fat_mass/wc.muscle_mass_to_energy;
    fat_mass = 0;
    if(muscle_mass<=0){
      this->kill();
      muscle_mass=0;
    }
  }
}
void Fish::recalculate_mass(){
  mass = muscle_mass + structure_mass + fat_mass;
  vel = momentum/mass;
}
void Fish::move(Vect del_pos, float dt){
  this->pos+=del_pos;
  vel = del_pos/dt;
  momentum = vel*mass;
}

void Fish::move_to(Vect pos){
  this->pos = pos;
}
void Fish::move_to(float x,float y){
  this->pos.x=x;
  this->pos.y=y;
}
void Fish::periodic_bound(float max_x, float max_y){
  pos.periodic_bound(max_x,max_y);
}

Behavior* Fish::get_behavior(){
  return bhvr;
}
void Fish::set_behavior(Behavior* bhvr){
  this->bhvr = bhvr;
}
Vect Fish::get_pos() const {
  return pos;
}
Vect Fish::get_force() const{
  return force;
}
Vect Fish::get_vel() const{
  return vel;
}
void Fish::convert_energy_to_struct(float mass){
  if(mass > 0){
    //std::cout<<energy<<" struct: "<<structure_mass;
    if(mass<fat_mass){
      structure_mass += mass*wc.energy_to_muscle_mass;
      fat_mass -= mass;
    }
    else{
      structure_mass += fat_mass*wc.energy_to_muscle_mass;
      fat_mass =0;
    }
  }
  //std::cout<<"->"<<structure_mass<<std::endl;
}
void Fish::convert_energy_to_muscle(float mass){
  if(mass > 0){
    //  std::cout<<energy<<" struct: "<<muscle_mass;
    if(mass<fat_mass){
      muscle_mass += mass*wc.energy_to_muscle_mass;
      fat_mass -= mass;
    }
    else{
      muscle_mass += fat_mass*wc.energy_to_muscle_mass;
      fat_mass =0;
    }
    //std::cout<<"->"<<muscle_mass<<std::endl;
  }
}
void Fish::convert_muscle_to_energy(float mass){
  if(mass > 0){
    if(muscle_mass > mass){
      fat_mass = mass*wc.muscle_mass_to_energy;
      muscle_mass -= mass;
    }
    else{
      fat_mass = muscle_mass*wc.muscle_mass_to_energy;
      muscle_mass = 0;
    }
  }
}
void Fish::convert_struct_to_energy(float mass){
  if(mass > 0){
    if(structure_mass > mass){
      fat_mass = mass*wc.struct_mass_to_energy;
      structure_mass -= mass;
    }
    else{
      fat_mass = structure_mass*wc.struct_mass_to_energy;
      structure_mass = 0;
    }
  }
}
Fish* Fish::reproduce(float struct_mass, float muscle_mass, float energy_mass, Behavior* bhvr){
  float required_energy  = 0;
  required_energy+= struct_mass/wc.energy_to_struct_mass
    +muscle_mass/wc.energy_to_muscle_mass
    +energy_mass;
  if(this->fat_mass<required_energy)
    return NULL;
  this->fat_mass -=required_energy;
  FishState fs;
  fs.fish_id = 0;
  fs.struct_mass = struct_mass;
  fs.muscle_mass = muscle_mass;
  fs.fat_mass = energy_mass;
  fs.pos = pos;
  fs.vel = vel;
  fs.dead = false;
  fs.bhvr = bhvr;
  //TODO add id;
  Fish* baby = new Fish(0, fs, wc);
  return baby;
}
bool Fish::can_eat_by_size(Fish* f){
  return structure_mass * wc.struct_to_max_fish_mass > f->get_mass() ; 
}
void Fish::kill(){
  dead = true;
}
bool Fish::is_herbivore() const{
  return bhvr->is_herbivore();
}
bool Fish::is_predator() const{
  return bhvr->is_predator();
}
