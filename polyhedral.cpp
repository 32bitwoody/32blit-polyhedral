#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>

#include "polyhedron.hpp"

#include "polyhedral.hpp"
#include "colour.hpp"

using namespace blit;


const uint16_t screen_width = 650;
const uint16_t screen_height = 480;
const uint16_t screen_cw = 325;
const uint16_t screen_ch = 240;
//Vec2 screen_off = Vec2(screen_cw, screen_ch);


const float_t view_angle=pi/4;
const float_t half_fov=tanf(view_angle);

bool buttonA=false;
bool moveObj=false;
bool buttonB=false;
bool addObj=false;

uint32_t ptime;

polyhedron binky;
std::vector<polyhedron> binkys;



/* setup */


void init() {
  set_screen_mode(ScreenMode::hires);

 
//binky.dummy_quad();
	 
//binky.dummy_ply();
//printf("Initalise polyhedron\n");
//binky.polyhedron();
//binkys.assign( 1, polyhedron() );
binkys.emplace_back();
for(int x = 0; x < binkys.size(); x++){
	printf("Binky %d ",x);
	binkys[x].copy_poly();
	binkys[x].ez=screen_width/(2*half_fov);
	binkys[x].pos=Vec3(20+x*20,60,600);

}
//binky2.init();
printf("Using dummy poly created by init\n");
printf("Create copy of model\n");
binky.copy_poly();
//binky2.copy_poly();
printf("Set Eye-Screen distance\n");
binky.ez=screen_width/(2*half_fov);
//binky2.ez=screen_width/(2*half_fov);

  printf("view angle %f\n",view_angle);
  printf("tan-fov/2 %f\n",half_fov);
  printf("eye to screen %d\n",binky.ez);
}

float angle = 45.0f;

float deg2rad(float a) {
  return a * (pi / 180.0f);
  }






void render(uint32_t time_ms) {
//		printf("Render\n");

  uint32_t ms_start = now();

  if (moveObj==false) {
	return;
  }
  moveObj=false;
  printf("Set pen and clear screen\n");
  screen.pen = Pen(20, 30, 40);
  screen.clear();
  printf("Screen cleared, ready to display\n");
  binky.display();
  for(int x = 0; x < binkys.size(); x++){
	printf("Binky %d ",x);

	binkys[x].display();
}


  uint32_t ms_end = now();

 // draw FPS meter
  screen.mask = nullptr;
  screen.pen = Pen(255, 0, 0);
  uint32_t fps;
  if((ms_end - ms_start)>0) {
    fps = 200 / (ms_end - ms_start);
} else {
    fps =10;
  }
  if (fps>10) { fps=10; }
  for (unsigned int i = 0; i < fps; i++) {
    screen.pen = Pen(i * 5, 255 - (i * 5), 0);
	screen.rectangle(Rect(i * 3 + 1, 238, 2, 2));
  }  
}
  

//    screen.text("angle: " + std::to_string(int(angle)), minimal_font, rect(0, 0, 160, 10));

void update(uint32_t time) {
//	printf("Update\n");
//	if((time-ptime)>1000) {
//	printf("%d-%d\n",ptime,time);
//	ptime=time;
//  angle=angle+72;
//  }
   if (pressed(Button::A) && buttonA==false) {
	printf("A pressed\n");
	buttonA=true;
	moveObj=true;
  }
   if (pressed(Button::B) && buttonB==false) {
	printf("B pressed\n");
	buttonB=true;
//	addObj=true;
  }
//     if (pressed(Button::X) && buttonX==false) {
//     if (pressed(Button::Y) && buttonX==false) {
//     if (pressed(Button::MENU) && buttonX==false) {
//     if (pressed(Button::HOME) && buttonX==false) {

//     if (pressed(Button::DPAD_LEFT) && buttonDL==false) {
//     if (pressed(Button::DPAD_RIGHT) && buttonDR==false) {
//     if (pressed(Button::DPAD_UP) && buttonDU==false) {
//     if (pressed(Button::DPAD_DOWN) && buttonDD==false) {


  if (pressed(Button::A)==false && buttonA==true && moveObj==false) {
    printf("Release: A\n");
    buttonA=false;
  }
  if (pressed(Button::B)==false && buttonB==true) {
    printf("Release: B\n");
    buttonB=false;
    binkys.emplace_back();
    int x = binkys.size()-1;
	printf("Binky %d ",x);
	binkys[x].copy_poly();
	binkys[x].ez=screen_width/(2*half_fov);
	binkys[x].pos=Vec3(20+x*20,40,800);

  }
  if (joystick.y < -0.1f || joystick.y > 0.1f ) {
	  binky.avl.x=joystick.y;
  }
  if (joystick.x < -0.1f || joystick.x > 0.1f ) {
	  binky.avl.y=joystick.x;
  }

//   joystick_history.emplace_back(Point(
 //       (SCREEN_WIDTH / 2) + blit::joystick.x * 30,
//        (SCREEN_HEIGHT / 2) + blit::joystick.y * 30
//    ));

//    if(joystick_history.size() > 256){
//        int trim = joystick_history.size() - 256;
//        joystick_history.erase(joystick_history.begin(), joystick_history.begin() + trim);
//    }

}
