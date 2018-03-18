#ifndef FISH_FACTORY_HPP
#define FISH_FACTORY_HPP

#include <map>

#include "fish.hpp"

class FishFactory{
  std::map<int,Fish*> fish_map;
  FishFactory();
public:
  Fish* create_default_fish();
  Fish* create_fish();

};



#endif //FISH_FACTORY_HPP
