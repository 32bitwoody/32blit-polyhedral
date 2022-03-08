#pragma once

#include <cstdint>

#include "32blit.hpp"


using namespace blit;

class dial {
 private:
  const uint16_t screen_width = 320;
  const uint16_t screen_height = 240;
  uint32_t start_ms;
  uint32_t stop_ms;
  uint32_t elap_ms;
  uint32_t elap_min=0;


float angle = 0.0f;
float angle_min = 0.0f;

float lastangle = 0.0f;
float lastangle_min = 0.0f;

int tick_count = 0;

std::array<Vec2, 4> pointer = {
    Vec2(0, 8),
    Vec2(-8, 0),
    Vec2(0, -108),
    Vec2(8, 0)
  };
  
std::array<Vec2, 4> minhand = {
    Vec2(0, 10),
    Vec2(-10, 0),
    Vec2(0, -90),
    Vec2(10, 0)
  };
  std::array<Vec2, 4> screw = {
    Vec2(24, 24),
    Vec2(296, 24),
    Vec2(24,216),
    Vec2(296, 216)
  };
std::array<Vec2, 12> tickmark = {
    Vec2(160,  5),
    Vec2(218,  21),
    Vec2(260,  63),
    Vec2(275, 120),
    Vec2(260, 178), 
    Vec2(218, 220), 
    Vec2(160, 235),
    Vec2(103, 220), 
    Vec2( 60, 178),
    Vec2( 45, 120),
    Vec2( 60, 63),
    Vec2(103, 20)
  };
Vec2 offset;
Mat3 tr1;
Mat3 tr2;

	public:
  Point dialcentre;
  uint8_t dialsize;


dial();
float deg2rad(float a) {

void move(void);
void init(Point c,uint8_t d);
void render(uint32_t time); 
void update(uint32_t time);
};
