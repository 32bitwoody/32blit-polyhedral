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
pos=Vec3(20,20,320);

}

void polyhedron::copy_poly(void) {
  for (int i=0; i<pt_c; i++) {
    nstnc.emplace_back(Vec3(model_pt[i].x,model_pt[i].y,model_pt[i].z));
	pstnc.emplace_back(Vec3(model_pt[i].x,model_pt[i].y,model_pt[i].z));
  }
}
void polyhedron::dummy_quad() {
  quad.emplace_back(Point(10,10));
  quad.emplace_back(Point(10,20));
  quad.emplace_back(Point(20,20));
//quad.emplace_back(Point(20,10));
}
void polyhedron::drawpoly() {

 
// screen.pen = Pen(255,0,0,255);
//  screen.polygon(quad);
// printf("Create temp face vectors\n");
//    loop through faces and flatten and draw;
//model_fc needs to be array of array

  printf("Looping through %d faces\n",fc_c);
  for (int i=0; i<fc_c; i++) {
//  for (i=0; i<fc_c; i++) {
	  printf("face %d: ",i);
	printf("pt1 %d:pt2 %d:pt3 %d\n",model_fc[i][0],model_fc[i][1],model_fc[i][3]);

//	  for (int j=0; j<3; j++) {
//	    Vec3 n=fcpt;

	  //   at some point we will want to look at z and decide which to draw first 
//	printf("0: x %f:y %f:z %f ",model_fc[0].x,model_fc[0].y,model_fc[0].z);
//	printf("1: x %f:y %f:z %f ",model_fc[1].x,model_fc[1].y,model_fc[1].z);
//	printf("2: x %f:y %f:z %f\n",model_fc[1].x,model_fc[2].y,model_fc[2].z);
    

    flatten(i);
    printf("0: x %d:y %d ",nstnc_flt[0].x,nstnc_flt[0].y);
	printf("1: x %d:y %d ",nstnc_flt[1].x,nstnc_flt[1].y);
	printf("2: x %d:y %d\n",nstnc_flt[2].x,nstnc_flt[2].y);
   printf("Draw 2D triangle\n");
//    drawtri(i);
  screen.pen = model_col[i];
      screen.triangle(nstnc_flt[0]+screen_off,nstnc_flt[1]+screen_off,nstnc_flt[2]+screen_off);

//    col=col+20;
//    i++;
  }
}
void polyhedron::move(void) {
	rotate();
	translate();
}
void polyhedron::display(void) {
//  printf("pt_c %d\n",pt_c);
//  printf("nstnc %ld ",sizeof(nstnc));
  rotate();
//  for (int i=0; i<pt_c; i++) {
//	printf("%d ins x %f:y %f:z %f ",i,nstnc[i].x,nstnc[i].y,nstnc[i].z);
//    printf("rot x %f:y %f:z %f ",nstnc[i].x,nstnc[i].y,nstnc[i].z);
  translate();
//    printf("mov x %f:y %f:z %f \n",pstnc[i].x,pstnc[i].y,pstnc[i].z);
  drawpoly();
//  }
  printf("--------\n");
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
//  model_shp = {3,3,3,3};

  model_fc = {
	{0,1,2},
	{0,1,3},
	{0,2,3},
	{1,2,3}
  };
  model_col = {
	  Pen(0,0,120,255),
	  Pen(0,0,160,255),
	  Pen(0,0,200,255),
	  Pen(0,0,240,255)
};
//  nstnc_flt= {
//	  Vec2(0,1),
//	  Vec2(0,1),
//	  Vec2(0,2),
//	  Vec2(1,2)
//};
  pt_c=4; //sizeof(model_pt)/sizeof(model_pt[0]);
  fc_c=4; //sizeof(model_fc)/sizeof(model_fc[0]);
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
  for (int i=0; i<pt_c; i++) {
    printf("i: %d x %2.0f:y %2.0f:z %2.0f",i,nstnc[i].x,nstnc[i].y,nstnc[i].z);
    int tx,ty,tz;

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
}
void polyhedron::inrange(void) {

}
void polyhedron::orderpoly(void) {

}
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
void polyhedron::drawtri(int i) {
  screen.pen = model_col[i];
}
void polyhedron::holddrawtri(int i) {

//  Pen p(model_col[i].r, model_col[i].g, model_col[i].b, model_col[i].alpha);
// p=(0,255,0,255);


  screen.pen = model_col[i];
  
  screen.triangle(nstnc_flt[0]+screen_off,nstnc_flt[1]+screen_off,nstnc_flt[2]+screen_off);
  screen.pen = Pen(255,0,0,255);
//  screen.line(nstnc_flt[0]+screen_off,nstnc_flt[1]+screen_off);
  
  
//  void Surface::line(const Point &p1, const Point &p2) {
Point p1=Point(4,12);
Point p2=Point(12,4);
    int32_t dx = int32_t(abs(p2.x - p1.x));
    int32_t dy = -int32_t(abs(p2.y - p1.y));
printf("p1.x=%d p1.y=%d ",p1.x,p1.y);
printf("p2.x=%d p2.y=%d\n",p2.x,p2.y);

printf("dx=%d dy=%d : ",dx,dy);

    int32_t sx = (p1.x < p2.x) ? 1 : -1;
    int32_t sy = (p1.y < p2.y) ? 1 : -1;
printf("sx=%d sy=%d : ",sx,sy);
    int32_t err = dx + dy;
printf("err=%d : ",err);
    Point p(p1);
printf("p.x=%d p.y=%d\n",p.x,p.y);

    while (true) {
      if (screen.clip.contains(p)) {
//        screen.pbf(&pen, this, offset(p), 1);
printf(" plot\n");
      }

      if ((p.x == p2.x) && (p.y == p2.y)) break;

      int32_t e2 = err * 2;
            printf("e2=%d: ",e2);

      if (e2 >= dy) { err += dy; p.x += sx; printf("dy=%d err+=dy;px=sx ",dy); }
      if (e2 <= dx) { err += dx; p.y += sy; printf("dx=%d err+=dx;py=sy ",dx); }
      printf("err=%d ",err);
  
printf("p.x=%d p.y=%d ",p.x,p.y);
    }
//  }
  
  
//  screen.triangle(vecs[0]+screen_off,vecs[1]+screen_off,vecs[2]+screen_off);
}
