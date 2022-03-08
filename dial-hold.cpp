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

void dial::draw_face() {
  screen.pen = Pen(20, 30, 40);
//  screen.clear();

  screen.pen = Pen(20, 30, 80);
  screen.circle(dialcentre, dialsize);
  screen.pen = Pen(0, 0, 0);
  screen.circle(dialcentre, dialsize-1);
  screen.pen = Pen(20, 30, 80);
  screen.circle(dialcentre, dialsize-4);
  screen.pen = Pen(20, 30,80);

//  for (uint8_t ta = 0; ta < 12; ta++) {
//    screen.circle(tickmark[ta],3);
//  }
//  for (uint8_t ta = 0; ta < 4; ta++) {
//    screen.pen = Pen(20, 30, 80);
//    screen.circle(screw[ta],16);
//    screen.pen = Pen(10, 14, 40);
//    screen.circle(screw[ta],14);
//    screen.pen = Pen(2, 3, 8);
//    screen.rectangle(Rect(screw[ta].x-16, screw[ta].y-2, 33, 4));
//  }
}

void dial::init(Point c, uint8_t d) {
  blit::set_screen_mode(blit::ScreenMode::hires);
  offset=Vec2(160,120);
  dialcentre=c;
  dialsize=d;
  draw_face();
  
}

void dial::render(uint32_t time_ms) {
  screen.pen = Pen(0, 0, 0);
  screen.circle(dialcentre, 109);
    
  screen.pen = Pen(20, 30, 40);
  screen.rectangle(Rect(100, 150, 50, 10));
  screen.pen = Pen(20, 60, 90);
  screen.text(std::to_string(time_ms/1000), minimal_font, Point(149, 151),true,TextAlign::right);
    
  screen.pen = Pen(20, 30, 40);
  screen.rectangle(Rect(170, 150, 50, 10));
  screen.pen = Pen(20, 60, 90);
  screen.text(std::to_string(elap_min)+":"+std::to_string(elap_ms/1000), minimal_font, Point(219, 151),true,TextAlign::right);  
    
  tr2=Mat3::rotation(deg2rad(angle_min-lastangle_min));
  tr2.inverse();
//      pointer *=  tr;
  for (auto &v : minhand) {
    v *= tr2;
  }  
    
  screen.pen = Pen(0, 0,255);
  screen.line(minhand[0]+offset, minhand[1]+offset);
  screen.line(minhand[1]+offset, minhand[2]+offset);
  screen.line(minhand[2]+offset, minhand[3]+offset);
  screen.line(minhand[3]+offset, minhand[0]+offset);
    
  tr1=Mat3::rotation(deg2rad(angle-lastangle));
  tr1.inverse();
//      pointer *=  tr;
  for (auto &v : pointer) {
    v *= tr1;
  }
  
  screen.pen = Pen(0, 0,255);
  screen.line(pointer[0]+offset, pointer[1]+offset);
  screen.line(pointer[1]+offset, pointer[2]+offset);
  screen.line(pointer[2]+offset, pointer[3]+offset);
  screen.line(pointer[3]+offset, pointer[0]+offset);

//  screen.pen = Pen(255, 255, 255);
//  screen.text("Sec: " + std::to_string(elap_ms/1000), minimal_font, Point(155, 120));
//  screen.text("Min: " + std::to_string(elap_min), minimal_font, Point(155, 130));
//  screen.text("Delta: " + std::to_string(deg2rad(angle-lastangle)), minimal_font, Point(155, 140));
  lastangle=angle;
  lastangle_min=angle_min;
}

void dial::update(uint32_t time_ms) {

}
