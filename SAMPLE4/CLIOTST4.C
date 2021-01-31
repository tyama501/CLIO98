/* GPOINT */
#include <stdio.h>
#include <stdlib.h>

#include "CLIO98.H"

#define LIOSIZE 5200

#define DEBUG 0

char *lioaddr;
char far *liobuf;

char _asm_1(char *);

/* DOS */
#define c_INT_21() _asm_1("\n\tINT\t33\n")
#define c_AH_08() _asm_1("\n\tMOV\tAH,8\n")
#define c_AH_0B() _asm_1("\n\tMOV\tAH,11\n")

int main(void)
{
  int i, j;
  int xul = 200;
  int yul = 100;

  char cdata[8][8] = {{1,1,1,1,3,1,1,1},
                      {3,1,1,3,3,3,1,3},
                      {3,1,1,3,3,3,1,3},
                      {3,1,3,3,3,3,1,3},
                      {3,3,7,3,3,7,3,3},
                      {1,3,3,6,6,3,3,1},
                      {1,1,3,3,3,3,1,1},
                      {1,1,1,3,3,1,1,1}};

  char palnum;

  char c_key;

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
  liogcls(liobuf);

  printf("Press q to exit\n");

  for (j = 0; j < 8; j++){
    for (i = 0; i < 8; i++){
      liogline(liobuf, xul+i*8, yul+j*8, xul+i*8+7, yul+j*8+7, cdata[j][i]);
    }
  }

  printf("===== Read Pallet Number =====\n");

  for (j = 0; j < 8; j++){
    for (i = 0; i < 8; i++){
      palnum = liogpoin(liobuf, xul+i*8, yul+j*8);
      printf("%d,", palnum);
    }
    printf("\n");
  }

  while (1){

    c_AH_0B();
    if (c_INT_21()==0xFF) {
      c_AH_08();
      c_key = c_INT_21();
      if (c_key=='q' || c_key=='Q'){
       liogcls(liobuf);
       free(lioaddr);
       exit(0);
      }
    }
  }

  free(lioaddr);
  return 0;
}
