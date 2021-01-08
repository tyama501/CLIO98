/* Flying jis 0x416b */
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
  int i;
  int alive[7];
  int cx[7];
  int cy[7];
  int dx[7];
  int dy[7];

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

  for (i = 0; i < 7; i++){
    alive[i] = 0;
  }

  printf("Press q to exit\n");

  while (1){
    for (i = 0; i < 7; i++){
      if (!alive[i]){
        alive[i] = 1;
        cx[i] = 312;
        cy[i] = 192;
        if (i%2){
          dx[i] = -(rand() >> 10);
          dy[i] = -(rand() >> 10);
        }
        else {
          dx[i] = (rand() >> 10);
          dy[i] = (rand() >> 10);
        } 
        break;
      }
    }

    for (i = 0; i < 7; i++){
      if (alive[i]){
        liogput2(liobuf, cx[i], cy[i], 0x2121, i+1);
        cx[i] += dx[i];
        cy[i] += dy[i];
        if ((cx[i] < 0) || (cx[i] > 639) || (cy[i] < 0) || (cy[i] > 399)){
          alive[i] = 0;
        }
        else {
          liogput2(liobuf, cx[i], cy[i], 0x416b, i+1);
        }
      }
    }

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
