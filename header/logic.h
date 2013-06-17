#include "CF\header\header.h"
/*----------------------logic.h----------------------*/
void mousePressed(int x,int y);
void drawMouseTrail(MOUSEPOINT *point);
void clearMouseTrail(void);

void processEvents(void);
void buildEvent(void);
void drawScreen(void);

void bombCutted(OBJECT *bomb);
void normalFruitCutted(OBJECT *frt);
void coldBananaCutted(OBJECT *coldBanana);
void hotBananaCutted(OBJECT *hotBanana);
void X2BananaCutted(OBJECT *X2banana);


void mousePressed(int x,int y)
{
	if(pointsLength(MOUSE_TRIAL)<50)
	{
		MOUSEPOINT *point= newMousePoint(x,y);
		addPoint(MOUSE_TRIAL,point);
		drawMouseTrail(point);
	}
}
void drawMouseTrail(MOUSEPOINT *point)
{
	point->old_color=getPixl(point->x,point->y);
	drawPoint(point->x,point->y,0,1);
}
void clearMouseTrail(void)
{
	MOUSEPOINT *cur_point = MOUSE_TRIAL->nextPoint;
	while(cur_point != NULL)
	{
		drawPoint(cur_point->x,cur_point->y,0,cur_point->old_color);
		cur_point = cur_point->nextPoint;
	}
	deleteAllPoints(MOUSE_TRIAL);
}
void processEvents(void)
{
	EVENT *cur_evt=EVENTS->nextEvent;
	while(cur_evt != NULL)
	{
		cur_evt->frameToBreak --;
		if(!cur_evt->frameToBreak)
		{
			switch(cur_evt->type)
			{
				case GENERATE:
					addObject(OBJECTS,cur_evt->obj);
				case CUTTED:
				case CLEAR_MOUSE_TRAIL:
					break;
			}
		}
		cur_evt = cur_evt->nextEvent;
	}
	
	return;
}
void buildEvent(void)
{
	/*control mouse*/
	int x,y,btn;
	get_mouse_status(&x,&y,&btn);
	if(btn & 1)
	{
		mousePressed(x,y);
	}
	if (GENERATED_FRUIT[0]==0)
	{
		randomize();
		addEvent(EVENTS,newEvent(80,GENERATE,(OBJECT *)newObj(300+random(300),768,0,-20+random(40),-100+random(30),20,BANANA_NORMAL)));
		addEvent(EVENTS,newEvent(120,GENERATE,(OBJECT *)newObj(300+random(300),768,0,-20+random(40),-100+random(30),20,BANANA_NORMAL)));
		GENERATED_FRUIT[0] += 2;
	}
	return;
}
void drawScreen(void)
{
	OBJECT *cur_obj = OBJECTS->nextObj;
	while(cur_obj != NULL)
	{
		int x,y;
		x = cur_obj->x;
		y = cur_obj->y;
		if(x>1024||x<-41||y>768||y<-41)
		{
			OBJECT *temp = cur_obj;
			cur_obj = cur_obj->nextObj;
			switch(temp->type)
			{
				case BANANA_NORMAL:
				GENERATED_FRUIT[0]--;
				break;
			}
			deleteObject(OBJECTS,temp);
			continue;
		}
		drawObject(cur_obj);
		cur_obj = cur_obj->nextObj;
	}
}
void bombCutted(OBJECT *bomb)
{

}
void normalFruitCutted(OBJECT *frt)
{
	int x,y,rateY;
	OBJTYPE type;
	x=frt->x;
	y=frt->y;
	type=frt->type;

	randomize();
	rateY=-50+random(100);
	deleteObject(OBJECTS,frt);
	addEvent(EVENTS,newEvent(1,GENERATE,(OBJECT *)newObj(x,y,0,-300,rateY,20,BANANA_NORMAL_LEFT)));
	addEvent(EVENTS,newEvent(1,GENERATE,(OBJECT *)newObj(x+10,y,0,300,-rateY,20,BANANA_NORMAL_RIGHT)));
	GENERATED_FRUIT[0]--;
}
void coldBananaCutted(OBJECT *coldBanana)
{

}
void hotBananaCutted(OBJECT *hotBanana)
{

}
void X2BananaCutted(OBJECT *X2banana)
{

}
