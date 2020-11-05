#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>

#include "polyhedron.hpp"

using namespace blit;

 polyhedron::polyhedron(){
pt_c=1;
dummy_ply();
avl=Vec3(0,0,0);
pos=Vec3(30,30,320);
//pos=Vec3(0,0,0);

}

void polyhedron::copy_poly(void) {
  for (int i=0; i<pt_c; i++) {
    nstnc.emplace_back(Vec3(model_pt[i].x,model_pt[i].y,model_pt[i].z));
	pstnc.emplace_back(Vec3(model_pt[i].x,model_pt[i].y,model_pt[i].z));
  }
}
void polyhedron::dummy_quad() {
  int local_z=0;
  printf("Set max_z\n");
  max_z=0;
  min_z=2000; // Need to think about visible range
  printf("loop over %d faces\n",fc_c); 
  for(int i=0; i<fc_c; i++) {
    printf("i%d:",i);
    local_z=0;
    for(int n=0; n<3; n++) {
      printf(" n%d X:%f,Y:%f,Z:%f ",n,pstnc[model_fc[i][n]].x,pstnc[model_fc[i][n]].y,pstnc[model_fc[i][n]].z);
      disp_fc[i][n].x=pstnc[model_fc[i][n]].x * ez / pstnc[model_fc[i][n]].z;
      disp_fc[i][n].y=pstnc[model_fc[i][n]].y * ez / pstnc[model_fc[i][n]].z;
//      if(disp_fc[i][n].x<0) { disp_fc[i][n].x=0; }
//      if(disp_fc[i][n].x>screen_width) { disp_fc[i][n].x=screen_width; }
//      if(disp_fc[i][n].y<0) { disp_fc[i][n].y=0; }
//      if(disp_fc[i][n].y>screen_width) { disp_fc[i][n].y=screen_width; }
      printf("x-%d,y-%d:",disp_fc[i][n].x,disp_fc[i][n].y);
      
      if(pstnc[model_fc[i][n]].z>local_z) {
        local_z=pstnc[model_fc[i][n]].z;
        printf("\nsetting z-%d\n",local_z);

      }
      if(pstnc[model_fc[i][n]].z>max_z) {
        max_z=pstnc[model_fc[i][n]].z;
      }
       if(pstnc[model_fc[i][n]].z<min_z) {
        min_z=pstnc[model_fc[i][n]].z;
      }
    }
    disp_z[i]=local_z;
    printf("z-%d\n",local_z);
    min_z++;
  }
//  quad.emplace_back(Point(10,10));
//  quad.emplace_back(Point(10,20));
//  quad.emplace_back(Point(20,20));
//quad.emplace_back(Point(20,10));
}
void polyhedron::drawpoly() {
  int z=max_z;
  int i=0;
  while(z>min_z) {
    if(disp_z[i]==z && i<fc_c) {
      screen.pen = model_col[i];
      printf("Display triange %d\n",i);
      screen.triangle(disp_fc[i][0]+screen_off,disp_fc[i][1]+screen_off,disp_fc[i][2]+screen_off);
    }
    i++;
    if(i>fc_c) {
      i=0;
      z--;
    }
  }
} 
// screen.pen = Pen(255,0,0,255);
//  screen.polygon(quad);
// printf("Create temp face vectors\n");
//    loop through faces and flatten and draw;
//model_fc needs to be array of array

//  printf("Looping through %d faces\n",fc_c);
//  for (int i=0; i<fc_c; i++) {
//  for (i=0; i<fc_c; i++) {
//	  printf("face %d: ",i);
//	printf("pt1 %d:pt2 %d:pt3 %d\n",model_fc[i][0],model_fc[i][1],model_fc[i][3]);

//	  for (int j=0; j<3; j++) {
//	    Vec3 n=fcpt;

	  //   at some point we will want to look at z and decide which to draw first 
//	printf("0: x %f:y %f:z %f ",model_fc[0].x,model_fc[0].y,model_fc[0].z);
//	printf("1: x %f:y %f:z %f ",model_fc[1].x,model_fc[1].y,model_fc[1].z);
//	printf("2: x %f:y %f:z %f\n",model_fc[1].x,model_fc[2].y,model_fc[2].z);
    

//    flatten(i);
//    printf("0: x %d:y %d ",nstnc_flt[0].x,nstnc_flt[0].y);
//	printf("1: x %d:y %d ",nstnc_flt[1].x,nstnc_flt[1].y);
//	printf("2: x %d:y %d\n",nstnc_flt[2].x,nstnc_flt[2].y);
//   printf("Draw 2D triangle\n");
//  screen.pen = model_col[i];
//      screen.triangle(nstnc_flt[0]+screen_off,nstnc_flt[1]+screen_off,nstnc_flt[2]+screen_off);

//  }
//}
void polyhedron::move(void) {
	rotate();
	translate();
}
void polyhedron::display(void) {
//  printf("pt_c %d\n",pt_c);
//  printf("nstnc %ld ",sizeof(nstnc));
  printf("Rotating ");
  rotate();
//  for (int i=0; i<pt_c; i++) {
//	printf("%d ins x %f:y %f:z %f ",i,nstnc[i].x,nstnc[i].y,nstnc[i].z);
//    printf("rot x %f:y %f:z %f ",nstnc[i].x,nstnc[i].y,nstnc[i].z);
  printf("Translate ");
  translate();
//    printf("mov x %f:y %f:z %f \n",pstnc[i].x,pstnc[i].y,pstnc[i].z);
//  drawpoly();
//   reduceface();
  printf("Flatten ");
  dummy_quad();
//  }
//  printf("--------\n");
}

void polyhedron::loadply(void) {
 int f=OpenMode::read;
//  blit::
 if(tfile.open("test.txt",OpenMode::read)) {
	 printf("file opened\n");
	 tfile.read(0,10,(char *)text_buf);
	 printf(":%s",text_buf);
	 tfile.close();
 }
 if(tfile.open("test2.txt",OpenMode::write)) {
	 printf("file opened\n");
	 tfile.write(0,10,(char *)text_buf);
	 tfile.close();
 }
}


void polyhedron::dummy_ply(void) {
  model_pt.clear();
  model_pt.emplace_back(Vec3( 90,   0, 90));
  model_pt.emplace_back(Vec3(-90,   0, 90));
  model_pt.emplace_back(Vec3(  0,  90,  0));
  model_pt.emplace_back(Vec3(  0,   0, -90));
  model_pt.emplace_back(Vec3( 0,   0, 0));
  model_pt.emplace_back(Vec3( 10,   0, 0));
  model_pt.emplace_back(Vec3( 0,   10, 0));
  model_pt.emplace_back(Vec3( 0,   0, 10));

//  model_shp = {3,3,3,3};

  model_fc = {
	{0,1,2},
	{1,2,3},
	{0,2,3},
	{0,1,3}
  };
  model_col = {
	  Pen(240,0,0,255),
	  Pen(0,240,0,255),
	  Pen(0,0,240,255),
	  Pen(240,240,240,128)
};
//  nstnc_flt= {
//	  Vec2(0,1),
//	  Vec2(0,1),
//	  Vec2(0,2),
//	  Vec2(1,2)
//};
  pt_c=8; //sizeof(model_pt)/sizeof(model_pt[0]);
  fc_c=4; //sizeof(model_fc)/sizeof(model_fc[0]);
  printf("Clear 2d vector\n");
  disp_fc.clear();
  disp_z.clear();
  printf("resize pt1 of 2d vector\n");
  disp_fc.resize(fc_c);
  disp_z.resize(fc_c);
  printf("resize pt2 of 2d vector\n");
  for (int i=0; i<fc_c; i++) {
//     for(int n=0; n<3; n++) {
    disp_fc[i].resize(3);
  }
  printf("size of 2d vector %ld,%ld,%ld\n",sizeof(disp_fc),sizeof(disp_fc[0]),sizeof(disp_fc[0][0]));
//  }
  printf("Created dummy poly, %d pt, %d fc\n",pt_c,fc_c);
//  printf("full %ld, vector 0 %ld\n",sizeof(model_pt),sizeof(model_pt[0]));
  copy_poly();
}
void polyhedron::rotate(void) {

  Vec3 c,s;
    printf("Rot x %f:y %f:z %f \n",avl.x,avl.y,avl.z);

  c.x = cosf(avl.x);
  s.x = sinf(avl.x);
  c.y = cosf(avl.y);
  s.y = sinf(avl.y);
  c.z = cosf(avl.z);
  s.z = sinf(avl.z);
      printf("Axis x sin %f: cos %f, ",s.x,c.x);
      printf("Axis y sin %f: cos %f, ",s.y,c.y);
      printf("Axis z sin %f: cos %f\n",s.z,c.z);
  printf("Points %d\n",pt_c);
  for (int i=0; i<pt_c; i++) {
    printf("i: %d x %2.0f:y %2.0f:z %2.0f",i,nstnc[i].x,nstnc[i].y,nstnc[i].z);

    float_t tx,ty,tz;

    tx = nstnc[i].x * c.z - nstnc[i].y * s.z;
    ty = nstnc[i].x * s.z + nstnc[i].y * c.z;
   
    tz = nstnc[i].z * c.x - ty * s.x;
    nstnc[i].y = nstnc[i].z * s.x + ty * c.x;  
//    t2.x=nstnc[i].x;
//    t2.z=nstnc[i].z;
//    nstnc[i].y=nstnc[i].y;   
    nstnc[i].x = tx * c.y - tz * s.y;
    nstnc[i].z = tx * s.y + tz * c.y;
      printf(" ->x %2.0f:y %2.0f:z %2.0f\n",nstnc[i].x,nstnc[i].y,nstnc[i].z);

  }
  printf("Finished rotate\n");
}
void polyhedron::translate(void) {
  printf("Mov x %f:y %f:z %f\n",pos.x,pos.y,pos.z);

  for (int i=0; i<pt_c; i++) {
	printf("i: %d x %2.0f:y %2.0f:z %2.0f",i,pstnc[i].x,pstnc[i].y,pstnc[i].z);

    pstnc[i].x = nstnc[i].x + pos.x;
    pstnc[i].y = nstnc[i].y + pos.y;
    pstnc[i].z = nstnc[i].z + pos.z;
    printf(" ->x %2.0f:y %2.0f:z %2.0f\n",pstnc[i].x,pstnc[i].y,pstnc[i].z);
  }
  printf("Finished translate\n");
}
void polyhedron::inrange(void) {

}
void polyhedron::orderpoly(void) {
	int n=0;
//  for (int i=0; i<pt_c; i++) {
//}
}
//void polyhedron::reduceface(void) {
 // int max_z=0; 
//  for (int i=0; i<fc_c; i++) {
//    int local_z=0;
//    for(int n=0; n<3; n++) {
//      disp_fc[i][n].x=pstnc[model_fc[i][n]].x * ez / pstnc[model_fc[i][n]].z;
//      disp_fc[i][n].y=pstnc[model_fc[i][n]].y * ez / pstnc[model_fc[i][n]].z;
//      if(pstnc[model_fc[i][n]].z>local_z) {
//        local_z=pstnc[model_fc[i][n]].z;
 //     }
//      if(pstnc[model_fc[i][n]].z>max_z) {
//        max_z=pstnc[model_fc[i][n]].z;
//      }
 
//    }
//    disp_z[i]=local_z;
//  }
//}
void polyhedron::flatten(int i) {
//  for (int i=0; i<fc_c; i++) {
//  std::array<Point, 3> face;
//	    face[j]=pstnc[n.x];
//  face[0]=pstnc[model_fc[i].x];
//  face[1]=pstnc[model_fc[i].y];
//  face[2]=pstnc[model_fc[i].z];
  for(int n=0; n<3; n++) {
//	face[n]=pstnc[model_fc[i].x];

    nstnc_flt[n].x=pstnc[model_fc[i][n]].x * ez / pstnc[model_fc[i][n]].z;
    nstnc_flt[n].y=pstnc[model_fc[i][n]].y * ez / pstnc[model_fc[i][n]].z;
  }
}
