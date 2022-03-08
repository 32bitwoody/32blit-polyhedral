#pragma once

#include <cstdint>

#include "32blit.hpp"
#include "constants.hpp"
#include "dial.hpp"
#include "vehicle.hpp"
#include "polyhedron.hpp"

using namespace blit;

uint32_t update_ms_start;
uint32_t render_ms_start;
uint32_t update_ms_end;
uint32_t render_ms_end;
uint32_t ufps;
uint32_t rfps;


bool buttonA=false;
bool moveObj=false;
bool buttonB=false;
bool buttonY=false;
bool buttonX=false;
bool buttonMENU=false;
bool fire_laser=false;
bool dpadL=false;
bool dpadR=false;
bool dpadU=false;
bool dpadD=false;

bool addObj=false;
bool showdata=false;

float dH,dA;
uint32_t ptime;

Vec3 viewang;
Vec3 viewvec;
Vec3 viewpos;
float speed;

float laser_aim=40;
float laser_range=2000;
int_fast32_t laser_temp=0;
int_fast32_t laser_indic;
  uint32_t score;
  float stick_value;

  ViewPyramid view;

  dial heading;
  dial pitchroll;
  dial radardial;
  
//vehicle shipy;
 uint_fast32_t count;
  vehicle shipy[FLEET];
 int32_t working_z[FLEET];
  int32_t working_s[FLEET];

 int32_t prev_z;
  int32_t max_z;
uint32_t space_distance;
Vec3 space_vec[FLEET];
Vec3 space_pos[FLEET];

//    vehicle shipyb;

    Vec3 radar[FLEET];


//polyhedron testshape[2];


void init();
void update(uint32_t time);
void update_button(uint32_t time);

  void update_space(uint32_t time);
void update_fps(uint32_t time);
void update_laser(uint32_t time);

void vector_fromangle();


void render(uint32_t time);
void render_crosshair();
void render_indicators();

void render_laser();

void render_fps(uint32_t time);
void drawcircle(Point centre, uint_fast16_t radius);

