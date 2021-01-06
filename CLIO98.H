/* CLIO98.H */

extern void lioset(void);
extern void lioginit(char far *liobuf);
extern void liogscre(char far *liobuf);
extern void liogview(char far *liobuf);
extern void liogcol1(char far *liobuf);
extern void liogcol2(char far *liobuf, int palnum, int color);
extern void liogcls(void);
extern void liogcirc(char far *liobuf, int xcenter, int ycenter, int radius, int palnum);
extern void liogline(char far *liobuf, int xstart, int ystart, int xend, int yend, int palnum);
extern void liogput2(char far *liobuf, int xupperl, int yupperl, int jiscode, int fgcol);