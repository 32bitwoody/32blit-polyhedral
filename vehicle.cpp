#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>

#include "vehicle.hpp"

using namespace blit;

vehicle::vehicle(){
  serial=rand();
  printf("vehicle: Instance %ld\n",serial); 
}
///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void vehicle::init(ViewPyramid &view,uint8_t v_type,uint8_t id) {
  //  blit::debugf("vehicle%d: Init %d\n",id,serial); 
  binky.init(view,id,serial);
//  binky.copy_poly();
//  printf("Set Eye-Screen distance\n");
//  binky.ez=screen_width/(2*half_fov);
//  binky.screen_width=screen_width;
//  binky.screen_height=screen_height;
  hitpoints=3000;

  if(v_type==0){
    binky.model_load((char*)pyramid_obj);
  }
  if(v_type==1){
    binky.model_load((char*)ship2t_obj);
}
if(v_type==2){
  binky.model_load((char*)wedge_obj);
}
if(v_type==3){
  binky.model_load((char*)tetra_obj);
}
//  //  blit::debugf("vehicle%d: Init: Obj angle %f,%f,%f\n",double(binky.objang.x),double(binky.objang.y),double(binky.objang.z));
//  binky.objang=Vec3(-0.393,3.14159,0);
    binky.objang=Vec3(0.1745,0.1,0);
    binky.objpos=Vec3(0,0,600);
//    binky.objang=Vec3(2.749,3.142,0);
  polyid=id;
    forward=true;
    rotate=false;
    accelerate=false;
    safe_approach=0.4;

}
/*--------------------------------------------------------------------*/
void vehicle::set(Vec3 objang, Vec3 objpos) {
  binky.objang=objang;
  binky.objpos=objpos;
    //  //  blit::debugf("vehicle%d: Set: Obj angle %f,%f,%f\n",polyid,double(binky.objang.x),double(binky.objang.y),double(binky.objang.z));
//      //  blit::debugf("vehicle%d: Set: Obj pos %f,%f,%f\n",polyid,double(objpos.x),double(objpos.y),double(objpos.z));
  //  blit::debugf("vehicle%d: binky: Obj pos %f,%f,%f\n",polyid,double(binky.objpos.x),double(binky.objpos.y),double(binky.objpos.z));
}
///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
void vehicle::render(uint32_t time) {
  if(exist==false) { 
//    blit::debugf("vehicle%d: exist=false \n ",polyid);

    return; 
  }
    blit::debugf("vehicle%d: render: \n ",polyid);
//screen.pen = Pen(255, 0,0);
//	screen.rectangle(Rect(180, 140, 10, 10));
//  bool showdata=false;
  binky.render(true);
    rendered=true;
}
///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
void vehicle::update(uint32_t time,Vec3 viewang,Vec3 viewpos,Vec3 viewvec) {
    if(exist==false) { return; }

  rendered=false;
  // radar review
  update_radar();

  
  //  blit::debugf("vehicle%d: Update: \n ",polyid);
// update 3D vectors  
//  //  blit::debugf("vehicle%d: Update: Obj angle %f,%f,%f\n",polyid,double(binky.objang.x),double(binky.objang.y),double(binky.objang.z));
//  blit::debugf("vehicle%d: Update: Obj pos %f,%f,%f\n",polyid,double(binky.objpos.x),double(binky.objpos.y),double(binky.objpos.z));
//  //  blit::debugf("vehicle%d: Update: View pos %f,%f,%f\n",polyid,double(viewpos.x),double(viewpos.y),double(viewpos.z));
  update_move(time,viewpos);
  binky.update(viewang,viewpos,viewvec);
  z=binky.viewobjpos.z;
  last_update=time;
}
/*--------------------------------------------------------------------*/
void vehicle::update_move(uint32_t time,Vec3 viewpos) {

  // are we on a collision approach, if so carry out evasion
  update_move_evade();

// review heading against other vehicles
  update_velocity();
 
  viewer_distance=binky.objpos-viewpos;
  view_dist_lgth=viewer_distance.length();
//  update_move_toward(viewpos);
  
  //  blit::debugf("vehicle%d: Distance %f %d\n",polyid,double(viewer_distance.length()),turnflag);
  if(view_dist_lgth>1500 &&     move_toward==false) { // turnflag!=true){
    //  blit::debugf("vehicle%d: Distance - far enough!\n",polyid);
    move_toward=true;
    move_t=0;
//    velocity=Vec3(0,0,0);
//    if(binky.objang.y>=0) {
//      binky.objang.y=binky.objang.y - 3.142;
//      binky.objang.x=0-binky.objang.x;
//    } else {
//      binky.objang.y=binky.objang.y + 3.142;
//      binky.objang.x=0-binky.objang.x;
    }
//    if(binky.objang.x>=0) {
//      binky.objang.x=binky.objang.x - 3.142;
//    } else {
//      binky.objang.x=binky.objang.x + 3.142;
//    }
//    turnflag=true;
    //  blit::debugf("vehicle%d: Move: Obj angle %f,%f,%f\n",polyid,double(binky.objang.x),double(binky.objang.y),double(binky.objang.z));
//  }
//  if(view_dist_lgth<800 && turnflag==true){
//    turnflag=false;
//  }
 last_move=time;
}
/*--------------------------------------------------------------------*/
void vehicle::update_move_evade() {
  switch(evasion) {
  case 0: 
    return;
    break;
  case 1: // +ve push down
    binky.objang=binky.objang+Vec3(0.01,0,0.01); 
    break;
  case 2:
    binky.objang=binky.objang+Vec3(0.01,0,0.01);
    break;
  case 3: // -ve x roll up
    binky.objang=binky.objang+Vec3(-0.01,0.01,0);
    break;
  case 4:
    binky.objang=binky.objang+Vec3(-0.01,0.01,0);
    break;
  default:
    break;
  }
  }
/*--------------------------------------------------------------------*/
void vehicle::update_move_toward(Vec3 viewpos) {
  if(adj==true) { return; }
  if(move_toward==false) { return; }

//    if(now()-last_move<100) { return; }

  heading=binky.objpos-viewpos;
  float heading_length=heading.length();
  if(heading_length>260 && heading_length<347) { 
    move_toward=false;
    return;
  }
  heading.normalize();
 
  blit::debugf("vehicle%d: navigation pos %f,%f,%f\n",polyid,double(binky.objpos.x),double(binky.objpos.y),double(binky.objpos.z));
  blit::debugf("vehicle%d: navigation ang %f,%f,%f\n",polyid,double(binky.objang.x),double(binky.objang.y),double(binky.objang.z));
  blit::debugf("vehicle%d: navigation vec %f,%f,%f\n",polyid,double(binky.objvec.x),double(binky.objvec.y),double(binky.objvec.z));
  blit::debugf("vehicle%d: navigation length %f\n",polyid,double(heading_length));
  blit::debugf("vehicle%d: navigation head %f,%f,%f\n",polyid,double(heading.x),double(heading.y),double(heading.z));
//  update_move_aim(viewpos,heading_length);
  update_move_pass(viewpos,heading_length);
}
/*--------------------------------------------------------------------*/
void vehicle::update_move_aim(Vec3 viewpos,float_t heading_length) {
//big turns first based on diff objvec to heading
  if(heading_length<1000) { return; }
  
  move_t=move_t+1;
  Vec3 aim=binky.objvec-heading;
  blit::debugf("vehicle%d: navigation aim %d: %f,%f,%f\n",polyid,move_t,double(aim.x),double(aim.y),double(aim.z));

  course_correct.y=0.02*(0-fabsf(aim.x))+0.02*(0-fabsf(aim.z));
  course_correct.z=0.20*(0-aim.y);
  course_correct.y=fabs(course_correct.y);
  if(course_correct.y>0.02)  { course_correct.y=0.02; }
  if(course_correct.y<-0.02) { course_correct.y=-0.02; }
  if(course_correct.z>0.02)  { course_correct.z=0.02; }
  if(course_correct.z<-0.02) { course_correct.z=-0.02; }
  if(course_correct.x>0.02)  { course_correct.x=0.02; }
  if(course_correct.x<-0.02) { course_correct.x=-0.02; }
//    blit::debugf("vehicle%d: navigation aiming %f,%f,%f\n",polyid,double(course_new.x),double(course_new.y),double(course_new.z));
  blit::debugf("vehicle%d: navigation correction %f,%f,%f\n",polyid,double(course_correct.x),double(course_correct.y),double(course_correct.z));
  adjustang(course_correct);
}
/*--------------------------------------------------------------------*/
void vehicle::update_move_pass(Vec3 viewpos,float_t heading_length) {
//    if(heading_length>1000) { return; }
  Vec3 course_new;

//  Vec3 pred_flight=binky.objpos-binky.objvec*heading_length;
  Vec3 pred_flight=binky.objvec*heading_length-binky.objpos;
//  float final_length=pred_flight.length();
  //length should be getting shorter, turn round
  Vec3 passing=viewpos-pred_flight;
     blit::debugf("vehicle%d: navigation pred %f,%f,%f\n",polyid,double(pred_flight.x),double(pred_flight.y),double(pred_flight.z));
    blit::debugf("vehicle%d: navigation pass %f,%f,%f\n",polyid,double(passing.x),double(passing.y),double(passing.z));

    
  
//if(heading_length>1000) {

//    Vec3 gross=binky.objvec-heading;
//    Vec3 gross_ang=binky.vector_toangle(gross);
    //frig with angle
//    gross_ang=gross_ang-Vec3(0.785398,0.785398,0.785398);
//   blit::debugf("vehicle%d: navigation diff %f,%f,%f\n",polyid,double(gross.x),double(gross.y),double(gross.z));
//   blit::debugf("vehicle%d: navigation diff_ang %f,%f,%f\n",polyid,double(gross_ang.x),double(gross_ang.y),double(gross_ang.z));
//}
//.....................................................................
  if(passing.x>=0) {
    course_new.y=0.0040*(150-passing.x);
    if(passing.x>150) {
//    course_correct.y=-0.0002*(;
      blit::debugf("vehicle%d: navigation too far turn ay %f\n",polyid,double(course_new.y));
    }
    if(passing.x<150) {
//    course_correct.y=+0.0002;
      blit::debugf("vehicle%d: navigation  too close turn ay %f\n",polyid,double(course_new.y));
    }
  }
  if(passing.x<0) {
    course_new.y=0.0020*(150+passing.x);
    if(passing.x<-150) {
//    course_correct.y=+0.0002;
      blit::debugf("vehicle%d: navigation too far turn ay %f\n",polyid,double(course_new.y));
    }
    if(passing.x>-150) {
//    course_correct.y=-0.0002;
      blit::debugf("vehicle%d: navigation too close turn ay %f\n",polyid,double(course_new.y));
    }
  }

//.....................................................................
  if(passing.y>=0) {
    course_new.z=0.0040*(150-passing.y);
    if(passing.y>150) {
//    course_correct.y=-0.0002*(;
      blit::debugf("vehicle%d: navigation too far turn az %f\n",polyid,double(course_new.z));
    }
    if(passing.y<150) {
//    course_correct.y=+0.0002;
      blit::debugf("vehicle%d: navigation  too close turn az %f\n",polyid,double(course_new.z));
    }
  }
  if(passing.y<0) {
    course_new.z=0.0020*(150+passing.y);
    if(passing.y<-150) {
//    course_correct.y=+0.0002;
      blit::debugf("vehicle%d: navigation too far turn az %f\n",polyid,double(course_new.z));
    }
    if(passing.y>-150) {
//    course_correct.y=-0.0002;
      blit::debugf("vehicle%d: navigation too close turn az %f\n",polyid,double(course_new.z));
    }
  }

//.....................................................................
  if(passing.z>=0) {
    course_new.x=0.0040*(150-passing.z);
    if(passing.z>150) {
//    course_correct.y=-0.0002*(;
      blit::debugf("vehicle%d: navigation too far turn ax %f\n",polyid,double(course_new.x));
    }
    if(passing.z<150) {
//    course_correct.y=+0.0002;
      blit::debugf("vehicle%d: navigation  too close turn ax %f\n",polyid,double(course_new.x));
    }
  }
  if(passing.z<0) {
    course_new.x=0.0020*(150+passing.z);
    if(passing.z<-150) {
//    course_correct.y=+0.0002;
      blit::debugf("vehicle%d: navigation too far turn ax %f\n",polyid,double(course_new.x));
    }
    if(passing.z>-150) {
//    course_correct.y=-0.0002;
      blit::debugf("vehicle%d: navigation too close turn ax %f\n",polyid,double(course_new.x));
    }
  }
    course_correct=course_new-binky.objang;

  if(course_correct.y>0.02)  { course_correct.y=0.02; }
  if(course_correct.y<-0.02) { course_correct.y=-0.02; }
//  if(course_correct.y<0.02 && course_correct.y>-0.02) { 
//  adjustang(course_correct);
//    binky.objang=binky.objang+course_correct;
//    return;
//  }
//    course_correct=course_new-binky.objang;

  if(course_correct.z>0.02)  { course_correct.z=0.02; }
  if(course_correct.z<-0.02) { course_correct.z=-0.02; }
//  if(course_correct.z<0.02 && course_correct.z>-0.02) { 
//    binky.objang=binky.objang+course_correct;
//      adjustang(course_correct);

//    return;
//  }
//  course_correct=course_new-binky.objang;

  if(course_correct.x>0.02)  { course_correct.x=0.02; }
  if(course_correct.x<-0.02) { course_correct.x=-0.02; }
//  if(course_correct.x<0.02 && course_correct.x>-0.02) { 
//    binky.objang=binky.objang+course_correct;
          adjustang(course_correct);

//return;
 // }
//---------------------------------------------------------------------
  blit::debugf("vehicle%d: navigation  ax %f ay %f az %f\n",polyid,double(course_new.x),double(course_new.y),double(course_new.z));
  blit::debugf("vehicle%d: navigation  ax %f ay %f az %f\n",polyid,double(course_correct.x),double(course_correct.y),double(course_correct.z));
//  binky.objang=binky.objang+course_correct;
}
/*--------------------------------------------------------------------*/
void vehicle::update_radar() {
//  blit::debugf("vehicle%d: radar: flight %f,%f,%f\n",polyid,double(velocity.x),double(velocity.y),double(velocity.z));
  uint_fast32_t collision_count=0;
  for (uint_fast32_t t = 0; t < count; t++) {
//    blit::debugf("vehicle%d: radar: %d %f,%f,%f\n",polyid,t,double(radar[t].x),double(radar[t].y),double(radar[t].z));
    if(radar_exist[t]==false) { continue; }
    if(radar[t].x==0 && radar[t].y==0 && radar[t].z==0) { continue; }
// get vector, normalise
    bearing=radar[t]-binky.objpos; //-radar[t];
    bear_range=bearing.length();
//      blit::debugf("vehicle%d: radar: %d bearing %f,%f,%f\n",polyid,t,double(bearing.x),double(bearing.y),double(bearing.z));
    bearing.normalize();
//      blit::debugf("vehicle%d: radar: %d normal %f,%f,%f\n",polyid,t,double(bearing.x),double(bearing.y),double(bearing.z));
  // check against direction vector NOT velocity
    approach=bearing-direction; //velocity;
//    blit::debugf("vehicle%d: radar: %d approach %f,%f,%f\n",polyid,t,double(approach.x),double(approach.y),double(approach.z));
 // difference can be bigger if distance is bigger
    working_safe_approach=safe_approach/radar_d[t];
//    blit::debugf("vehicle%d: radar: %d working safe %f %f %f\n",polyid,t,double(safe_approach),double(radar_d[t]),double(working_safe_approach));
    if(fabsf(approach.x)<safe_approach && fabsf(approach.y)<safe_approach && fabsf(approach.z)<safe_approach && bear_range<500 ) { 
      collision_count++;
      blit::debugf("vehicle%d: radar: %d collision approach %d %f,%f,%f\n",polyid,t,collision_count,double(approach.x),double(approach.y),double(approach.z));
      if(evasion==0) {
       // decide on evasion and set evasion flag
        evasion=1+(rand() % 4);
        blit::debugf("vehicle%d: radar: %d evasion mvr %d\n",polyid,t,evasion);
      }
  // set vector adjustment 
    }
     }
//  if( collision_count==0 && evasion==0) {
          //resume speed
      // resume objective, far point or near point with viewer
//  }
  if( collision_count==0 && evasion>0) {
    blit::debugf("vehicle%d: radar: no approach %f,%f,%f\n",polyid,double(approach.x),double(approach.y),double(approach.z));
    evasion=0;
  }
}
/*--------------------------------------------------------------------*/
void vehicle::update_velocity() {
 //  blit::debugf("vehicle%d: Move: Angle %f:%f,%f,%f\n",speed,double(binky.objang.x),double(binky.objang.y),double(binky.objang.z));
  direction=binky.vector_fromangle();
  binky.objvec=direction;
  Vec3 tmpnew=binky.vector_toangle(direction);
//  vel.z=-speed*cosf(binky.objang.x)-speed*cosf(binky.objang.y);
  velocity.z=direction.z*speed*-1;
//  vel.x=0; //speed*sinf(binky.objang.y)+speed*sinf(binky.objang.z);
  velocity.x=direction.x*speed;
//  vel.y=0; //speed*cos(binky.objang.z)+speed*sin(binky.objang.x);
  velocity.y=direction.y*speed*-1;
//    vel.y=speed*sinf(binky.objang.x)+speed*sinf(binky.objang.z);
    blit::debugf("vehicle%d: move Vel %f:%f,%f,%f\n",polyid,double(speed),double(velocity.x),double(velocity.y),double(velocity.z));
//  binky.objpos=binky.objpos+vel;
  adjustpos(velocity);
}
/*--------------------------------------------------------------------*/
void vehicle::adjustpos(Vec3 adjpos) {
  binky.objpos=binky.objpos+adjpos;
//  blit::debugf("vehicle%d: adjust pos: %f:%f,%f,%f\n",polyid,speed,double(binky.objpos.x),double(binky.objpos.y),double(binky.objpos.z));

}
/*--------------------------------------------------------------------*/
void vehicle::adjustang(Vec3 adjang) {
    blit::debugf("vehicle%d: adjust ang: %f %f,%f,%f\n",polyid,double(speed),double(adjang.x),double(adjang.y),double(adjang.z));

  binky.objang=binky.objang+adjang;
  if(binky.objang.x>6.283184)  { binky.objang.x=binky.objang.x-6.283184; }
  if(binky.objang.x<-6.283184) { binky.objang.x=binky.objang.x+6.283184; }
  if(binky.objang.y>6.283184)  { binky.objang.y=binky.objang.y-6.283184; }
  if(binky.objang.y<-6.283184) { binky.objang.y=binky.objang.y+6.283184; }
  if(binky.objang.z>6.283184)  { binky.objang.z=binky.objang.z-6.283184; }
  if(binky.objang.z<-6.283184) { binky.objang.z=binky.objang.z+6.283184; }
  blit::debugf("vehicle%d: adjust res: %f %f:%f,%f\n",polyid,double(speed),double(binky.objang.x),double(binky.objang.y),double(binky.objang.z));
float_t tmp=binky.objang.y/move_t;
if((tmp)<0.01999) {
    blit::debugf("vehicle%d: adjust ALERT %f %d %f \n",polyid,double(binky.objang.y),move_t,double(tmp));
}
}
/*--------------------------------------------------------------------*/
Vec3 vehicle::position() {
  blit::debugf("vehicle%d: position %f %f %f\n",polyid,double(binky.objpos.x),double(binky.objpos.y),double(binky.objpos.z));

  return(binky.objpos);
}
/*--------------------------------------------------------------------*/
Vec2 vehicle::position_view() {
  blit::debugf("vehicle%d: viewport position %f %f %f %f\n",polyid,double(binky.model_min_2D.x),double(binky.model_max_2D.x),double(binky.model_min_2D.y),double(binky.model_max_2D.y));

  return(Vec2((binky.model_max_2D.x+binky.model_min_2D.x)/2,(binky.model_max_2D.y+binky.model_min_2D.y)/2));
}
/*--------------------------------------------------------------------*/
float vehicle::laser_hit(float damage) {
  if(exist==false) { return(0); }
  if(damage<0) { return(0); }
  hitpoints=hitpoints-damage;
  blit::debugf("vehicle%d: laser damage %f %d\n",polyid,double(damage),hitpoints);

  if(hitpoints<0) {
    ploded();
  }
    if(hitpoints>0) {
    blit::debugf("vehicle%d: not yet laser \n",polyid);
  }
  return(hitpoints);
  }
/*--------------------------------------------------------------------*/
void vehicle::ploded() {
      blit::debugf("vehicle%d: sunk my battleship\n",polyid);

  exist=false;
}
/*--------------------------------------------------------------------*/
