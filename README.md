# CLIO98

PC-98のLIOの一部をLSI C-86 試食版で使えるようにしてみました。

SAMPLEはNeko Project 21/WとPC-9801RX21(実機)で試しています。

The aim for this repository is to use LIO of NEC PC-98 (Japanese computer in 80's and 90's)
with LSI C-86 compiler.

## Usage
#include "CLIO98.H"

## SAMPLES
SAMPLE/CLIOTST.C  
Draw circles and boxes.  
Put Kanji.  
<img src=IMAGE/CLIOTST_NP21W_3.png width="320pix">  
Executed on Neko Project 21/W emulator.  

SAMPLE2/CLIOTST2.C  
Flying jis 0x416b.

SAMPLE3/CLIOTST3.C  
Example of using GGET and GPUT.  
<img src=IMAGE/CLIOTST3_NP21W.png width="320pix">  

SAMPLE4/CLIOTST4.C  
Example to read the pallet number on the screen.  

SAMPLE5/CLIOTST5.C  
Example of rotating cube around y-axis.  
SAMPLE6/CLIOTST6.C  
Mini Game of docking Space vessel to Space Station.  
<img src=IMAGE/CLIOTST_NP21W_5.png width="320pix">
<img src=IMAGE/CLIOTST6_NP21W.png width="320pix">

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

### GPSET
    void liogpset(char far *liobuf, int xpset, int ypset, int palnum);
Draw point.  
xpset : x coordinate of the point  
ypset : y coordinate of the point  
palnum : Pallet number  

### GLINE
    void liogline(char far *liobuf, int xstart, int ystart, int xend, int yend, int palnum);
    void lioglinb(char far *liobuf, int xstart, int ystart, int xend, int yend, int palnum, int boxnum);
Draw line.  
liogline() draws a box filled with color.
lioglinb() has the parameter to select line, box, or filled box.    
xstart : x coordinate for start  
ystart : y coordinate for start  
xend : x coordinate for end  
yend : y coordinate for end  
palnum : Pallet number  
boxnum : box parameter (0:line, 1:box, 2:filled box)  

### GGET
    void liogget(char far *liobuf, int xupperl, int yupperl, int xlowerr, int ylowerr, char far *gbuf, int gbuflen);
Get graphic image to memory.  
Allocate memory and give far pointer.  
Please refer SAMPLE3.  
xupperl : x coordinate for upper left of the image.  
yupperl : y coordinate for upper left of the image.  
xlowerr : x coordinate for lower right of the image.  
ylowerr : y coordinate for lower right of the image.  
gbuf : far pointer of the memory.  
gbuflen : length of the memory.  

### GPUT1
    void liogput1(char far *liobuf, int xupperl, int yupperl, char far *gbuf, int gbuflen);
Put graphic image of the memory.  
Please refer SAMPLE3.  
xupperl : x coordinate for upper left to put the image.  
yupperl : y coordinate for upper left to put the image.  
gbuf : far pointer of the memory.  
gbuflen : length of the memory. 

### GPUT2
    void liogput2(char far *liobuf, int xupperl, int yupperl, int jiscode, int fgcol);
Put character of jis code.  
xupperl : x coordinate for upper left  
yupperl : y coordinate for upper left  
jiscode : jis code  
fgcol : Pallet number for foreground color  
  
### GPOINT
    char liogpoin(char far *liobuf, int xpoint, int ypoint);
Read pallet number.  
xpoint : x coordinate to read  
ypoint : y coordinate to read  
return value : pallet number  
