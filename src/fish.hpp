#ifndef FISH_HPP
#define FISH_HPP

#include "util.hpp"
#include "behavior.hpp"
#include "fish_state.hpp"
#include "definitions.hpp"
/*enum FishType{
  herb,
  carn
  };*/

class Fish;
struct FishState{
  int fish_id;
  float struct_mass;
  float muscle_mass;
  float fat_mass;
  float mass;
  Vect pos;
  Vect vel;
  Vect force;
  bool dead;
  Behavior* bhvr;
  std::string to_string();
  void from_string(std::string);
  FishState();
  FishState(Fish* fish);
  void set(Fish* fish);
};


class Fish{
  friend class FishState;
  int  fish_id; //The unique id of the fish. 
  float mass;   //mass defines how big the fish is. A predator
  // fish can only eat smaller fish. But more massive
  // fish requires more energy to move and has a higher rest motabilism
  float muscle_mass; //This dictacts how much energy can be converted to movement
  // at one time; Muscles consume more energy 
  float structure_mass; //This is the structual mass of the fish, e.i. how
  // big it is/how big is it's mouth etc. More struct mass allows the fish to
  // to eat bigger things.
  float fat_mass; //Energy is used for movement and motabilism. If this hits
  // zero the fish dies. Energy is gained by eating either plants or other
  // fish. Eating other fish provide all the energy to the eater, and a fraction
  // of the mass is converted to energy. Energy is also used to reproduce
  Vect pos; //the position of the fish in the world
  Vect vel; //the current velocity of the fish. They can't change direction instantanously
  //and there is a drag that slows them down.
  Vect force; //The force the fish is exerting on itself to change it's position/vel
  Vect momentum;
  Behavior* bhvr; //This object guides everything that the fish does--the brains & genes of it;
  bool dead=false;
  const WorldConsts& wc;
public:
  Fish(int id,const WorldConsts& wc);
  Fish(FishState& fs,const WorldConsts& wc);
  void convert_energy_to_struct(float mass);
  void convert_energy_to_muscle(float mass);
  void convert_struct_to_energy(float mass);
  void convert_muscle_to_energy(float mass);
  void eat_fish(Fish* fish); //this fish has eaten fish f.
  void eat_plant(float& plant,float dt);
  void motabilise_energy(float dt); //
  bool can_eat_by_size(Fish* f);
  void recalculate_mass();
  Fish* reproduce(float struct_mass, float muscle_mass, float fat_mass, Behavior* bhvr); //produces an new fish asexually <-genes will vary a bit
  Fish* reproduce(Fish* mate); //produce a new fish sexually <- will be implemented later
  
  
  void move(Vect del_pos,float dt);
  void move_to(Vect pos);
  void move_to(float x, float y);
  void periodic_bound(float max_x, float max_y);
  Behavior* get_behavior();
  void set_behavior(Behavior* bhvr);

  Vect get_pos() const;
  Vect get_force() const;
  void set_force(Vect f){
    force=f;
  }
  Vect get_vel() const;
  float get_mass() const{
    return mass;
  }
  float get_muscle_mass() const{
    return muscle_mass;
  }
  float get_structure_mass() const{
    return structure_mass;
  }
  float get_fat_mass() const{
    return fat_mass;
  }
  bool is_dead() const{
    return dead;
  }
  bool is_herbivore() const;
  bool is_predator() const;
  void kill();

};


#endif //fish_hpp
