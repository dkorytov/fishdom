#ifndef UTIL_HPP
#define UTIL_HPP
#include <iostream>


struct Vect{
  float x;
  float y;
  Vect();
  Vect(float x, float y);
  Vect(float radian);
  Vect(const Vect& vect);
  Vect operator+(const Vect& vec2);
  Vect operator-(const Vect& vec2);
  Vect operator*(const float& f);
  Vect operator*(const Vect& vec);
  Vect operator/(const float& f);
  Vect& operator=(const Vect& vec);
  Vect& operator+=(const Vect& vec);
  Vect& operator-=(const Vect& vec);
  void add(float x, float y);
  void add(Vect v);
  void periodic_bound(float max_x, float max_y);
  bool is_zero() const;
  float angle() const;
  void set_angle(float rad);
  float abs() const;
};

struct Pos{
  float x;
  float y;
  
  Pos operator+(const Pos& pos2);
  Pos operator*(const float& f);

};
struct Vel{
  float vx;
  float vy;
  Vel operator+(const Pos& pos2);
  Vel operator*(const float& f);
  Pos to_pos(float factor);
};
struct Force{
  float fx;
  float fy;
  Force operator+(const Pos& pos2);
  Force operator*(const float& f);
  Vel to_vel(float factor);
};

void periodic_bound(float& x, float max_x);

float uniform_random();
float normal_random();

std::ostream& operator<< (std::ostream& stream, const Vect& vect);

float vabs(const Vect v);
Vect  unit(const Vect v);


#endif //UTIL_HPP
