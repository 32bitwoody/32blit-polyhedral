#pragma once

#include <cstdint>

#include "32blit.hpp"

using namespace blit;

class polyhedron {
	private:
	File tfile;
char text_buf[100] = {0};
	public:
std::vector<Vec3> model_pt;
//std::array<Vec2, 3> nstnc_flt;
std::array<Point, 3> nstnc_flt;
//std::vector<Vec3> model_fc;
std::vector<std::vector<int>> model_fc; 
std::vector<Pen> model_col;
int pt_c;
int fc_c;
uint16_t ez;
Vec2 screen_off = Vec2(100, 100);


std::vector<Vec3> nstnc; // rotational representation
Vec3 pos;
std::vector<Vec3> pstnc;  // positional representation
//Vec3 rot;  //Not required
Vec3 vel;
std::array<Vec3, 2> vlimit;
Vec3 avl;
std::array<Vec3, 2> avlimit;
Vec3 acc;
std::array<Vec3, 2> alimit;
Vec3 rac;
std::array<Vec3, 2> ralimit;

//std::array<Vec3, 3> face;

// bounding box/ size / distance from centre point of most distant tip 

std::vector<Point> quad;

 polyhedron();
void dummy_quad(void);
void drawpoly(void);
void holddrawtri(int i);
void drawtri(int i);

void display(void);
void loadply(void);
void dummy_ply(void);
void copy_poly(void);
void move(void);
void rotate(void);
void translate(void);
void inrange(void);
void orderpoly(void);
void flatten(int i);
};
