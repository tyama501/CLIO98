/* Docking to Space station */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "CLIO98.H"
#include "XYZ3D.H"

#define LIOSIZE 5200

#define DEBUG 0

#define SPEED 16
#define DELTATIME 0.1

#define GRAVITY 9.8
#define RANGE 10

char *lioaddr;
char far *liobuf;

char _asm_1(char *);

/* DOS */
#define c_INT_21() _asm_1("\n\tINT\t33\n")
#define c_AH_08() _asm_1("\n\tMOV\tAH,8\n")
#define c_AH_0B() _asm_1("\n\tMOV\tAH,11\n")

void drawxzplane(void);
void drawlaunch(void);
void drawvessel(coord_t scoord);
coord_t calcscoord(coord_t scoord, coord_t vxyz, float t);
int checkvessel(coord_t *coord, coord_t nscoord);

void drawaxis(void);

int main(void)
{
  int i, j;

  /* Coordinate of Space station */
  coord_t coord[12] = {{-10,-10,0},
                      {-5,-10,0},
                      {-5,-5,0},
                      {5,-5,0},
                      {5,-10,0},
                      {10,-10,0},
                      {10,10,0},
                      {5,10,0},
                      {5,5,0},
                      {-5,5,0},
                      {-5,10,0},
                      {-10,10,0}};

  coord_t pcoord[12];

  /* Coordinate of Space vessel */
  coord_t scoord = {110,0,110};
  coord_t nscoord;

  float v0 = 80;
  float angle0 = 1.5;
  float t = 0;

  coord_t vxyz = {0,0,0};

  int lenable = 0;

  /* Matrix */
  float pm[4][4] = {{1,0,0,30},
                    {0,1,0,120},
                    {0,0,1,30},
                    {0,0,0,1}};

  float rm[4][4] = {{1,0,0,0},
                    {0,1,0,0},
                    {0,0,1,0},
                    {0,0,0,1}};

  float xyzf[3];
  float xyzfn[3];
  float radf = PI / SPEED;

  /* Point of Space station to connect */
  int pointnum[12][2] = {{0,1},
                         {1,2},
                         {2,3},
                         {3,4},
                         {4,5},
                         {5,6},
                         {6,7},
                         {7,8},
                         {8,9},
                         {9,10},
                         {10,11},
                         {11,0}};

  char c_key;

  lioaddr = (char *)calloc(LIOSIZE, sizeof(char));

  liobuf = (char far *)lioaddr;

  lioset();
  lioginit(liobuf);
  liogscre(liobuf);
  liogview(liobuf);
  liogcol1(liobuf);
  liogcls(liobuf);

  system("cls");
  printf("[Control Center]\n");
  printf("Press v to set initial velocity\n");
  printf("Press a to set angle\n");
  printf("Press l to launch vessel\n");
  printf("Press q to exit\n");

  /* Parallel move */
  for (i = 0; i < 12; i++){
    xyzf[0] = coord[i].x;
    xyzf[1] = coord[i].y;
    xyzf[2] = coord[i].z;

    for (j = 0; j < 3; j++){
      xyzfn[j] = pm[j][0] * xyzf[0] + pm[j][1] * xyzf[1] + pm[j][2] * xyzf[2] + pm[j][3];
    }

    coord[i].x = xyzfn[0];
    coord[i].y = xyzfn[1];
    coord[i].z = xyzfn[2];
  }


  rm[0][0] = cos(radf);
  rm[0][2] = sin(radf);
  rm[2][0] = -sin(radf);
  rm[2][2] = cos(radf);

  drawxzplane();
  drawlaunch();

  while (1){

    /* Draw Space station */  
    for (i = 0; i < 12; i++){
      line_xyz(liobuf, coord[pointnum[i][0]], coord[pointnum[i][1]], 0);
    }

    for (i = 0; i < 12; i++){
      plotxyz(liobuf, coord[i]);
    }

    /* Draw Space vessel */
    if (lenable){
      t+=DELTATIME;
      nscoord = calcscoord(scoord, vxyz, t);

      if (nscoord.y > 0){
        drawvessel(nscoord);
        lenable = checkvessel(coord, nscoord);
      }
      else {
        lenable = 0;
        printf("Fail!\n");
      }
    }
    else {
      t = 0;
    }

    /* Draw axis */
    drawaxis();

    /* Rotate */
    for (i = 0; i < 12; i++){

      pcoord[i].x = coord[i].x;
      pcoord[i].y = coord[i].y;
      pcoord[i].z = coord[i].z;

      xyzf[0] = coord[i].x;
      xyzf[1] = coord[i].y;
      xyzf[2] = coord[i].z;

      for (j = 0; j < 3; j++){
        xyzfn[j] = rm[j][0] * xyzf[0] + rm[j][1] * xyzf[1] + rm[j][2] * xyzf[2];
      }

      coord[i].x = xyzfn[0];
      coord[i].y = xyzfn[1];
      coord[i].z = xyzfn[2];
    }

    c_AH_0B();
    if (c_INT_21()==0xFF){
      c_AH_08();
      c_key = c_INT_21();
      if (c_key=='q' || c_key=='Q'){
        liogcls(liobuf);
        free(lioaddr);
        exit(0);
      }
      else if (c_key=='v' || c_key=='V'){
        printf("value ?\n");
        scanf("%f", &v0);
        printf("value changed to %f\n", v0);
      }
      else if (c_key=='a' || c_key=='A'){
        printf("value [degree]?\n");
        scanf("%f", &angle0);
        angle0 = angle0 * PI / 180;
        printf("value changed to %f[rad]\n", angle0);
      }
      else if (c_key=='l' || c_key=='L'){
        vxyz.x = -v0 * cos(angle0) * 1.41421356;
        vxyz.y = v0 * sin(angle0);
        vxyz.z = vxyz.x;
        lenable = 1;
      }
    }

    /* Erase Space station */
    for (i = 0; i < 12; i++){
      line_xyz(liobuf, pcoord[pointnum[i][0]], pcoord[pointnum[i][1]], 1);
    }

  }

  free(lioaddr);
  return 0;
}

void drawxzplane(void)
{
  int i;

  coord_t grid = {0,0,0};

  int startgrid[2];
  int endgrid[2];

  for (i = -160; i < 160; i+=20){

    grid.x = i;
    grid.z = 0;
    startgrid[0] = calcX(grid);
    startgrid[1] = calcY(grid);
    grid.z = 160;
    endgrid[0] = calcX(grid);
    endgrid[1] = calcY(grid);

    lioglinb(liobuf, startgrid[0], startgrid[1], endgrid[0], endgrid[1], 1, 0);

  }

  for (i = 0; i < 160; i+=20){ 

    grid.x = -160;
    grid.z = i;
    startgrid[0] = calcX(grid);
    startgrid[1] = calcY(grid);
    grid.x = 160;
    endgrid[0] = calcX(grid);
    endgrid[1] = calcY(grid);

    lioglinb(liobuf, startgrid[0], startgrid[1], endgrid[0], endgrid[1], 1, 0);

  }

}

void drawlaunch(void)
{
  int i;

  char sl[] = "Launch platform";

  /* Coordinate of Launch platform */
  coord_t lcoord[4] = {{100,0,100},
                       {120,0,100},
                       {100,0,120},
                       {120,0,120}};

  int pointnum[4][2] = {{0,1},
                        {1,3},
                        {3,2},
                        {2,0}};

  for (i = 0; i < 4; i++){
    line_xyz(liobuf, lcoord[pointnum[i][0]], lcoord[pointnum[i][1]], 0);
  }

  for (i = 0; i < 15; i++){
    liogput2(liobuf, calcX(lcoord[3])+i*8, calcY(lcoord[3])+10, sl[i], 4);
  }

}

void drawvessel(coord_t scoord)
{

  liogcirc(liobuf, calcX(scoord), calcY(scoord), 2, 6);

}

coord_t calcscoord(coord_t scoord, coord_t vxyz, float t)
{
  coord_t nscoord;

  nscoord.x = vxyz.x * t + scoord.x;
  nscoord.y = vxyz.y * t - (GRAVITY * t * t) / 2 + scoord.y;
  nscoord.z = vxyz.z * t + scoord.z;

  return nscoord;
}

int checkvessel(coord_t *coord, coord_t nscoord)
{
  int i;

  char cc[23];
  char sc[23];

  int lenable;

  sprintf(cc, "(%6.1f,%6.1f,%6.1f)", coord[0].x, coord[0].y, coord[0].z);

  for (i = 0; i < 22; i++){
    liogput2(liobuf, 416+i*8, 80, cc[i], 4);
  }

  sprintf(sc, "Space vessel");

  for (i = 0; i < 12; i++){
    liogput2(liobuf, 416+i*8, 100, sc[i], 6);
  }

  sprintf(sc, "(%6.1f,%6.1f,%6.1f)", nscoord.x, nscoord.y, nscoord.z);

  for (i = 0; i < 22; i++){
    liogput2(liobuf, 416+i*8, 120, sc[i], 6);
  }

  if ((coord[0].x - RANGE < nscoord.x) && (coord[0].x + RANGE > nscoord.x) &&
      (coord[0].y - RANGE < nscoord.y) && (coord[0].y + RANGE > nscoord.y) &&
      (coord[0].z - RANGE < nscoord.x) && (coord[0].z + RANGE > nscoord.z)){
    printf("Success!\n");
    lenable = 0;
  }
  else {
    lenable = 1;
  }

  return lenable;
}

void drawaxis(void)
{
  int i;

  char st[] = "Space station";

  /* Coordinate of axis */
  coord_t ax = {160,0,0};
  coord_t ay = {0,160,0};
  coord_t az = {0,0,160};

  int endax[2];
  int enday[2];
  int endaz[2];

  endax[0] = calcX(ax);
  endax[1] = calcY(ax);
  enday[0] = calcX(ay);
  enday[1] = calcY(ay);
  endaz[0] = calcX(az);
  endaz[1] = calcY(az);

  liogput2(liobuf, endax[0]+10, endax[1]+10, 'x', 4);
  liogput2(liobuf, enday[0]+10, enday[1]+10, 'y', 4);
  liogput2(liobuf, endaz[0]+10, endaz[1]+10, 'z', 4);

  lioglinb(liobuf, X0, Y0, endax[0], endax[1], 4, 0);
  lioglinb(liobuf, X0, Y0, enday[0], enday[1], 4, 0);
  lioglinb(liobuf, X0, Y0, endaz[0], endaz[1], 4, 0);

  for (i = 0; i < 13; i++){
    liogput2(liobuf, enday[0]+100+i*8, enday[1]+20, st[i], 4);
  }

}
