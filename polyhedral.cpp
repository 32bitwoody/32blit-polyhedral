#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>

#include "polyhedral.hpp"
#include "colour.hpp"

using namespace blit;

const uint16_t screen_width = 650;
const uint16_t screen_height = 480;
const uint16_t screen_cw = 325;
const uint16_t screen_ch = 240;
//Vec2 screen_off = Vec2(screen_cw, screen_ch);
Vec2 screen_off = Vec2(100, 100);


const float_t view_angle=pi/4;
const float_t half_fov=tanf(view_angle);
const uint16_t ez=screen_width/(2*half_fov);

bool buttonA=false;
bool moveObj=false;

std::array<Vec2, 3> vecs;
std::array<Vec3, 4> model_pt;
std::array<Vec2, 3> nstnc_flt;
std::array<Vec3, 4> model_fc;
std::array<Pen, 4> model_col;


std::array<Vec3, 4> nstnc;
Vec3 pos;
std::array<Vec3, 4> pstnc;
//Vec3 rot;  //Not required
Vec3 vel;
std::array<Vec3, 2> vlimit;
Vec3 avl;
std::array<Vec3, 2> avlimit;
Vec3 acc;
std::array<Vec3, 2> alimit;
Vec3 rac;
std::array<Vec3, 2> ralimit;

std::array<Vec3, 3> face;

uint32_t ptime;
/* setup */
Vec3 rotate3(Vec3 v,Vec3 angle) {
  Vec3 c,s;
  c.x = cosf(angle.x);
  s.x = sinf(angle.x);
  c.y = cosf(angle.y);
  s.y = sinf(angle.y);
  c.z = cosf(angle.z);
  s.z = sinf(angle.z);
//  printf("x %f:y %f:z %f",v.x,v.y,v.z);
Vec3 t(0,0,0);
Vec3 t2(0,0,0);

//  t.x = v.x * c.z - v.y * s.z;
//  t.y = v.x * s.z + v.y * c.z;
   
//   t.z = v.z * c.x - t.y * s.x;
//   t.y = v.z * s.x + t.y * c.x;  
t2.x=v.x;
t2.z=v.z;
t.y=v.y;   
   t.x = t2.x * c.y - t2.z * s.y;
   t.z = t2.x * s.y + t2.z * c.y;

return t;
}
Vec3 translate3(Vec3 v,Vec3 move) {
Vec3 t(0,0,0);

  t.x = v.x + move.x;
  t.y = v.y + move.y;
  t.z = v.z + move.z;

return t;
}
void init() {
  set_screen_mode(ScreenMode::hires);
  printf("view angle %f\n",view_angle);
  printf("tan-fov/2 %f\n",half_fov);
  printf("eye to screen %d\n",ez);
 
  model_pt = {
    Vec3( 30,   0, 30),
    Vec3(-30,   0, 30),
    Vec3(  0,  30,  0),
    Vec3(  0,   0, -30)
  };
  model_fc = {
	  Vec3(0,1,2),
	  Vec3(0,1,3),
	  Vec3(0,2,3),
	  Vec3(1,2,3)
};
  model_col = {
	  Pen(0,0,120,255),
	  Pen(0,0,160,255),
	  Pen(0,0,200,255),
	  Pen(0,0,240,255)
};
  avl=Vec3(0,pi/2,0);
  pos=Vec3(20,20,320);
  int cx=sizeof(model_pt)/sizeof(model_pt[0]);
  for (int i=0; i<cx; i++) {
    nstnc[i].x=model_pt[i].x;
	nstnc[i].y=model_pt[i].y;
	nstnc[i].z=model_pt[i].z;
  }  
}


float angle = 45.0f;

float deg2rad(float a) {
  return a * (pi / 180.0f);
  }

Vec2 flatten(Vec3 v) {

Vec2 t(0,0);
t.x=v.x * ez / v.z;
t.y=v.y * ez /v.z;
return t;
}


void drawtri(std::array<Vec2, 3> face,Pen facecol) {
//  Pen p(facecol.r, facecol.g, facecol.b, facecol.alpha);
// p=(0,255,0,255);


  screen.pen = facecol;
  
  screen.triangle(face[0]+screen_off,face[1]+screen_off,face[2]+screen_off);
//  screen.triangle(vecs[0]+screen_off,vecs[1]+screen_off,vecs[2]+screen_off);
}

void render(uint32_t time_ms) {
	if (moveObj==false) {
		return;
	}
	moveObj=false;
  screen.pen = Pen(20, 30, 40);
  screen.clear();
 
  int cx=sizeof(model_pt)/sizeof(model_pt[0]);
  printf("cx %d\n",cx);
  for (int i=0; i<cx; i++) {
	printf("%d ins x %f:y %f:z %f ",i,nstnc[i].x,nstnc[i].y,nstnc[i].z);
    nstnc[i]=rotate3(nstnc[i],avl);
    printf("rot x %f:y %f:z %f ",nstnc[i].x,nstnc[i].y,nstnc[i].z);
    pstnc[i]=translate3(nstnc[i],pos);
    printf("mov x %f:y %f:z %f \n",pstnc[i].x,pstnc[i].y,pstnc[i].z);
    
    printf("Rot x %f:y %f:z %f ",avl.x,avl.y,avl.z);
    printf("Mov x %f:y %f:z %f\n",pos.x,pos.y,pos.z);
  }
  printf("--------\n");
  std::array<Vec3, 3> face;
//    loop through faces and flatten and draw;
//model_fc needs to be array of array
  int col=128;
  int i=0;
  for (auto &fcpt : model_fc) {
	printf("pt1 %f:pt2 %f:pt3 %f\n",fcpt.x,fcpt.y,fcpt.z);

//	  for (int j=0; j<3; j++) {
//	    Vec3 n=fcpt;
    std::array<Vec3, 3> face;
//	    face[j]=pstnc[n.x];
	face[0]=pstnc[fcpt.x];
    face[1]=pstnc[fcpt.y];
    face[2]=pstnc[fcpt.z];
	  //   at some point we will want to look at z and decide which to draw first 
	printf("0: x %f:y %f:z %f ",face[0].x,face[0].y,face[0].z);
	printf("1: x %f:y %f:z %f ",face[1].x,face[1].y,face[1].z);
	printf("2: x %f:y %f:z %f\n",face[1].x,face[2].y,face[2].z);
    nstnc_flt[0]=flatten(face[0]);
    nstnc_flt[1]=flatten(face[1]);
    nstnc_flt[2]=flatten(face[2]);
    printf("0: x %f:y %f ",nstnc_flt[0].x,nstnc_flt[0].y);
	printf("1: x %f:y %f ",nstnc_flt[1].x,nstnc_flt[1].y);
	printf("2: x %f:y %f\n",nstnc_flt[1].x,nstnc_flt[2].y);
  
  drawtri(nstnc_flt,model_col[i]);
  col=col+20;
  i++;
}
}
  

//    screen.text("angle: " + std::to_string(int(angle)), minimal_font, rect(0, 0, 160, 10));

void update(uint32_t time) {
//	if((time-ptime)>1000) {
//	printf("%d-%d\n",ptime,time);
//	ptime=time;
//  angle=angle+72;
//  }
   if (pressed(Button::A) && buttonA==false) {
	buttonA=true;
	moveObj=true;
  }
//On release place new piece
  if (pressed(Button::A)==false && buttonA==true && moveObj==false) {
    printf("Release: A\n");
    buttonA=false;
  }
}
