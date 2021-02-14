/* Rotate Cube */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "CLIO98.H"

#define LIOSIZE 5200

#define DEBUG 0

#define X0 320
#define Y0 200

#define SPEED 16

char *lioaddr;
char far *liobuf;

char _asm_1(char *);

/* DOS */
#define c_INT_21() _asm_1("\n\tINT\t33\n")
#define c_AH_08() _asm_1("\n\tMOV\tAH,8\n")
#define c_AH_0B() _asm_1("\n\tMOV\tAH,11\n")

typedef struct {
  float x;
  float y;
  float z;
} coord_t;

void plotxyz(coord_t coord);
void line_xyz(coord_t start_c, coord_t end_c, int erase);

int calcX(coord_t coord);
int calcY(coord_t coord);

int main(void)
{
  int i, j;

  /* Coordinate of cube */
  coord_t coord[8] = {{-10,-10,-10},
                      {10,-10,-10},
                      {-10,10,-10},
                      {10,10,-10},
                      {-10,-10,10},
                      {10,-10,10},
                      {-10,10,10},
                      {10,10,10}};

  coord_t pcoord[8];

  /* Coordinate of axis */
  coord_t ax = {160,0,0};
  coord_t ay = {0,160,0};
  coord_t az = {0,0,160};

  int endax[2];
  int enday[2];
  int endaz[2];

  /* Matrix */
  float pm[4][4] = {{1,0,0,50},
                    {0,1,0,50},
                    {0,0,1,50},
                    {0,0,0,1}};

  float rm[4][4] = {{1,0,0,0},
                    {0,1,0,0},
                    {0,0,1,0},
                    {0,0,0,1}};

  float xyzf[3];
  float xyzfn[3];
  float radf = PI / SPEED;

  /* Point of cube to connect */
  int pointnum[12][2] = {{0,1},
                         {1,3},
                         {3,2},
                         {2,0},
                         {4,5},
                         {5,7},
                         {7,6},
                         {6,4},
                         {0,4},
                         {1,5},
                         {2,6},
                         {3,7}};

  char c_key;

  lioaddr = (char *)calloc(LIOSIZE, sizeof(char));

  liobuf = (char far *)lioaddr;

  lioset();
  lioginit(liobuf);
  liogscre(liobuf);
  liogview(liobuf);
  liogcol1(liobuf);
  liogcls(liobuf);

  printf("Press q to exit\n");

  /* Parallel move */
  for (i = 0; i < 8; i++){
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

  endax[0] = calcX(ax);
  endax[1] = calcY(ax);
  enday[0] = calcX(ay);
  enday[1] = calcY(ay);
  endaz[0] = calcX(az);
  endaz[1] = calcY(az);

  liogput2(liobuf, endax[0]+10, endax[1]+10, 'x', 4);
  liogput2(liobuf, enday[0]+10, enday[1]+10, 'y', 4);
  liogput2(liobuf, endaz[0]+10, endaz[1]+10, 'z', 4);

  while (1){

    /* Draw cube */
    for (i = 0; i < 12; i++){
      line_xyz(coord[pointnum[i][0]], coord[pointnum[i][1]], 0);
    }

    for (i = 0; i < 8; i++){
      plotxyz(coord[i]);
    }

    /* Draw axis */
    lioglinb(liobuf, X0, Y0, endax[0], endax[1], 4, 0);
    lioglinb(liobuf, X0, Y0, enday[0], enday[1], 4, 0);
    lioglinb(liobuf, X0, Y0, endaz[0], endaz[1], 4, 0);

    /* Rotate */
    for (i = 0; i < 8; i++){

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
    }

    /* Erase cube */
    for (i = 0; i < 12; i++){
      line_xyz(pcoord[pointnum[i][0]], pcoord[pointnum[i][1]], 1);
    }

  }

  free(lioaddr);
  return 0;
}

void plotxyz(coord_t coord)
{
  int x;
  int y;

  x = calcX(coord);
  y = calcY(coord);

#if DEBUG
  printf("%f, %f, %f, %d, %d\n", coord.x, coord.y, coord.z, x, y);
#endif
  liogpset(liobuf, x, y, 6);
}

void line_xyz(coord_t start_c, coord_t end_c, int erase)
{
  int start_x;
  int start_y;
  int end_x;
  int end_y;

  start_x = calcX(start_c);
  start_y = calcY(start_c);
  end_x = calcX(end_c);
  end_y = calcY(end_c);

  if (erase){
    lioglinb(liobuf, start_x, start_y, end_x, end_y, 0, 0);
  }
  else if (start_c.z < 0){
    lioglinb(liobuf, start_x, start_y, end_x, end_y, 10, 0);
  }
  else {
    lioglinb(liobuf, start_x, start_y, end_x, end_y, 2, 0);
  }

}

int calcX(coord_t coord)
{
  float x;

  x = X0 + coord.x - coord.z / 1.41421356;

  return (int)x;
}

int calcY(coord_t coord)
{
  float y;

  y = Y0 - coord.y + coord.z / 1.41421356;

  return (int)y;
}
