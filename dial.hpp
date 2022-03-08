#pragma once

#include <cstdint>

#include "32blit.hpp"
#include "constants.hpp"

using namespace blit;

class dial {
  
	private:
  Point dialcentre;
  uint8_t dialsize;
  uint8_t type;
  float needleangle;
  Vec2 needleheight;
  float lastangle = 0.0f;
  Mat3 tr1;
  std::array<Vec2, 6> pointer;
  float nhmulti;
  float namulti;
  bool invert;
	public:
    Vec3 log[FLEET];

    Vec3 viewang = Vec3(0,0,0);
    Vec3 viewpos = Vec3(0,0,0);
    

dial();
float deg2rad(float a);
void init(Point c,uint8_t d,uint8_t t);
void render(uint32_t time); 
void render_type0(uint32_t time);
void render_type1(uint32_t time);
void render_type2(uint32_t time);

void update(uint32_t time,float na,float nh);
};
