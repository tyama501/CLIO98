/* GGET/GPUT1 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "CLIO98.H"

#define LIOSIZE 5200

#define GBUFMARGIN 8
#define NUMPLANE 4

#define DEBUG 0

char *lioaddr;
char far *liobuf;

int calgbuflen(int xupperl, int yupperl, int xlowerr, int ylowerr);

char _asm_1(char *);

/* DOS */
#define c_INT_21() _asm_1("\n\tINT\t33\n")
#define c_AH_08() _asm_1("\n\tMOV\tAH,8\n")
#define c_AH_0B() _asm_1("\n\tMOV\tAH,11\n")

int main(void)
{
  int i, j;
  int xul = 50;
  int yul = 100;
  int xlr = 113;
  int ylr = 163;

  char cdata[8][8] = {{0,0,0,0,3,0,0,0},
                      {3,0,0,3,3,3,0,3},
                      {3,0,0,3,3,3,0,3},
                      {3,0,3,3,3,3,0,3},
                      {3,3,7,3,3,7,3,3},
                      {0,3,3,6,6,3,3,0},
                      {0,0,3,3,3,3,0,0},
                      {0,0,0,3,3,0,0,0}};

  char c_key;

  int gbuflen;
  char *gbufaddr;
  char far *gbuf;

  lioaddr = (char *)calloc(LIOSIZE, sizeof(char));

  liobuf = (char far *)lioaddr;

  gbuflen = calgbuflen(xul, yul, xlr, ylr);

  gbufaddr = (char *)calloc(gbuflen, sizeof(char));

  gbuf = (char far *)gbufaddr;

#if DEBUG
  printf("%x\n", lioaddr);
  printf("%lx\n", liobuf);
  printf("%d\n", gbuflen);
  printf("%x\n", gbufaddr);
  printf("%lx\n", gbuf);
#endif

  lioset();
  lioginit(liobuf);
  liogscre(liobuf);
  liogview(liobuf);
  liogcol1(liobuf);
  liogcls(liobuf);

  printf("Press q to exit\n");

  printf("===== Get =====\n");

  for (j = 0; j < 8; j++){
    for (i = 0; i < 8; i++){
      liogline(liobuf, xul+i*8, yul+j*8, xul+i*8+7, yul+j*8+7, cdata[j][i]);
    }
  }
  lioglinb(liobuf, xul, yul, xul+63, yul+63, 1, 1);
  liogget(liobuf, xul, yul, xlr, ylr, gbuf, gbuflen);

  printf("===== Put =====\n");

  liogput1(liobuf, xul, yul+100, gbuf, gbuflen);
  liogput1(liobuf, xul+100, yul+100, gbuf, gbuflen);
  liogput1(liobuf, xul+200, yul+100, gbuf, gbuflen);

  while (1){

    c_AH_0B();
    if (c_INT_21()==0xFF) {
      c_AH_08();
      c_key = c_INT_21();
      if (c_key=='q' || c_key=='Q'){
       liogcls(liobuf);
       free(lioaddr);
       free(gbufaddr);
       exit(0);
      }
    }
  }

  free(lioaddr);
  free(gbufaddr);
  return 0;
}

int calgbuflen(int xupperl, int yupperl, int xlowerr, int ylowerr)
{
  int gbuflen;

  gbuflen = ((xlowerr-xupperl)/8 + 1) * (ylowerr-yupperl+1) * NUMPLANE + GBUFMARGIN;
  return gbuflen;
}