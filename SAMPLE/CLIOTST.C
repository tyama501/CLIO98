#include <stdio.h>
#include <malloc.h>

#include "CLIO98.H"

#define LIOSIZE 5200

#define DEBUG 0

char *lioaddr;
char far *liobuf;

int main(void)
{
  int i;
  int col;

  lioaddr = (char *)calloc(LIOSIZE, sizeof(char));

  liobuf = (char far *)lioaddr;

#if DEBUG
  printf("%x\n", lioaddr);
  printf("%lx\n", liobuf);
#endif

  lioset();
  lioginit(liobuf);
  liogscre(liobuf);
  liogview(liobuf);
  liogcol1(liobuf);

  col = 0x002;
  for (i = 8; i <= 15; i++){
    col = (col << 1);
    liogcol2(liobuf, i, col);
  }

  liogcls();

  for (i = 1; i <= 7; i++){
    liogcirc(liobuf, 256+i*16, 100, 20, i);
  }

  for (i = 0; i <= 7; i++){
    liogline(liobuf, 256+i*16, 200, 256+(i+1)*16, 230, i+8);
  }

  liogput2(liobuf, 300, 300, 0x3441, 4);
  liogput2(liobuf, 340, 300, 0x3b7a, 4);

  free(lioaddr);
  return 0;
}
