#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>

#include "dial.hpp"

using namespace blit;

dial::dial(){
}

float dial::deg2rad(float a) {
  return a * (pi / 180.0f);
}


///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void dial::init(Point c,uint8_t d,uint8_t t) {
 dialcentre=c;
  dialsize=d;
  nhmulti=d*0.6366;
  namulti=1; //3.125;
  // 0 is a horizon type tracking pitch and roll
  // 1 is a compass type tracking heading
  // 2 is a meter type tracking a 
  type=t;
    //  blit::debugf("dial: init: centre %f,%f size %f\n",double(dialcentre.x),double(dialcentre.y),double(dialsize));

    pointer = {
    Vec2(d,0),
    Vec2(4, 2),
    Vec2(0, -6),
    Vec2(-4, 2),
    Vec2(-1*d,0),
    Vec2(0,-2)
  };
}
//  https://cal-linux.com/tutorials//vectors.html
  

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering. time is the 
// amount if milliseconds elapsed since the start of your game
// dial values given in height and angle, assuming 1 as fsd
void dial::render(uint32_t time) {
//  //  blit::debugf("dial: centre %f,%f size %f\n",double(dialcentre.x),double(dialcentre.y),double(dialsize));
//  //  blit::debugf("dial: angle %f-%f-1.571=%f\n",double(needleangle),double(needleheight.y));
  if(type==0) {  
    render_type0(time);
  }
  if(type==1) {  
    render_type1(time);
  }
  if(type==2) {  
    render_type2(time);
  }
}
/*--------------------------------------------------------------------*/
void dial::render_type0(uint32_t time) {
   screen.pen = Pen(0, 64, 0);
  screen.circle(dialcentre, dialsize);
//  screen.pen = Pen(0, 0, 0);
//  screen.circle(dialcentre, dialsize-1);
//  screen.pen = Pen(20, 30, 80);
//  screen.circle(dialcentre, dialsize-4);
//  screen.pen = Pen(20, 30,80);
   screen.pen = Pen(0, 0,255);
    for (uint_fast32_t p = 0; p < 5; p++) {
      screen.line(pointer[p]+dialcentre-needleheight, pointer[p+1]+dialcentre-needleheight);
    }
//  screen.line(pointer[1]+dialcentre-needleheight, pointer[2]+dialcentre-needleheight);
//  screen.line(pointer[2]+dialcentre-needleheight, pointer[3]+dialcentre-needleheight);
//  screen.line(pointer[3]+dialcentre-needleheight, pointer[4]+dialcentre-needleheight);
//  screen.line(pointer[4]+dialcentre-needleheight, pointer[5]+dialcentre-needleheight);
    screen.line(pointer[5]+dialcentre-needleheight, pointer[0]+dialcentre-needleheight);
//  screen.pen = Pen(0, 0, 0);
//  screen.circle(dialcentre, dialsize-1);
//    lastangle=needleangle;
}
/*--------------------------------------------------------------------*/
void dial::render_type1(uint32_t time) {
  screen.pen = Pen(0, 64, 0);
  screen.circle(dialcentre, dialsize);
  screen.pen = Pen(0, 0,255);


    screen.line(dialcentre-Vec2(needleheight.y,dialsize*0.7), dialcentre-Vec2(needleheight.y,dialsize*-0.7));

}

/*--------------------------------------------------------------------*/
void dial::render_type2(uint32_t time) {
  screen.pen = Pen(0, 64, 0);
  screen.rectangle(Rect(dialcentre.x-dialsize,dialcentre.y-dialsize,dialsize+dialsize,dialsize+dialsize));
  screen.pen = Pen(255, 255,255);
      screen.alpha = 64;

  screen.line(dialcentre-Vec2(dialsize,0),dialcentre-Vec2(dialsize-10,0));
    screen.line(dialcentre-Vec2(0,dialsize),dialcentre-Vec2(0,dialsize-10));
  screen.line(dialcentre+Vec2(dialsize,0),dialcentre+Vec2(dialsize-10,0));
  screen.line(dialcentre+Vec2(0,dialsize),dialcentre+Vec2(0,dialsize-10));

      screen.alpha = 255;

    screen.pen = Pen(0, 0,200);
uint32_t tt=1;

//  screen.circle(dialcentre,1);
  for (uint_fast32_t s = 0; s < FLEET; s++) {
    if(log[s].length()>2000) { continue; }
    if(log[s].x==0 && log[s].z==0) { continue; }
    if(fabsf(log[s].y)<100) { tt=3; }
    if(fabsf(log[s].y)<200) { tt=2; }
    screen.circle(dialcentre+Point(log[s].x/2000*dialsize,log[s].z/2000*-dialsize),tt);
    blit::debugf("dial%d: nh %f na %f\n",type,double(log[s].x),double(log[s].z));
  }
}
///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
void dial::update(uint32_t time,float na,float nh) {
  blit::debugf("dial: nh %f na %f\n",double(nh),double(na));

//  static float oldnh;
//float flips=abs(nh);
//if(nh>6.283184) { nh=nh-6.283184; }
//if(nh>4.712388) { nh=3.141592-nh; }
if(((nh>3.141592 && nh<6.283184) || ( nh<-3.141592 && nh>-6.283184)) && invert==false) {
  na=na+3.141592;
  invert=true;
}
  if(nh>1.570796) { 
    nh=3.141592-nh;
  } else if(nh<-1.570796) {
    nh=-3.141592-nh;
  }
  if(nh>1.570796) { 
    nh=3.141592-nh;
  } else if(nh<-1.570796) {
    nh=-3.141592-nh;
  }
  blit::debugf("dial: nh %f \n",double(nh));



  needleangle=na*namulti;
// needleheight.y=(nh-flips)*nhmulti;
  needleheight.y=(nh)*nhmulti;

//  if(needleheight.y>dialsize || needleheight.y<dialsize*-1){
//    nhmulti=nhmulti*-1;
//  }
  float tmp=needleangle-lastangle;
  tr1=Mat3::rotation(tmp);
//  tr1=Mat3::scale(Vec2(0.5,2));
//  tr1.inverse();
//      pointer *=  tr;
  for (auto &v : pointer) {
    v *= tr1;
  }
    lastangle=needleangle;

//  pointer[0].x=dialsize*cosf(nh);
//  pointer[2].x=dialsize*cosf(nh)*-1;
//  needleheight.x=dialsize*cosf(needleheight.y);
//  oldnh=nh;
// update 3D vectors  
//if (pressed(Button::MENU)) {
  //  blit::debugf("dial: angle %f\n",double(na));
    blit::debugf("dial: height %f %f %f\n",double(nh),double(needleheight.y),double(nhmulti));
//}
}

