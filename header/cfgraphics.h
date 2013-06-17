#include "CF\refere~1\vesa8bit.h"
#include "CF\header\header.h"
#include "CF\header\data.h"
/*----------------------cfgraphics.c----------------------*/
void drawBackground(void);
void drawObject(OBJECT *obj);
void drawPic(OBJECT *obj,unsigned char pic[][]);
void clearPic(OBJECT *obj);
void drawPoint(int x,int y,int radious,unsigned char color);
unsigned char getPixl(int x,int y);
void calPosition(void);

void drawBackground(void)
{
	display_8bit_bmp("CF\\bgd.bmp");
	return;
}
void drawObject(OBJECT *obj)
{
	switch(obj->type)
	{
		case BANANA_NORMAL:
			clearPic(obj);
			drawPic(obj,BANANA_NORMAL_BYTE);
		break;
	}
	
}
void drawPic(OBJECT *obj,unsigned char pic[][])
{
	int i,j,x,y;
	x=obj->x;
	y=obj->y;
	obj->old_x=x;
	obj->old_y=y;
	for(i=0;i<40;i++)
	{
		for(j=0;j<40;j++)
		{
			int sx,sy;
			sx=x+i;
			sy=y+j;
			if((sx)>1023||(sx)<0||(sy)>767||(sy)<0)
				continue;
			obj->old_byte[i][j]=vesa_getpixel(sx,sy);
			vesa_putpixel(sx,sy,14);
		}
	}
}
void clearPic(OBJECT *obj)
{
	int i,j,x,y;
	x=obj->old_x;
	y=obj->old_y;
	if(x==65530) return;
	for(i=0;i<40;i++)
	{
		for(j=0;j<40;j++)
		{
			int sx,sy;
			sx=x+i;
			sy=y+j;
			if((sx)>1023||(sx)<0||(sy)>767||(sy)<0)
				continue;
			vesa_putpixel(sx,sy,obj->old_byte[i][j]);
		}
	}
}
void drawPoint(int x,int y,int radious,unsigned char color)
{
	if(radious == 0)
	{
		vesa_putpixel(x,y,color);
	}
}
unsigned char getPixl(int x,int y)
{
	return vesa_getpixel(x,y);
}
void calPosition(void)
{
	float t = 1.0/CLOCK_F*FPS/CLOCK_F;
	OBJECT *cur_obj = OBJECTS->nextObj;
	while(cur_obj != NULL)
	{
		cur_obj->x += (int)(cur_obj->rateX*t);
		cur_obj->y += (int)(cur_obj->rateY*t);
		cur_obj->rotation += (int)(cur_obj->rotationRate*t);
		cur_obj->rateY += G*t;
		cur_obj = cur_obj->nextObj;
	}
}
