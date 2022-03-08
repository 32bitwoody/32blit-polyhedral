#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>

#include "fonts/round_large.h"
#include "polyhedral.hpp"

using namespace blit;
///////////////////////////////////////////////////////////////////////////
// To do
//
// clean polyhedron var
// clean vehicle
// add mtl code
// build space class to hold ship numbers and positions
// add dynamics files
// add dynamics load
// use dynamics
// add physicish code to push ships 
// build Me vehicle code for dials and control
// put control into button update
// global variables (ship limits)
// consider non clear method for redraw
// facility in vehicle to set pos and direction
//
///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup of game
//
void init() {
  set_screen_mode(ScreenMode::hires);
//  //  blit::debugf("main: init\n");

  view.screen_width = 320;
  view.screen_height = 240;
  view.screen_cw = 80;// 160;
  view.screen_ch = 60; //120;

  view.fov=pi/4;
  view.half_fov=tanf(view.fov/2);

//  //  blit::debugf("main: start polyhedron\n");
  viewang = Vec3(0,0,0);
  viewpos = Vec3(0,0,0);
//    for (uint32_t s = 0; s < 2; s++) {

//  testshape[s].init(view,3,s);
//  testshape[s].model_load();
//  testshape[s].objpos=Vec3(100-(s*80),0,800);
//  testshape[s].objang=Vec3(0,-0.12,0);
//}  
//  //  blit::debugf("main: start ship\n");
  count=4;
  if(count>FLEET) { count=FLEET; }
  for (int_fast32_t s = 0; s < count; s++) {

//  if(s<2) {
//    shipy[s].init(view,PYRAMID,s+10);
//} else {
    shipy[s].init(view,PYRAMID,s+10);

//}

  //0.1745
  //Vec3((s*20)-10,90,0)
    shipy[s].set(Vec3(-0.0873+s*0.0436,0,0),Vec3(-300+(s*200),90,3000));
    
    shipy[s].speed=0.75;
    shipy[s].count=count;
    for (uint_fast32_t t = 0; t < count; t++) {
      shipy[s].radar_exist[t]=true;
    }
  }
//  shipy[2].set(Vec3(0,1.5708,0),Vec3(200,00,1500));
//  shipy[2].speed=0.5;

//  shipy[1].set(Vec3(0,1.5708,0),Vec3(-600,0,1500));
//  shipy[1].speed=1.5;

  shipy[0].speed=1.5;
//  shipy[0].set(Vec3(0,3.141592,0),Vec3(0,0,2000));
//  shipy[0].set(Vec3(0,1.570796,0),Vec3(0,0,2000));
 //   shipy[0].set(Vec3(0,0,0),Vec3(0,0,-2000));
//    shipy[0].set(Vec3(0,1.570796,0),Vec3(0,0,2000));

//  shipy[0].set(Vec3(0,3.141592,0),Vec3(0,0,2000));
//  shipy[0].set(Vec3(3.141592,3.141592,3.141592),Vec3(0,0,2000));

//  shipy[0].set(Vec3(0,3.141592,0),Vec3(2000,0,200));

  
//  //  blit::debugf("main: start dials\n");
  heading.init(Point(290,69),30,1);
  pitchroll.init(Point(290,138),30,0);
  radardial.init(Point(290,205),30,2);

  dH=0;
  dA=0;
  speed=0;
}
///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of the game
//
void render(uint32_t time) {
  render_ms_start = now();
  screen.pen = Pen(20, 30, 40);
  screen.clear();
//  blit::debugf("main: Render start %d s\n",ms_start);
  screen.alpha = 255;

  for (uint_fast32_t z = 2000; z > 0; z=z-50) {
    for (uint_fast32_t s = 0; s < count; s++) {
//  blit::debugf("main: Render%d %d %d\n",s,shipy[s].rendered,shipy[s].z);

      if(shipy[s].rendered==false && shipy[s].z>z) {
        shipy[s].render(time);
      }
    }
  }
    screen.pen = Pen(0, 64, 0);
  screen.rectangle(Rect(260, 2 , 60, 30));
    screen.pen = Pen(0, 0,255);

  std::string fms = std::to_string(score);
//  screen.text(fms, minimal_font, Rect(260, 4, 60, 10));
//  blit::screen.text(prefixedScore, hud_font_small, blit::Point(49 + shadowOffset, 18 + shadowOffset), true, blit::TextAlign::left);

//  screen.text(fms, outline_font, Rect(260, 4, 60, 10), true, TextAlign::center_right);
  screen.text(fms, round_large, Rect(260, 4, 60, 30), true, TextAlign::center_center);


  heading.render(time);
  pitchroll.render(time);
  radardial.render(time);
  
  render_crosshair();
  render_indicators();

  render_laser();

//  update_fps(time);
  render_fps(time);

    }
/*--------------------------------------------------------------------*/
void render_laser() {
  if(fire_laser==false || laser_temp>100) { return; }
    screen.alpha = 128;
    screen.pen = Pen(20,20,200);
    screen.line(Point(0,240),Point(160,120));
    screen.line(Point(1,240),Point(161,120));

    screen.line(Point(320,240),Point(160,120));
    screen.line(Point(319,240),Point(159,120));


  }
/*--------------------------------------------------------------------*/
void render_crosshair() {
    screen.alpha = 128;
    screen.pen = Pen(20,200,20);
    screen.line(Point(160,68),Point(160,108));
    screen.line(Point(160,132),Point(160,172));
    screen.line(Point(92,120),Point(148,120));
    screen.line(Point(172,120),Point(228,120));
  }
/*--------------------------------------------------------------------*/
void render_indicators() {
    screen.alpha = 255;
    if(shipy[0].adj==true) {
      screen.circle(Point(194,118),2);
    }
    screen.pen = Pen(0,0,200);
    screen.line(Point(161,132+(3-speed)*13),Point(161,172));
    screen.line(Point(162,132+(3-speed)*13),Point(162,172));

    screen.pen = Pen(200,0,0);
    screen.line(Point(159,132+30-(laser_temp*0.2)),Point(159,172));
    screen.line(Point(158,132+30-(laser_temp*0.2)),Point(158,172));

    if(fire_laser==false || laser_indic==0) { return; }
    blit::debugf("main: laser indic: %d \n",laser_indic);

    screen.pen = Pen(200,100,0);
//    for(uint8_t n=1; n>2; n++) { //int(laser_indic); n--) {
 //     screen.circle(Point(160,120),20);
 drawcircle(Point(160,120),40);
//}
//    laser_indic

}
/*--------------------------------------------------------------------*/
void render_fps(uint32_t time) {
  // draw FPS meter
  render_ms_end = now();
//    blit::debugf("main: Render end %d s\n",ms_end);

  uint8_t block_size = 4;
  uint8_t o=1;
  if((render_ms_end - render_ms_start)>0) {
    rfps = 1000 / (render_ms_end - render_ms_start);
    blit::debugf("main: Render %d fps\n",rfps);

  } else {
    rfps =10;
  }
  
  screen.alpha = 128;
  if (rfps>30) { rfps=30; }

  for (uint32_t i = 0; i < rfps; i++) {
    screen.pen = Pen(i * 5, 255 - (i * 5), 0);
    screen.rectangle(Rect(i * (block_size + 1) + o+140, screen.bounds.h - block_size - 1, block_size, block_size));
    if(i>5) {o=2;}
    if(i>10) {o=3;}
    if(i>15) {o=4;}
    if(i>20) {o=5;}
    if(i>25) {o=6;}
    if(i>30) {o=7;}
  } 
}
/*--------------------------------------------------------------------*/
void update_fps(uint32_t time) {
  // draw FPS meter
//  uint32_t update_ms_end = now();
//    blit::debugf("main: Render end %d s\n",ms_end);

  uint8_t block_size = 4;
  uint8_t o=1;

  
  screen.alpha = 128;
  if (ufps>30) { ufps=30; }

  for (uint32_t i = 0; i < ufps; i++) {
    screen.pen = Pen(i * 5, 255 - (i * 5), 0);
    screen.rectangle(Rect(i * (block_size + 1) + o+140, screen.bounds.h - block_size - block_size - 1, block_size, block_size));
    if(i>5) {o=2;}
    if(i>10) {o=3;}
    if(i>15) {o=4;}
    if(i>20) {o=5;}
    if(i>25) {o=6;}
    if(i>30) {o=7;}
  } 
}
///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of the game
//
void update(uint32_t time) {
//    update_ms_start = now();

  blit::debugf("main: update:\n");

//  //  blit::debugf("main: viewvec: %f\n",double(viewvec.z));

  update_button(time);
  vector_fromangle();
// let everyone sort out where they are  
//  //  blit::debugf("main: viewvec: %f\n",double(viewvec.z));
  for (uint_fast32_t s = 0; s < count; s++) {
//    testshape[s].update(viewang,viewpos,viewvec);

    shipy[s].update(time,viewang,viewpos,viewvec);
    working_z[s]=shipy[s].z;
    working_s[s]=s;
  }
  update_space(time);
  if(shipy[0].adj==false) {
    pitchroll.update(time,viewang.z,viewang.x);
    heading.update(time,0,viewang.y);
  } else {
    pitchroll.update(time,shipy[0].binky.objang.z,shipy[0].binky.objang.x);
    heading.update(time,0,shipy[0].binky.objang.y);
  } 
//    pitchroll.update(time,shipy.viewvec.z,shipy.viewvec.y);
//  heading.update(time,shipy.viewvec.x,0);

  update_laser(time);
  
  update_ms_end = now();
  if((update_ms_end - update_ms_start)>0) {
    ufps = (update_ms_end - update_ms_start);
    blit::debugf("main: Update %d fps %d-%d\n",ufps,update_ms_end,update_ms_start);
  } else {
    ufps =10;
  }
  update_ms_start = now();

}
/*--------------------------------------------------------------------*/
void update_space(uint32_t time) {
  // decide what order to render and who has crashed 
  uint32_t tmp_z,tmp_s,tmp_fuz;
  Vec3 shipy_pos;
//  laser_indic=0;
  for (uint_fast32_t s = 0; s < count; s++) {
//    blit::debugf("s:%d ",s);
    for (uint_fast32_t t = 0; t < count; t++) {
//      blit::debugf("t:%d ",t);
//decide render order
      if(t<count-1) {
      if(working_z[t]<working_z[t+1]) {
        tmp_z=working_z[t];
        tmp_s=working_s[t];
        working_z[t]=working_z[t+1];
        working_s[t]=working_s[t+1];
        working_z[t+1]=tmp_z;
        working_s[t+1]=tmp_s;
      }
    }
      if(s==t) {
              blit::debugf("main: same ship ignore %d %d\n",s,t);

        shipy[s].radar[t]=Vec3(0,0,0);
 //       blit::debugf("s=t continue ");
        continue; 
      }
      if(shipy[s].exist==false) { continue; }
      if(shipy[t].exist==false) { continue; }
//check location against other objects
      space_vec[s]=shipy[s].position()-shipy[t].position();
      space_distance=space_vec[s].length();
      blit::debugf("main: ranging: %d %d %f\n",s,t,double(space_distance));

      if(space_distance<90) {
        shipy[s].ploded();
        shipy[t].ploded();
        for (uint32_t u = 0; u < count; u++) {

        shipy[u].radar[t]=Vec3(0,0,0);
        shipy[u].radar[s]=Vec3(0,0,0);

        shipy[u].radar_exist[t]=false;
        shipy[u].radar_exist[s]=false;
      }
      blit::debugf("main: crash: %d %d %f\n",s,t,double(space_distance));

      } else {
        tmp_fuz=0; //rand() % int(space_distance*0.1);
        shipy_pos=shipy[t].position();
        shipy[s].radar[t]=shipy_pos+Vec3(tmp_fuz,tmp_fuz,tmp_fuz);
        shipy[s].radar_d[t]=space_distance+tmp_fuz;

        blit::debugf("main: actual: %d %d %f,%f,%f\n",s,t,double(shipy_pos.x),double(shipy_pos.y),double(shipy_pos.z));
        blit::debugf("main: radar: %d %d %f,%f,%f\n",s,t,double(shipy[s].radar[t].x),double(shipy[s].radar[t].y),double(shipy[s].radar[t].z));
      }
    }
    if(shipy[s].exist==true) {
      space_vec[s]=shipy[s].position()-viewvec;
      space_pos[s]=shipy[s].position()-viewpos;
      radardial.log[s]=space_vec[s];
    } else {
      radardial.log[s]=Vec3(0,0,0);
    }
// check ship is in line, laser on and in range
// need viewport position of shipy[s]
    if(fire_laser==true && laser_temp<100) {
      space_distance=space_vec[s].length();
      space_vec[s].normalize();
      Vec2 shipy_pos=shipy[s].position_view();
//      blit::debugf("main: laser: %d %f,%f,%f\n",s,double(shipy[s].binky.objpos.x),double(shipy[s].binky.objpos.y),double(shipy[s].binky.objpos.z));
//      blit::debugf("main: laser: %d %f,%f,%f\n",s,double(viewvec.x),double(viewvec.y),double(viewvec.z));
      blit::debugf("main: laser: %d %f,%f \n",s,double(shipy_pos.x),double(shipy_pos.y));
      if(shipy_pos.x>laser_aim && 
      shipy_pos.x<(laser_aim*2) &&
            shipy_pos.y>laser_aim &&
      shipy_pos.y<(laser_aim*2) &&
      (space_pos[s].length())<laser_range
      
      ) {
        blit::debugf("main: laser hit: %d \n",s);
        laser_indic=shipy[s].laser_hit((laser_range-space_vec[s].z)*0.01);
       // laser_indic=(laser_range-space_vec[s].z)*0.01;
       if(laser_indic>0) {score++; }
      }
    } 
  }
}
/*--------------------------------------------------------------------*/
void update_button(uint32_t time) {
  
  if (pressed(Button::A) ) { //&& buttonA==false) {
    buttonA=true;
     fire_laser=true;
//    shipy[0].speed=shipy[0].speed+0.1;
//    if(shipy[0].speed>0.7) {
//      shipy[0].speed=0.7;
//    }
  }
//.....................................................................  
  if (pressed(Button::A)==false ) { //&& buttonA==true) {
    buttonA=false;
    fire_laser=false;
  }
  if (pressed(Button::B) && buttonB==false) {
   	buttonB=true;
  }  
  if (pressed(Button::B)==false && buttonB==true) {
    buttonB=false;
  }
  stick_value=-0.003;
  if (pressed(Button::X)) {
    stick_value=-0.001;
  } 
  if (pressed(Button::X) && buttonX==false) {
	buttonX=true;
  dH=0;
//    shipy.adjustpos(Vec3(0,10,0));
  }
  if (pressed(Button::X)==false && buttonX==true) {
    buttonX=false;
  }
  if (pressed(Button::Y) && buttonY==false) {
	  buttonY=true;
//    shipy.adjustpos(Vec3(0,-10,0));
  }
  if (pressed(Button::Y)==false && buttonY==true) {
    //  //  blit::debugf("main: Release: Y\n");
    buttonY=false;
  }
//.....................................................................  
  if (joystick.y > -0.05f || joystick.y < 0.05f ) {

  }

  if (joystick.y < -0.001f || joystick.y > 0.001f ) {
    if(shipy[0].adj==false) {
      viewang=viewang+Vec3(stick_value*joystick.y,0,0);
    }
    if(shipy[0].adj==true) {
      shipy[0].adjustang(Vec3(stick_value*joystick.y,0,0));
    }
  }
  if (joystick.x > -0.05f || joystick.x < 0.05f ) {
  }
  if (joystick.x < -0.001f || joystick.x > 0.001f ) {
    if(shipy[0].adj==false) {
      viewang=viewang+Vec3(0,stick_value*joystick.x,stick_value*joystick.x);
    }
    if(shipy[0].adj==true) {
      shipy[0].adjustang(Vec3(0,stick_value*joystick.x,0));
    }
  }

//.....................................................................  
  if (pressed(Button::DPAD_LEFT) && pressed(Button::X)==false) { // && dpadL==false) {
    dpadL=true;
    if(shipy[0].adj==false) {
      viewang=viewang+Vec3(0,-0.01,0);
    }
    if(shipy[0].adj==true) {
//    shipy[0].adjustang(Vec3(0,0,-0.01));
      shipy[0].adjustang(Vec3(0,-0.01,0));
//    dA=dA+0.1;
//        testshape[0].objang.z=testshape[0].objang.z + 0.3142;
    }
  }
  if (pressed(Button::DPAD_LEFT)==false ) { //&& dpadL==true) {
    dpadL=false;
        
  }
//  if (pressed(Button::DPAD_LEFT) && pressed(Button::X)) {
//    if(shipy[0].adj==true) {
//      shipy[0].adjustang(Vec3(0,0,-0.01));
//    }
  //}
//.....................................................................  
  if (pressed(Button::DPAD_RIGHT) && pressed(Button::X)==false) { //&& dpadR==false) {
    dpadR=true;
    if(shipy[0].adj==false) {
      viewang=viewang+Vec3(0,0.01,0);
    }
    if(shipy[0].adj==true) {
//    shipy[0].adjustang(Vec3(0,0,0.01));
      shipy[0].adjustang(Vec3(0,0.01,0));
//    dA=dA-0.1;
//            testshape[0].objang.z=testshape[0].objang.z - 0.3142;
    }
  }
  if (pressed(Button::DPAD_RIGHT)==false && dpadR==true) {
    dpadR=false;
  }
  if (pressed(Button::DPAD_RIGHT) && pressed(Button::X)) { 
    shipy[0].adjustang(Vec3(0,0,0.005));
  }
//.....................................................................  
  if (pressed(Button::DPAD_UP) ) { //&& dpadU==false) {
    dpadU=true;
//    dH=dH+0.2;
//    shipy[0].adjustang(Vec3(-0.01,0,0));
    if(shipy[0].adj==true) {
      shipy[0].speed=shipy[0].speed+0.1;
      if(shipy[0].speed>3) {
        shipy[0].speed=0;
      }
    } else {
      speed=speed+0.1;
      if(speed>3) {
        speed=3;    
      }
    }
//    testshape[0].objang.x=testshape[0].objang.x - 0.3142;
  }
  if (pressed(Button::DPAD_UP)==false ) { //&& dpadU==true) {
    dpadU=false;
  }
//.....................................................................  
  if (pressed(Button::DPAD_DOWN) ) { //&& dpadD==false) {
    dpadD=true;
    if(shipy[0].adj==true) {
 //     shipy[0].adjustang(Vec3(0.01,0,0));
      shipy[0].speed=shipy[0].speed-0.1;
      if(shipy[0].speed<0) {
        shipy[0].speed=0;
      }
    } else {
      speed=speed-0.1;
      if(speed<0) {
        speed=0;    
      }
    }
  }
  if (pressed(Button::DPAD_DOWN)==false ) { //&& dpadD==true) {
    dpadD=false;
  }
  blit::debugf("main: Speed %f \n",double(speed));
//.....................................................................  
  if (pressed(Button::MENU) && buttonMENU==false) {
	  buttonMENU=true;
    if(shipy[0].adj==true) {
      shipy[0].adj=false;
    } else {
      shipy[0].adj=true;
    }
//    shipy.adjustpos(Vec3(0,10,0));
  }
  if (pressed(Button::MENU)==false && buttonMENU==true) {
    buttonMENU=false;
  }
  }
/*--------------------------------------------------------------------*/
void update_laser(uint32_t time) {
  if(fire_laser==true && laser_temp<141) { laser_temp++; }
  if(fire_laser==false && laser_temp>0) { laser_temp--; }
    
}
/*--------------------------------------------------------------------*/
void vector_fromangle() {
//  Vec3 co,so;
//  blit::debugf("main: Object Angle x %f:y %f:z %f \n",double(viewang.x),double(viewang.y),double(viewang.z));
//  blit::debugf("polyhedron: View Angle x %f:y %f:z %f \n",double(viewang.x),double(viewang.y),double(viewang.z));
//  viewvec.x=sinf(viewang.y)*cosf(viewang.z);
//  viewvec.y=sinf(viewang.x)*cosf(viewang.z);
//  viewvec.z=cosf(viewang.x)*cosf(viewang.y);
  
    viewvec.x=sinf(viewang.y)*cosf(viewang.x);
    viewvec.y=sinf(viewang.x); 
    viewvec.z=cosf(viewang.x)*cosf(viewang.y);
//  blit::debugf("main: Vector x %f:y %f:z %f \n",double(viewvec.x),double(viewvec.y),double(viewvec.z));
}
/*--------------------------------------------------------------------*/

void drawcircle(Point centre, uint_fast16_t radius) {
  
 // screen.circle(Point(160,120),20);
    if(centre.x > view.screen_width || centre.y >= view.screen_height) {
        //DEBUG("GUI_DrawCircle Input exceeds the normal display range\r\n");
        return;
    }

    //Draw a circle from(0, R) as a starting point
    int16_t XCurrent, YCurrent;
    XCurrent = 0;
    YCurrent = radius;

    //Cumulative error,judge the next point of the logo
    int16_t Esp = 3 - (radius << 1 );

    int16_t sCountY;
 //Draw a hollow circle
        while(XCurrent <= YCurrent ) {
          screen.pixel(centre + Point(XCurrent, YCurrent));
            //GUI_DrawPoint(X_Center + XCurrent, Y_Center + YCurrent, Color, Dot_Pixel, DOT_STYLE_DFT );//1
            screen.pixel(centre + Point( - XCurrent,  + YCurrent)); //, Color, Dot_Pixel, DOT_STYLE_DFT );//2
            screen.pixel(centre + Point( - YCurrent,  + XCurrent)); //, Color, Dot_Pixel, DOT_STYLE_DFT );//3
            screen.pixel(centre + Point( - YCurrent,  - XCurrent)); //, Color, Dot_Pixel, DOT_STYLE_DFT );//4
            screen.pixel(centre + Point( - XCurrent,  - YCurrent)); //, Color, Dot_Pixel, DOT_STYLE_DFT );//5
            screen.pixel(centre + Point( + XCurrent,  - YCurrent)); //, Color, Dot_Pixel, DOT_STYLE_DFT );//6
            screen.pixel(centre + Point( + YCurrent,  - XCurrent)); //, Color, Dot_Pixel, DOT_STYLE_DFT );//7
            screen.pixel(centre + Point( + YCurrent,  + XCurrent)); //, Color, Dot_Pixel, DOT_STYLE_DFT );//0

            if(Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent );
                YCurrent --;
            }
            XCurrent ++;
        }
}
