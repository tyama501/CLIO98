#include "CLIO98.H"
#include "XYZ3D.H"

void plotxyz(char far *liobuf, coord_t coord)
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

void line_xyz(char far *liobuf, coord_t start_c, coord_t end_c, int erase)
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
