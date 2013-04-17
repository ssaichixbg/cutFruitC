#include <stdio.h>
#include <stdlib.h>
#include <bios.h>
#include <graphics.h>

##ifndef G
#define G 9
#endif
void init(void);
void putPixl(int x,int y,int color);
PICTURE *readPic(FILE *file);

void drawBackground(void);
void drawObject(OBJECT *obj);
void drawPic(int x,int y,int rotation,FILE *file);
void drawPoint(int x,int y,int point);

void calPosition(OBJECT *obj,short int second);
