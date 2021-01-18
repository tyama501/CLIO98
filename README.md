# CLIO98

PC-98のLIOの一部をLSI C-86 試食版で使えるようにしてみました。

SAMPLEはNeko Project 21/WとPC-9801RX21(実機)で試しています。

The aim for this repository is to use LIO of NEC PC-98 (Japanese computer in 80's and 90's)
with LSI C-86 compiler.

## Usage
#include "CLIO98.H"

## Function

### Setting
    void lioset(void);
Setting interrupt vectors for LIO. 
Do this at the beginning.

### GINIT
    void lioginit(char far *liobuf);
Allocate memory and give far pointer for the initialization.
Please refer SAMPLE. 

### GSCREEN
    void liogscre(char far *liobuf);
Set screen for color and 640x400.

### GVIEW
    void liogview(char far *liobuf);
Set view range for 640x400.

### GCOLOR1
    void liogcol1(char far *liobuf);
Set 16 color mode.    

### GCOLOR2
    void liogcol2(char far *liobuf, int palnum, int color);
Set color pallet.  
palnum : Pallet number  
color  : Color for the pallet  

### GCLS
    void liogcls(char far *liobuf);
Clear screen.    

### GCIRCLE
    void liogcirc(char far *liobuf, int xcenter, int ycenter, int radius, int palnum);
Draw circle.  
xcenter : x coordinate of the center  
ycenter : y coordinate of the center  
radius : Radius  
palnum : Pallet number  

### GLINE
    void liogline(char far *liobuf, int xstart, int ystart, int xend, int yend, int palnum);
Draw line.  
For now, a box filled with color is drawn.   
The box and the filling parameter will be added later.  
xstart : x coordinate for start  
ystart : y coordinate for start  
xend : x coordinate for end  
yend : y coordinate for end  
palnum : Pallet number  

### GPUT2
    void liogput2(char far *liobuf, int xupperl, int yupperl, int jiscode, int fgcol);
Put character of jis code.  
xupperl : x coordinate for upper left  
yupperl : y coordinate for upper left  
jiscode : jis code  
fgcol : Pallet number for foreground color  

## SAMPLES
SAMPLE/CLIOTST.C  
Draw circles and boxes.  
Put Kanji.  

SAMPLE/CLIOTST2.C  
Flying jis 0x416b.  


