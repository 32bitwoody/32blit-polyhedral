#pragma once

#include <cstdint>

#include "32blit.hpp"
#include "constants.hpp"
#include "assets.hpp"
#include "polyhedron.hpp"


using namespace blit;

class vehicle {
	private:
    int32_t hitpoints;
    bool turnflag;
    bool move_toward=true;
 uint32_t move_t=0;

    Vec3 direction;
    uint32_t serial;
    uint8_t polyid;
    Vec3 viewer_distance;
    float view_dist_lgth;
    Vec3 bearing;
    float bear_range;
    Vec3 approach;
    Vec3 course_correct;
    float_t safe_approach;
    float_t working_safe_approach;
Vec3 heading;

    uint8_t evasion=0;
    uint32_t last_update;
    uint32_t last_move;

	public:
  polyhedron binky;
  bool adj=false;

//    Vec3 viewang = Vec3(0,0,0);
//    Vec3 viewpos = Vec3(0,0,0);
//        Vec3 viewvec = Vec3(0,0,1);

//    Vec3 pos=Vec3(0,0,320);
    Vec3 radar[FLEET];
    uint32_t radar_d[FLEET];
    bool radar_exist[FLEET];

    Vec3 velocity;
    bool exist=true;
    float speed;
    bool forward;
    bool rotate;
    bool accelerate;
    int32_t z;
    bool rendered=false;
    uint8_t count;

vehicle();
  void init(ViewPyramid &view,uint8_t v_type,uint8_t id);
  void set(Vec3 objang, Vec3 objpos);

  void render(uint32_t time); 
  void update(uint32_t time,Vec3 viewang,Vec3 viewpos,Vec3 viewvec);
  void update_move(uint32_t time,Vec3 viewpos);
  void update_move_evade();
  void update_move_toward(Vec3 viewpos);
  void update_move_aim(Vec3 viewpos,float_t heading_length);
  void update_move_pass(Vec3 viewpos,float_t heading_length);

  void update_radar();
  void update_velocity();

  Vec3 position();
  Vec2 position_view();

  void adjustpos(Vec3 adjpos);
  void adjustang(Vec3 adjang);
  float_t laser_hit(float_t damage);
  void ploded();
};
