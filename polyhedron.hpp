#pragma once

#include <cstdint>

#include "32blit.hpp"
#include "constants.hpp"
#include "assets.hpp"
#include "object.hpp"

using namespace blit;

class polyhedron {
	private:
	  File plyfile;
    char text_buf[2] = {0};
    char plybuffer[100] = {0};
    Object *pyramid_object;
//    int16_t depth_buffer[320][240];
    uint8_t polyid;
 float_t half_fov;
  uint32_t serial;
  
	public:
Vec2 model_max_2D;
Vec2 model_min_2D;
    ViewPyramid view;
    Vec3 model_pt[POINTS];
    bool model_pt_shw[POINTS];
    int model_fc[FACES][3];
    bool model_fc_shw[FACES];

    Pen model_col[FACES];
//    Vec3 model_nm[20];
//    Vec3 model_nm_rot[20];
    Vec3 viewobjptvec[POINTS];
    Vec3 viewobjfcvec[FACES];

    Vec3 objpos = Vec3(0,0,800);
    Vec3 objang = Vec3(3,0,0);
    Vec3 objvec = Vec3(1,0,0);

    Vec3 viewobjang = Vec3(3,0,0);
    Vec3 viewobjvec = Vec3(3,0,0);

    Vec3 viewobjpos = Vec3(0,0,0);
    
    std::array<Point, 3> model_view_2D;
    std::vector<std::vector<Point>> disp_fc; 
    std::vector<int> disp_z;
int pt_c;
int fc_c;
uint16_t ez;
Vec2 screen_off = Vec2(160, 120);
    Vec3 co,so; //store sine and cosine for rotation


//std::vector<Vec3> nstnc; 
 Vec3 model_pt_rot[20]; // rotated representation
//Vec3 pos;
//std::vector<Vec3> pstnc; 
 Vec3 model_pt_viewpos[20]; // viewed position representation

//Vec3 avl = Vec3(0,0,0);
//int max_z=0; 
//int min_z=0; 



// bounding box/ size / distance from centre point of most distant tip 

polyhedron();
  void init(ViewPyramid &view,uint8_t id,uint32_t serial);
  void render(bool dump);
  void old_drawpolyhedron(void);

void holddrawtri(int i);
void drawtri(int i);

void update(Vec3 viewang,Vec3 viewpos, Vec3 viewvec);
void dummy_ply(void);
void model_load(char *obj_file);

void update_rotate(Vec3 viewang);
void update_translate(Vec3 viewang,Vec3 viewpos);
void update_visible(Vec3 viewpos);


Vec3 vector_toangle(Vec3 vector);

Vec3 vector_fromangle();
void vector_unit();
Vec3 vector_cross(Vec3 vectora,Vec3 vectorb);


float_t update_dotproduct(Vec3 viewvec,Vec3 normvec);
void vector_trig(Vec3 angle);
//void update_rotate_normals(Vec3 viewang,uint8_t i);

void flatten_tri(int i);
Point flatten_v(Vec3 v);

int32_t orient2d(Point p1, Point p2, Point p3);
bool is_top_left(const Point &p1, const Point &p2);

void plot_triangle(int i);
void plot_points(int i);

};
