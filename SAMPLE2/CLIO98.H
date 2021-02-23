/* CLIO98.H */

#ifndef CLIO98_H
#define CLIO98_H

extern void lioset(void);
extern void lioginit(char far *liobuf);
extern void liogscre(char far *liobuf);
extern void liogview(char far *liobuf);
extern void liogcol1(char far *liobuf);
extern void liogcol2(char far *liobuf, int palnum, int color);
extern void liogcls(char far *liobuf);
extern void liogcirc(char far *liobuf, int xcenter, int ycenter, int radius, int palnum);
extern void liogpset(char far *liobuf, int xpset, int ypset, int palnum);
extern void liogline(char far *liobuf, int xstart, int ystart, int xend, int yend, int palnum);
extern void lioglinb(char far *liobuf, int xstart, int ystart, int xend, int yend, int palnum, int boxnum);
extern void liogget(char far *liobuf, int xupperl, int yupperl, int xlowerr, int ylowerr, char far *gbuf, int gbuflen);
extern void liogput1(char far *liobuf, int xupperl, int yupperl, char far *gbuf, int gbuflen);
extern void liogput2(char far *liobuf, int xupperl, int yupperl, int jiscode, int fgcol);
extern char liogpoin(char far *liobuf, int xpoint, int ypoint);

#endif
