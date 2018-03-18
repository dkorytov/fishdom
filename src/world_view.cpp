
#include <iostream>
#include <math.h>
#include "world_view.hpp"
#include "fish_state.hpp"
WorldView::WorldView(World* world):world(world){
  Glib::signal_timeout().connect( sigc::mem_fun(*this, &WorldView::on_timeout), 100 );
}
WorldView::~WorldView(){
  
}
bool WorldView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
  draw_plant(world->plant_grid,cr);
  draw_plant_grad(world->del_plant_grid,cr);
  std::vector<FishState> fish_states = world->get_fish_states();
  cr->save();
  cr->translate(100,100);
  Pango::FontDescription font;
  //font.set_family("Monospace");
  font.set_weight(Pango::WEIGHT_BOLD);
  // http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html

  float angle =0; 
  angle+=world->time/M_PI/2.0;
  Vect v(angle);

  std::stringstream ss;
  ss<<world->time<<" "<<v.angle()<<std::endl;
  auto layout = create_pango_layout(ss.str());

  layout->set_font_description(font);
  layout->show_in_cairo_context(cr);
  
  cr->translate(100,100);
  cr->rotate(angle);
  cr->line_to(0,0);
  cr->line_to(100,0);
  cr->stroke();
  cr->restore();
  for(auto itf = fish_states.begin();itf!=fish_states.end();++itf){
    FishState* fs = &*itf;
    draw_fish(fs,cr);
  }
  return true;
}
void WorldView::draw_fish(const FishState* fs, const Cairo::RefPtr<Cairo::Context>& cr){
  
  cr->save();
  float radius = pow((3.0*fs->fat_mass)/(4.0*M_PI*world->fish_mass_density),1.0/3.0);
  //4/3pi r^3
  if(fs->dead){
    cr->set_source_rgb(0.0,0.3,0.3);
  }
  else{
    cr->set_source_rgb(0.1,0.1,0.6);
  }
  //cr->translate(fs->pos.x,fs->pos.y);
  cr->translate(fs->pos.x,fs->pos.y);
  cr->arc(0,0,radius,0,2.0*M_PI);
  cr->fill();
  //cr->stroke();

  //drawing the head
  cr->save();
  //pi r^3 /3 *(h/r)
  float hr = 1.;
  float r = pow((3.0*fs->struct_mass)/(M_PI*hr*world->fish_mass_density),1.0/3.0);
  //std::cout<<"fs: "<<fs->vel.x<<" "<<fs->vel.y<<std::endl;
  //std::cout<<"\t"<<fs->force.x<<" "<<fs->force.y<<std::endl;

  if(!fs->force.is_zero()){
    //cr->rotate((fs->force.angle()+fs->vel.angle()+3.14*8)/2.0); //rotate to match planned direction
    float fa = fs->force.angle();
    float va = fs->vel.angle();
    float angle = (fa+va)/2.0;
    if(fabs(fa-va)>M_PI)
      angle+=M_PI;
    cr->rotate(angle);
  }
  else
    cr->rotate(fs->vel.angle());
  cr->translate(radius,0);
  cr->move_to(hr*r,0);
  cr->line_to(0,r);
  cr->line_to(0,-r);
  cr->line_to(hr*r,0);
  cr->fill();
  //cr->stroke();
  cr->restore();

  //drawing the body
  cr->save();
  cr->rotate(fs->vel.angle()); //rotate to match the velocity direction
  hr = 4.0;
  r = pow((3.0*fs->muscle_mass)/(M_PI*hr*world->fish_mass_density),1.0/3.0);
  cr->translate(-radius,0);
  cr->move_to(-hr*r,0);
  cr->line_to(0,r);
  cr->line_to(0,-r);
  cr->line_to(-hr*r,0);
  cr->fill();
  //cr->stroke();
  cr->restore();

  cr->restore();
}
void WorldView::draw_plant(const std::vector<std::vector<float> > plants,
			   const Cairo::RefPtr<Cairo::Context>& cr){
  cr->save();
  for(uint i =0;i<plants.size();++i){
    for(uint j = 0;j<plants[i].size();++j){
      float val = plants[i][j]/world->wc.max_plant_density;
      float val3 = (1.0-val);
      float val2 = 1.0-val*.5;
      cr->set_source_rgb(val3,val2,val3);
      //cr->set_source_rgb(0,val,0);
      cr->rectangle(100*i,100*j,100,100);
      cr->fill();
    }
  }
  cr->restore();
}

void WorldView::draw_plant_grad(const std::vector<std::vector<Vect> > d_plant,const Cairo::RefPtr<Cairo::Context>& cr){
  cr->save();
  for(uint i =0;i<d_plant.size();++i){
    for(uint j=0;j<d_plant[i].size();++j){
      float dx,dy;
      dx = d_plant[i][j].x;
      dy = d_plant[i][j].y;
      cr->line_to(100*i+50,100*j+50);
      cr->line_to(100*i+50+dx*50,100*j+50+dy*50);
      cr->stroke();
    }
  }
  cr->restore();
  
}
bool WorldView::on_timeout(){
 auto win = get_window();
    if (win)
    {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
    return true;
}
