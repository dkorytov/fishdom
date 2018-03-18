#ifndef BEHAVOIR_HPP
#define BEHAVOIR_HPP

#include <gtkmm.h>
#include "util.hpp"

class Fish;
class World;
class Behavior{
  //FishType fishtype; //carnivore or herbavore
  float prefered_muscle; //the ratios of prefered fat/struct/muscle
  float prefered_struct; //the fish will try to keep these levels balanced when there
  float prefered_fat;    //is extra food around->otherwise it dips into the fat

protected:
  Vect planned_force;
public:
  //virtual FishType get_fish_type();
  //virtual void adjust_mass();
  virtual void sense_world(Fish* fish, World* world,float dt)=0;
  virtual void grow_fish(Fish* fish,World* world, float dt)=0;
  virtual bool is_predator() = 0;
  virtual bool is_herbivore() = 0;
  Vect get_planned_force();
};

class RandomWalk:public Behavior{
  virtual void sense_world(Fish* fish, World* world,float dt);
  virtual void grow_fish(Fish* fish,World* world, float dt);
  virtual bool is_predator();
  virtual bool is_herbivore();

};

class GradientWalker: public Behavior{
  virtual void sense_world(Fish* fish, World* world, float dt);
  virtual void grow_fish(Fish* fish,World* world, float dt);
  virtual bool is_predator();
  virtual bool is_herbivore();
};

class Predator: public Behavior{
  virtual void sense_world(Fish* self, World* world, float dt);
  virtual void grow_fish(Fish* fish,World* world, float dt);
  virtual bool is_predator();
  virtual bool is_herbivore();
};
class NNHerb: public Behavior{
  virtual void sense_world(Fish* self, World* world, float dt);
  virtual void grow_fish(Fish* fish,World* world, float dt);
};
class NNPred: public Behavior{
  virtual void sense_world(Fish* self, World* world, float dt);
  virtual void grow_fish(Fish* fish,World* world, float dt);
};
#endif 
