
#include <math.h>
#include <cmath>
#include <random>
#include <chrono>
#include "util.hpp"
#include <iostream>
Vect::Vect():x(0),y(0){}
Vect::Vect(float radian){
  set_angle(radian);
}
Vect::Vect(float x,float y):x(x),y(y){}
Vect::Vect(const Vect& vect):x(vect.x),y(vect.y){}
Vect Vect::operator+(const Vect& vec2){
  return Vect(x+vec2.x,y+vec2.y);
}
Vect Vect::operator-(const Vect& vec2){
  return Vect(x-vec2.x,y-vec2.y);
}
Vect& Vect::operator=(const Vect& vec2){
  x = vec2.x;
  y = vec2.y;

  return *this;
}
Vect Vect::operator*(const float& f){
  return Vect(f*x,f*y);
}

Vect Vect::operator*(const Vect& vec){
  return Vect(vec.x*x,vec.y*y);
}
Vect Vect::operator/(const float& f){
  Vect v(x/f,y/f);
  return v;
}
Vect& Vect::operator+=(const Vect& vec){
  x+=vec.x;
  y+=vec.y;
  return *this;
}
Vect& Vect::operator-=(const Vect& vec){
  x-=vec.x;
  y-=vec.y;
  return *this;
}

void Vect::add(float x1, float y1){
  x+=x1;
  y+=y1;

}
void Vect::add(Vect v){
  add(v.x,v.y);
}
void Vect::periodic_bound(float max_x, float max_y){
  ::periodic_bound(x,max_x);
  ::periodic_bound(y,max_y);
}
bool Vect::is_zero() const{
  return x==0 && y==0;
}
float Vect::angle() const{
  return std::atan2(y,x);
}
void Vect::set_angle(float rad){
  x = std::cos(rad);
  y = std::sin(rad);
}
float Vect::abs() const{
  return sqrt(x*x+y*y);
}
void periodic_bound(float& x, float max_x){
  x=fmod(x,max_x);
  if(x<0)
    x +=max_x;
}
static std::default_random_engine rnd_generator;
static std::uniform_real_distribution<float>* rnd_unfm_distribution = NULL;
static std::normal_distribution<float>* rnd_norm_distribution = NULL;
float uniform_random(){
  if(rnd_unfm_distribution == NULL){
    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //std::default_random_engine generator (seed);
    rnd_unfm_distribution = new std::uniform_real_distribution<float>(0,1);
  }
  std::random_device rd;
  return (*rnd_unfm_distribution)(rd);
  //return (*rnd_unfm_distribution)(rnd_generator);
}
float normal_random(){
  if(rnd_norm_distribution == NULL){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    rnd_norm_distribution = new std::normal_distribution<float>(0,1);
  }
  return (*rnd_norm_distribution)(rnd_generator);
}

std::ostream& operator<< (std::ostream& stream, const Vect& vect){
  return stream<<"("<<vect.x<<", "<<vect.y<<")";
}
float vabs(const Vect v){
  return sqrt(v.x*v.x+v.y*v.y);
}

Vect unit( Vect v){
  float mag = vabs(v);
  if(mag != 0)
    return v/mag;
  else
    return v;
}
