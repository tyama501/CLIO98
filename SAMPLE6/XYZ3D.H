#ifndef XYZ3D_H
#define XYZ3D_H

#define X0 320
#define Y0 200

typedef struct {
  float x;
  float y;
  float z;
} coord_t;

void plotxyz(char far *liobuf, coord_t coord);
void line_xyz(char far *liobuf, coord_t start_c, coord_t end_c, int erase);

int calcX(coord_t coord);
int calcY(coord_t coord);

#endif
