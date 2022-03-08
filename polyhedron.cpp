#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>
#include <ctype.h>

#include "polyhedron.hpp"
using namespace blit;
/*--------------------------------------------------------------------*/
polyhedron::polyhedron(){
  serial=rand();
    printf("polyhedron: instance %ld\n",serial); 

  pt_c=1;
  dummy_ply();
  objang=Vec3(0,0,0);
  objvec=Vec3(0,0,1);
  objpos=Vec3(0,0,800);
}
/*--------------------------------------------------------------------*/
void polyhedron::init(ViewPyramid &passedview,uint8_t id,uint32_t s) {
  serial=s;
  //  blit::debugf("polyhedron%d: init %d\n",id,serial); 

  view.screen_width=passedview.screen_width;
  view.screen_height=passedview.screen_height;
  screen_off=Vec2(passedview.screen_cw,passedview.screen_ch);
  ez=view.screen_width/(2*passedview.half_fov);
  half_fov=passedview.fov/2;

  //  blit::debugf("ez %d\n",ez);
  polyid=id;
}
/*--------------------------------------------------------------------*/
void polyhedron::render(bool dump) {
      blit::debugf("polyhedron%d: render %d\n",polyid,serial); 
    model_min_2D=Vec2(321,241);
  model_max_2D=Vec2(-1,-1);
          blit::debugf("polyhedron%d: viewport position %f %f %f %f\n",polyid,double(model_min_2D.x),double(model_max_2D.x),double(model_min_2D.y),double(model_max_2D.y));

  blit::debugf("polyhedron%d: loop over %d faces\n",polyid,fc_c); 
  for(uint8_t i=0; i<fc_c; i++) {
//    //  blit::debugf("polyhedron%d: ViewFaceRender%d visible %d\n",polyid,i,model_fc_shw[i]);
    if(model_fc_shw[i]==true) {
//      //  blit::debugf("%d ",i); 
      plot_triangle(i);
//      plot_points(i);
    }
  }
      blit::debugf("polyhedron%d: viewport position %f %f %f %f\n",polyid,double(model_min_2D.x),double(model_max_2D.x),double(model_min_2D.y),double(model_max_2D.y));

//  //  blit::debugf("\n"); 
}
/*--------------------------------------------------------------------*/
void polyhedron::dummy_ply(void) {
  //debugf not avail, too early?
  model_pt[0]=Vec3( 45, -30, -60);
  model_pt[1]=Vec3( 45,  30, -60);
  model_pt[2]=Vec3(-45,  30, -60);
  model_pt[3]=Vec3(-45, -30, -60);
  model_pt[4]=Vec3(  0,   0,  60);

  model_fc[0][0] = 0;
  model_fc[0][1] = 1;
  model_fc[0][2] = 3;
  
	model_fc[1][0] = 1;
  model_fc[1][1] = 2;
  model_fc[1][2] = 3;
  
	model_fc[2][0] = 0;
  model_fc[2][1] = 1;
  model_fc[2][2] = 4;
  
	model_fc[3][0] = 0;
  model_fc[3][1] = 4;
  model_fc[3][2] = 3;
  
  model_fc[4][0] = 2;
  model_fc[4][1] = 4;
  model_fc[4][2] = 1;
  
  model_fc[5][0] = 3;
  model_fc[5][1] = 4;
  model_fc[5][2] = 2;
  
  model_col[0]= Pen(0,0,127,255);
	model_col[1]=   Pen(0,0,255,255);
	 model_col[2]=  Pen(0,127,0,255);
	model_col[3]=   Pen(0,127,127,255);
	 model_col[5]=  Pen(0,255,0,255);
	model_col[4]=   Pen(0,127,0,255);

  pt_c=5; 
  fc_c=6; 


//  printf("polyhedron: Created dummy poly, %d pt, %d fc\n",pt_c,fc_c);

//  printf("Clear 2d vector\n");
  disp_fc.clear();
  disp_z.clear();
//  printf("resize pt1 of 2d vector\n");
  disp_fc.resize(fc_c);
  disp_z.resize(fc_c);
//  printf("resize pt2 of 2d vector\n");
  for (int i=0; i<fc_c; i++) {
//     for(int n=0; n<3; n++) {
    disp_fc[i].resize(3);
  }
//  printf("size of 2d vector %d,%d,%d\n",sizeof(disp_fc),sizeof(disp_fc[0]),sizeof(disp_fc[0][0]));
//  }
//  printf("Created dummy poly, %d pt, %d fc\n",pt_c,fc_c);
//  printf("full %d, vector 0 %d\n",sizeof(model_pt),sizeof(model_pt[0]));
}
/*--------------------------------------------------------------------*/
void polyhedron::update(Vec3 viewang,Vec3 viewpos,Vec3 viewvec) {
  //  blit::debugf("polyhedron%d: Update sketch3D\n",polyid);
  
//  //  blit::debugf("polyhedron: Pos x %f:y %f:z %f \n",double(objpos.x),double(objpos.y),double(objpos.z));
//  //  blit::debugf("polyhedron: Rot x %f:y %f:z %f \n",double(objang.x),double(objang.y),double(objang.z));
//  //  blit::debugf("polyhedron: Rot x %f:y %f:z %f \n",double(objvec.x),double(objvec.y),double(objvec.z));
//  //  blit::debugf("polyhedron: View x %f:y %f:z %f \n",double(viewang.x),double(viewang.y),double(viewang.z));

//  //  blit::debugf("polyhedron: Rotating \n");
  update_rotate(viewang);
//  //  blit::debugf("polyhedron: Translate \n");
  viewobjpos=objpos-viewpos;
//  //  blit::debugf("polyhedron%d: sketch3D ViewObjPos x %f:y %f:z %f\n",polyid,viewobjpos.x,viewobjpos.y,viewobjpos.z);

  update_translate(viewang,viewpos);
            // Need to build smarter test that looks at arc of view
      // and takes into account a view length of the observer ship
      // also need to consider far distance
      
      //objpos-viewpos (dirvec) -> viewobjvec
// if viewobjvec > viewarc (angles by tan) 
      // 
  update_visible(viewpos);
  //check face normal, if -ve returned then plot_triangle
//  //  blit::debugf("polyhedron: sketch3D loop over %d faces\n",fc_c); 
  float_t dp;
  vector_trig(viewang);
  for(uint8_t i=0; i<fc_c; i++) {
    model_fc_shw[i]=false;
    viewobjfcvec[i]=0;
    for(int n=0; n<3; n++) {
 //     //  blit::debugf("polyhedron%d: sketch3D Face%d pt%d -> %d visible %d\n",polyid,i,n,model_fc[i][n],model_pt_shw[model_fc[i][n]]);
      if(model_pt_shw[model_fc[i][n]]==true) {
         model_fc_shw[i]=true;
      }
      //work out view vec to face from 3 points
      viewobjfcvec[i]+=viewobjptvec[model_fc[i][n]];
      //can we get a normal on the fly
      //each edge lies on the face
      // taking vector of two edges and
      // multiply two vectors
//        blit::debugf("polyhedron%d: sketch3D translate: face%d pt%d, adv normal normal x %f:y %f:z %f \n",polyid,i,n,double(model_pt_viewpos[model_fc[i][n]].x),double(model_pt_viewpos[model_fc[i][n]].y),double(model_pt_viewpos[model_fc[i][n]].z));

    }
    viewobjfcvec[i]=viewobjfcvec[i]/3;
    if(model_fc_shw[i]==true) {
//        blit::debugf("polyhedron%d: sketch3D visible: %d\n",polyid, i);
 //     update_rotate_normals(objang,i);
//    Vec3 testangle=vector_toangle(viewobjpos);
//      dp=update_dotproduct(viewvec,model_nm_rot[i]);
      Vec3 vectora=model_pt_viewpos[model_fc[i][1]]-model_pt_viewpos[model_fc[i][0]];
//   blit::debugf("polyhedron%d: sketch3D edge10: face%d, x %f:y %f:z %f \n",polyid,i,double(vectora.x),double(vectora.y),double(vectora.z));
      Vec3 vectorb=model_pt_viewpos[model_fc[i][2]]-model_pt_viewpos[model_fc[i][1]];
//    blit::debugf("polyhedron%d: sketch3D edge21: face%d, x %f:y %f:z %f \n",polyid,i,double(vectorb.x),double(vectorb.y),double(vectorb.z));
//   Vec3 vectorc=model_pt_viewpos[model_fc[i][0]]-model_pt_viewpos[model_fc[i][2]];
//     blit::debugf("polyhedron%d: sketch3D edge02: face%d, x %f:y %f:z %f \n",polyid,i,double(vectorc.x),double(vectorc.y),double(vectorc.z));
      vectora.normalize();
      vectorb.normalize();
// vectorc.normalize();
//    blit::debugf("polyhedron%d: sketch3D norm10: face%d,  x %f:y %f:z %f \n",polyid,i,double(vectora.x),double(vectora.y),double(vectora.z));
//    blit::debugf("polyhedron%d: sketch3D norm21: face%d,  x %f:y %f:z %f \n",polyid,i,double(vectorb.x),double(vectorb.y),double(vectorb.z));
//     blit::debugf("polyhedron%d: sketch3D norm02: face%d,  x %f:y %f:z %f \n",polyid,i,double(vectorc.x),double(vectorc.y),double(vectorc.z));


//need x product

      Vec3 vectorab=vector_cross(vectora,vectorb);
      vectorab.normalize();

//  blit::debugf("polyhedron%d: sketch3D newcrossproduct: face%d, x %f:y %f:z %f \n",polyid,i,double(vectorab.x),double(vectorab.y),double(vectorab.z));
//vectorc.cross(vectorb);
//  blit::debugf("polyhedron%d: sketch3D crossproduct: face%d, adv normal normal x %f:y %f:z %f \n",polyid,i,double(vectorc.x),double(vectorc.y),double(vectorc.z));


////  blit::debugf("polyhedron%d: sketch3D dotproduct: face%d, adv normal normal x %f:y %f:z %f \n",polyid,i,double(normb.x),double(normb.y),double(normb.z));
// Vec3 normc=vectora*vectorc;
//  normc.normalize();

////  blit::debugf("polyhedron%d: sketch3D dotproduct: face%d, adv normal normal x %f:y %f:z %f \n",polyid,i,double(normc.x),double(normc.y),double(normc.z));
//   blit::debugf("polyhedron%d: sketch3D viewvec: face%d, x %f:y %f:z %f \n",polyid,i,double(viewvec.x),double(viewvec.y),double(viewvec.z));

      dp=update_dotproduct(viewobjfcvec[i],vectorab);
//        blit::debugf("polyhedron%d: sketch3D testdotproduct: face%d, %f \n",polyid,i,double(dp));

//      dp=update_dotproduct(viewobjfcvec[i],model_nm_rot[i]);

//        blit::debugf("polyhedron%d: sketch3D dotproduct: face%d, %f \n\n",polyid,i,double(dp));
      if(dp<-0.005){
        model_fc_shw[i]=true;
//        //  blit::debugf("%d: show\n",i); 
      } else {
        model_fc_shw[i]=false;
//        //  blit::debugf("%d: not\n",i); 
      }
    }
//    //  blit::debugf("polyhedron%d: sketch3D %d visible %d\n",polyid,i,model_fc_shw[i]);
  }
}
/*--------------------------------------------------------------------
void polyhedron::update_rotate_normals(Vec3 angle,uint8_t i) {
  float_t tx,ty,tz;
  vector_trig(angle);
//  //  blit::debugf("polyhedron: model normal x %f:y %f:z %f \n",double(model_nm[i].x),double(model_nm[i].y),double(model_nm[i].z));
//  //  blit::debugf("polyhedron: rotation x %f:y %f:z %f \n",double(angle.x),double(angle.y),double(angle.z));
  tx = model_nm[i].x * co.z - model_nm[i].y * so.z;
  ty = model_nm[i].x * so.z + model_nm[i].y * co.z;
  tz = model_nm[i].z * co.x - ty * so.x;
  model_nm_rot[i].y = model_nm[i].z * so.x + ty * co.x;  
  model_nm_rot[i].x = tx * co.y - tz * so.y;
  model_nm_rot[i].z = tx * so.y + tz * co.y;
  blit::debugf("polyhedron%d: rotated normal%d x %f:y %f:z %f \n",polyid,i,double(model_nm_rot[i].x),double(model_nm_rot[i].y),double(model_nm_rot[i].z));
}
--------------------------------------------------------------------*/
void polyhedron::vector_trig(Vec3 angle) {
//    Vec3 co,so;

  co.x = cosf(angle.x);
  so.x = sinf(angle.x);
  co.y = cosf(angle.y);
  so.y = sinf(angle.y);
  co.z = cosf(angle.z);
  so.z = sinf(angle.z);
}
/*--------------------------------------------------------------------*/
void polyhedron::update_rotate(Vec3 viewang) {
//  //  blit::debugf("polyhedron: Angle x %f:y %f:z %f \n",double(objang.x),double(objang.y),double(objang.z));
//  //  blit::debugf("polyhedron: Vector x %f:y %f:z %f \n",double(objvec.x),double(objvec.y),double(objvec.z));

  viewobjang=objang-viewang;
//    //  blit::debugf("polyhedron: Sight line x %f:y %f:z %f \n",double(viewobjang.x),double(viewobjang.y),double(viewobjang.z));

  vector_trig(viewobjang);
//  vector_trig(objang);

 float_t tx,ty,tz;
  for (int i=0; i<pt_c; i++) {
   
    tx = model_pt[i].x * co.z - model_pt[i].y * so.z;
    ty = model_pt[i].x * so.z + model_pt[i].y * co.z;
    tz = model_pt[i].z * co.x - ty * so.x;
    model_pt_rot[i].y = model_pt[i].z * so.x + ty * co.x;  
    model_pt_rot[i].x = tx * co.y - tz * so.y;
    model_pt_rot[i].z = tx * so.y + tz * co.y;
//     //  blit::debugf("polyhedron: ->x %2.0f:y %2.0f:z %2.0f\n",model_pt_rot[i].x,model_pt_rot[i].y,model_pt_rot[i].z);
  }
//  //  blit::debugf("polyhedron: Finished rotate\n");
}
/*--------------------------------------------------------------------*/
void polyhedron::update_translate(Vec3 viewang,Vec3 viewpos) {
      float_t tx,ty,tz;

//   printf("ViewAng x %f:y %f:z %f\n",double(viewang.x),double(viewang.y),double(viewang.z));
  vector_trig(viewang);

//     viewobjpos=objpos-viewpos;

//   printf("ViewTr x %f:y %f:z %f\n",viewobjpos.x,viewobjpos.y,viewobjpos.z);

  tx = viewobjpos.x * co.z + viewobjpos.y * so.z;
  ty = viewobjpos.y * co.z - viewobjpos.x * so.z;
  tz = viewobjpos.z * co.x + ty * so.x;
  viewobjpos.y = ty * co.x - viewobjpos.z * so.x;  
  viewobjpos.x = tx * co.y + tz * so.y;
  viewobjpos.z = tz * co.y - tx * so.y;

  for (int i=0; i<pt_c; i++) {
//	printf("i: %d x %2.0f:y %2.0f:z %2.0f",i,model_pt_viewpos[i].x,model_pt_viewpos[i].y,model_pt_viewpos[i].z);
    model_pt_viewpos[i].x = model_pt_rot[i].x + viewobjpos.x;
    model_pt_viewpos[i].y = model_pt_rot[i].y + viewobjpos.y;
    model_pt_viewpos[i].z = model_pt_rot[i].z + viewobjpos.z;
 //   //  blit::debugf("polyhedron%d: translate %d: x %2.0f:y %2.0f:z %2.0f\n",polyid,i,double(model_pt_viewpos[i].x),double(model_pt_viewpos[i].y),double(model_pt_viewpos[i].z));
 
    // Also work out viewvector for point
    viewobjptvec[i]=model_pt_viewpos[i]-viewpos;
    //normalise vector to unit length
    viewobjptvec[i].normalize();
//    //  blit::debugf("polyhedron%d: translate %d view- x %f:y %f:z %f\n",polyid,i,viewobjptvec[i].x,viewobjptvec[i].y,viewobjptvec[i].z);
  }
}
/*--------------------------------------------------------------------*/
void polyhedron::update_visible(Vec3 viewpos) {
  Vec3 vector,angle;
//  //  blit::debugf("polyhedron%d: ViewObjPos view- x %f:y %f:z %f fov/2 %f\n",polyid,viewpos.x,viewpos.y,viewpos.z,half_fov);
  for (int i=0; i<pt_c; i++) {
//    //  blit::debugf("polyhedron%d: ViewObjPos point%d x %f:y %f:z %f\n",polyid,i,model_pt_viewpos[i].x,model_pt_viewpos[i].y,model_pt_viewpos[i].z);
    vector=model_pt_viewpos[i]-viewpos;
//    //  blit::debugf("polyhedron%d: ViewObjPos vector%d x %f:y %f:z %f\n",polyid,i,vector.x,vector.y,vector.z);
    angle=vector_toangle(vector);
     blit::debugf("polyhedron%d: viewobjpos angle%d x %f:y %f:z %f\n",polyid,i,double(angle.x),double(angle.y),double(angle.z));
    model_pt_shw[i]=true;
    if(angle.x>half_fov || angle.x<(0-half_fov)) {
      model_pt_shw[i]=false;
//      //  blit::debugf("polyhedron%d: ViewObjPos point%d not visible\n",polyid,i);

    }

  }
  }

/*--------------------------------------------------------------------*/
Vec3 polyhedron::vector_cross(Vec3 vectora,Vec3 vectorb) {
  Vec3 vectorab;
  vectorab.x=vectora.y*vectorb.z - vectora.z*vectorb.y;
    vectorab.y=vectora.x*vectorb.z - vectora.z*vectorb.x;
      vectorab.z=vectora.y*vectorb.x - vectora.x*vectorb.y;
      return(vectorab);
}

/*--------------------------------------------------------------------*/
Vec3 polyhedron::vector_toangle(Vec3 vector) {
  Vec3 angle;
  if(vector.x==0) { vector.x=0.1; }
  if(vector.y==0) { vector.y=0.1; }
  if(vector.z==0) { vector.z=0.1; }
  if(vector.z>0) {
    angle.x=atanf(vector.y/vector.z);
  } else {
    angle.x=3.1416+atanf(vector.y/vector.z);
  }
  angle.y=atanf(vector.z/vector.x);
  angle.z=atanf(vector.y/vector.x);

  blit::debugf("polyhedron%d: object AnglefromVector x %f:y %f:z %f \n",polyid,double(angle.x),double(angle.y),double(angle.z));
return(angle);
}
/*--------------------------------------------------------------------*/
Vec3 polyhedron::vector_fromangle() {
  Vec3 outvec;
  blit::debugf("polyhedron%d: object angle x %f:y %f:z %f \n",polyid,double(objang.x),double(objang.y),double(objang.z));
//      //  blit::debugf("polyhedron: View Angle x %f:y %f:z %f \n",double(viewang.x),double(viewang.y),double(viewang.z));

//    float xcosay=objang.x*cosf(objang.y);
//    float xsinay=objang.x*sinf(objang.y);
//    float zsinay=objang.z*sinf(objang.y);
//        float tanay=tanf(objang.y);

//    float angv=xcosay+zsinay; //+xsinay
//    float lenv=cosf(angv);
//      blit::debugf("polyhedron%d: object tmp %f,%f,%f,%f,%f \n",polyid,double(xcosay),double(xsinay),double(zsinay),double(angv),double(lenv));

//  outvec.x=cosf(objang.z)+sinf(objang.y);
//  outvec.x=outvec.x-1;
//  outvec.x=cosf(objang.z)+sinf(objang.y)+cosf(objang.y)+sinf(objang.z);
//    outvec.x=sinf(objang.y)*cosf(objang.z)*cosf(objang.x);
    outvec.x=sinf(objang.y)*cosf(objang.x);


//    blit::debugf("polyhedron%d: trig  %f %f %f %f %f %f \n",polyid,double(cosf(objang.x)),double(sinf(objang.x)),double(cosf(objang.y)),double(sinf(objang.y)),double(cosf(objang.z)),double(sinf(objang.z)));

//  outvec.y=sinf(objang.z)+sinf(objang.x);
//  outvec.y=sinf(objang.x)*cosf(objang.z);
//outvec.y=sinf(angv);
outvec.y=sinf(objang.x); //+sinf(objang.z);


//if(tanaz!=0) {
//  outvec.x=outvec.y/tanaz;

//} else {
//outvec.x=sinf(objang.y)*lenv;
//}

  outvec.z=cosf(objang.x)*cosf(objang.y);
//  if(tanay==0) {
//    outvec.x=0;
//  } else {
//  outvec.x=outvec.z/tanay;
//}
//  outvec.z=outvec.z-1;
//outvec.z=cosf(objang.y)*lenv;

  blit::debugf("polyhedron%d: object vector x %f:y %f:z %f \n",polyid,double(outvec.x),double(outvec.y),double(outvec.z));
  return(outvec);
}
/*--------------------------------------------------------------------*/
float_t polyhedron::update_dotproduct(Vec3 viewvec,Vec3 normvec) {
//  //  blit::debugf("polyhedron%d: dotproduct: \n");
//  //  blit::debugf("polyhedron%d: dotproduct: View Vector x %f:y %f:z %f \n",polyid,double(viewvec.x),double(viewvec.y),double(viewvec.z));
//  //  blit::debugf("polyhedron%d: dotproduct: Face Normal x %f:y %f:z %f \n",polyid,double(normvec.x),double(normvec.y),double(normvec.z));

  float_t x=viewvec.x*normvec.x;
  float_t y=viewvec.y*normvec.y;
  float_t z=viewvec.z*normvec.z;
//  //  blit::debugf("polyhedron%d: dotproduct: x %f:y %f:z %f \n",polyid,double(x),double(y),double(z));

  float_t sigma=x+y+z;
  return(sigma);
}
/*--------------------------------------------------------------------*/
void polyhedron::flatten_tri(int i) {


  for(int n=0; n<3; n++) {
    model_view_2D[n].x=model_pt_viewpos[model_fc[i][n]].x * ez / model_pt_viewpos[model_fc[i][n]].z;
    model_view_2D[n].y=model_pt_viewpos[model_fc[i][n]].y * ez / model_pt_viewpos[model_fc[i][n]].z;
   
  }
 
}
/*--------------------------------------------------------------------*/
Point polyhedron::flatten_v(Vec3 v) {
  Point tmp;
    tmp.x=v.x * ez / v.z;
    tmp.y=v.y * -ez / v.z;
  return(tmp);
}
/*--------------------------------------------------------------------*/
int32_t polyhedron::orient2d(Point p1, Point p2, Point p3) {
  return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}
/*--------------------------------------------------------------------*/
bool polyhedron::is_top_left(const Point &p1, const Point &p2) {
  return (p1.y == p2.y && p1.x > p2.x) || (p1.y < p2.y);
  }
/*--------------------------------------------------------------------*/
void polyhedron::plot_triangle(int i) {
 
  for(int n=0; n<3; n++) {
    //  blit::debugf(" %d:",n);
    model_view_2D[n]=flatten_v(model_pt_viewpos[model_fc[i][n]])+screen_off;
    //  blit::debugf(" %d,%d ",model_view_2D[n].x,model_view_2D[n].y);
     if(model_view_2D[n].x<model_min_2D.x) { model_min_2D.x=model_view_2D[n].x; }
    if(model_view_2D[n].x>model_max_2D.x) { model_max_2D.x=model_view_2D[n].x; }
    if(model_view_2D[n].y<model_min_2D.y) { model_min_2D.y=model_view_2D[n].y; }
    if(model_view_2D[n].y>model_max_2D.y) { model_max_2D.y=model_view_2D[n].y; }
  }
//  screen.pen = Pen(32, 32,255);
  screen.pen = model_col[i];     
  //  blit::debugf("\n");
  screen.triangle(model_view_2D[0]+screen_off,model_view_2D[1]+screen_off,model_view_2D[2]+screen_off);
  
  


  


}
/*--------------------------------------------------------------------*/
void polyhedron::plot_points(int i) {
  for(int n=0; n<3; n++) {
    model_view_2D[n]=flatten_v(model_pt_viewpos[model_fc[i][n]])+screen_off;
  }
  screen.pen = Pen(255, 255,255);
//  screen.pen = model_col[i];     
//  screen.triangle(model_view_2D[0]+screen_off,model_view_2D[1]+screen_off,model_view_2D[2]+screen_off);
  screen.circle(model_view_2D[0]+screen_off,10);
  screen.circle(model_view_2D[1]+screen_off,10);
  screen.circle(model_view_2D[2]+screen_off,10);
}
/*--------------------------------------------------------------------*/

void polyhedron::model_load(char *obj_file) {
    blit::debugf("polyhedron%d: load poly\n",polyid);
//  pyramid_object = Object::load_obj((char*)pyramid_obj);
    pyramid_object = Object::load_obj(obj_file);
//    blit::debugf("polyhedron: got poly\n");
//    blit::debugf("polyhedron: Object pyramid\n");
    blit::debugf("polyhedron: vertices: %d\n",pyramid_object->vc);
    pt_c=pyramid_object->vc; 
  for (unsigned int i = 0; i < pyramid_object->vc; i++) {
      blit::debugf(" :   point %d: %f,%f,%f\n",i,double(pyramid_object->v[i].x),double(pyramid_object->v[i].y),double(pyramid_object->v[i].z));
    model_pt[i]=pyramid_object->v[i];
  }
  fc_c=0;
//   blit::debugf("polyhedron:  Groups: %d\n",pyramid_object->gc);
  for (unsigned int i = 0; i < pyramid_object->gc; i++) {
//    blit::debugf("polyhedron:   faces in group %d: %d\n",i,pyramid_object->g[i].fc);
    for (unsigned int ii = 0; ii < pyramid_object->g[i].fc; ii++) {
      
 //     printf("  point %d: %f,%f,%f\n",i,pyramid_object->g[i].f[ii].x,pyramid_object->g[i].f[ii].y,pyramid_object->f[i].f[ii].z);
//      blit::debugf("polyhedron:    face %d: ",ii);
      for (unsigned int iii = 0; iii < 3; iii++) {

//          blit::debugf(" %d ",pyramid_object->g[i].f[ii].v[iii]);
        model_fc[ii][iii] = pyramid_object->g[i].f[ii].v[iii];
      }
//       blit::debugf("\n");
    }
  }
  fc_c=pyramid_object->g[0].fc;
    pt_c=pyramid_object->vc; 
    blit::debugf("polyhedron%d: loaded poly points %d, faces %d\n",polyid,pt_c,fc_c);

// Now we work normals on the fly from edge vectors
//  //  blit::debugf("polyhedron:  Normals: %d\n",pyramid_object->nc);
//  for (unsigned int i = 0; i < pyramid_object->nc; i++) {
//    //  blit::debugf("polyhedron:   vector %d: %f,%f,%f\n",i,double(pyramid_object->n[i].x),double(pyramid_object->n[i].y),double(pyramid_object->n[i].z));
//  model_nm[i]=pyramid_object->n[i];
//  }
}
/*--------------------------------------------------------------------*/   
