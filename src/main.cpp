#include <iostream>
#include <gtkmm.h>


#include "dtk/param.hpp"
#include "all.hpp"

#include "world_view.hpp"

#include <thread>
#include <chrono>
struct RunWorldParam{
  float dt;
  float sleep_time;
  int total_time;
};


void run_world(World* wc, RunWorldParam* param){
  int sleep = 1000*param->sleep_time;
  for(int i =0;i<param->total_time;++i){
    //std::cout<<i<<"/"<<param->total_time<<std::endl;
    wc->step(param->dt);
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
  }
}


int main(int argc, char** argv){
  std::cout<<"This is the begining of the fishdom program :)"<<std::endl;
  float mt = 1.; //factor to increase motabalism/eating
  WorldConsts wc;
  wc.max_plant_density = 256;
  wc.max_plant_density_for_eating=128;
  wc.linear_drag = 30.5;
  wc.quad_drag  = .2;             
  wc.energy_to_force = 1;       
                         
  wc.energy_to_muscle_mass =.1;
  wc.energy_to_struct_mass =.5;
  wc.muscle_mass_to_energy =0.5; 
  wc.struct_mass_to_energy =0.0;
  wc.daily_muscle_cost = 0.1*mt;     
  wc.daily_struct_cost = 0.05*mt;     
  wc.daily_fat_cost = 0.001*mt;
  wc.muscle_efficiency = 300.0*mt;
  wc.world_x_len = 1000;
  wc.world_y_len = 1000;
  wc.grid_x_size = 10;
  wc.grid_y_size = 10;
  wc.grid_dx = 100;
  wc.grid_dy = 100;
  wc.struct_to_plant_eating = .2*mt;
  wc.struct_to_max_plant_eating = 1.0*mt;
  wc.struct_to_max_fish_mass=1.0*mt;
  wc.fish_to_energy_efficiency=1.0;
  
  wc.plant_diffusion_coef = 0.0001*mt;
  wc.plant_growth_exp = 0.001*mt;
  wc.plant_growth_linear = 1*mt;

  World world(wc);
  world.add_fish(1);
  auto fishs = world.get_fish_states();
  for(size_t i =0;i<fishs.size();++i){
    std::cout<<"fish "<<fishs[i].fish_id<<" pos: "<<fishs[i].pos.x<<" "<<fishs[i].pos.y<<std::endl;
  }
  auto app =  Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");
  Gtk::Window window;
  window.set_default_size(1000, 1000);
  WorldView world_view(&world);
  window.add(world_view);
  world_view.show();

  std::thread t;
  float factor=6;
  RunWorldParam param;
  param.sleep_time =0.005*factor;
  param.total_time = 1000000;
  param.dt = 0.01*factor;
  t=std::thread(run_world,&world,&param);
  app->run(window);
  t.join();
  return 0;
}
