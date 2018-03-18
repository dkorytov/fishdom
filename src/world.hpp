
#ifndef WORLD_HPP
#define WORLD_HPP
#include <map>
#include <vector>
#include "fish.hpp"


class World{
public:
  const WorldConsts& wc;
  const float fish_mass_density=.01;
  std::map<int,Fish*> all_fish;
  std::vector<std::vector<float> > plant_grid;
  std::vector<std::vector<Vect> > del_plant_grid;
  float time;
  int time_step;
  int fish_num=0;
public:
  World(const WorldConsts& wc);
  void step(float dt);
  void step_defuse_plants(float dt);
  void step_grow_plants(float dt);
  void step_move_fish(float dt);
  void step_interact_fish(float dt);
  void step_fish_sense(float dt);
  void step_grow_fish(float dt);
  void add_fish(Fish* fish);
  void remove_fish(Fish* fish);
  void add_fish(int num);
  void add_pred(int num);
  void plant_ij_from_pos(Vect pos, int& i, int& j);
  void wrap_plant_ij(int& i, int& j);
  float get_plant(int i, int j);
  float get_plant(Vect pos);
  Vect get_grad_plant(int i, int j);
  Vect get_grad_plant(Vect pos);
  Vect get_cic_grad_plant(Vect pos);
  void calc_del_plant_grid();
  std::vector<FishState> get_fish_states();
  std::vector<std::vector<int> > get_plant_state();
  const std::map<int,Fish*>* get_all_fish();
  Vect dist(Vect v1, Vect v2);
  void plant_cic_ij_from_pos(Vect pos,
				    int& i1,int& j1, float& val1,
				    int& i2,int& j2, float& val2,
				    int& i3,int& j3, float& val3,
				    int& i4,int& j4, float& val4);
  bool cat_eat_by_pos(Fish* pred, Fish* prey);
};




#endif //world_hpp
