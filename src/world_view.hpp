#ifndef WORLD_VIEW_HPP
#define WORLD_VIEW_HPP

#include <gtkmm/drawingarea.h>

#include "world.hpp"
#include "fish_state.hpp"
class WorldView : public Gtk::DrawingArea{
  World* world;
public:
  WorldView(World* world);
  virtual ~WorldView();
  
protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  bool on_timeout();
  void draw_fish(const FishState* fs,const Cairo::RefPtr<Cairo::Context>& cr);
  void draw_plant(const std::vector<std::vector<float> > plants, const Cairo::RefPtr<Cairo::Context>& cr);
  void draw_plant_grad(const std::vector<std::vector<Vect> > dxdy_plant,const Cairo::RefPtr<Cairo::Context>& cr);
};


#endif  //WORLD_VIEW_HPP
