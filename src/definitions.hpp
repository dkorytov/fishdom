#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

struct WorldConsts{
  float max_plant_density;
  float linear_drag;
  float quad_drag;
  float energy_to_force;
  
  float energy_to_muscle_mass;
  float energy_to_struct_mass;
  float energy_to_fat_mass;
  
  float muscle_mass_to_energy;
  float struct_mass_to_energy;
  float fat_mass_to_energy;

  float daily_muscle_cost;
  float daily_struct_cost;
  float daily_fat_cost; //fat storage cost
  float muscle_efficiency=100;

  
  float world_x_len;
  float world_y_len;
  int   grid_x_size;
  int   grid_y_size;

  float grid_dx;
  float grid_dy;
  
  float struct_to_plant_eating;
  float struct_to_max_plant_eating;
  float struct_to_max_fish_mass;
  float mass_to_reach;
  float fish_to_energy_efficiency;
  float max_plant_density_for_eating;

  float plant_diffusion_coef;
  float plant_growth_exp;
  float plant_growth_linear;
};


#endif //DEFINITIONS_HPP
