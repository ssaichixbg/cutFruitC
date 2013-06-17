#include "CF\header\header.h"

OBJECT *newObj(int x,int y,int r,int rx,int ry,int rR,OBJTYPE type);
OBJECT *newObjectChain(void);
int addObject(OBJECT *objs,OBJECT *newObj);
OBJECT * deleteObject(OBJECT *objs,OBJECT *obj);
int deleteAllObjects(OBJECT *objs);

EVENT *newEVENT(int frameToBreak,EVENTTYPE type,OBJECT *obj);
EVENT *newEventChain(void);
int addEvent(EVENT *events,EVENT *newEvent);
EVENT * deleteEvent(EVENT *events,EVENT *event);
int deleteAllEvents(EVENT *events);

MOUSEPOINT *newMousePoint(int x,int y);
MOUSEPOINT *newMousePointChain(void);
int addPoint(MOUSEPOINT *points,MOUSEPOINT *newPoint);
int pointsLength(MOUSEPOINT *points);
int deleteAllPoints(MOUSEPOINT *points);




OBJECT *newObj(int x,int y,int r,int rx,int ry,int rR,OBJTYPE type)
{
	OBJECT *newObj = (OBJECT *)malloc(sizeof(OBJECT));
	newObj->x=x;
	newObj->y=y;
	newObj->rotation=r;
	newObj->rateX=rx;
	newObj->rateY=ry;
	newObj->rotationRate=rR;
	newObj->type=type;
	newObj->old_x=65530;
	newObj->old_y=65530;
	return newObj;
}
OBJECT *newObjectChain(void)
{
	OBJECT *newObj = (OBJECT *)malloc(sizeof(OBJECT));	
	newObj->lastObj = NULL;
	newObj->nextObj = NULL;
	return newObj;
}
int addObject(OBJECT *objs,OBJECT *newObj)
{
	OBJECT *cur_obj = objs;
	while(cur_obj->nextObj != NULL)
	{
		cur_obj = cur_obj->nextObj;
	}
	newObj->lastObj = cur_obj;
	newObj->nextObj = NULL;
	cur_obj->nextObj = newObj;
	return 1;
}
OBJECT *deleteObject(OBJECT *objs,OBJECT *obj)
{
	OBJECT *cur_obj = objs->nextObj;
	while(cur_obj != obj && cur_obj != NULL)
	{
		cur_obj = cur_obj->nextObj;
	}
	if(cur_obj != obj ) return objs;
	if(cur_obj->nextObj != NULL)
	{
		cur_obj->lastObj->nextObj=cur_obj->nextObj;
		cur_obj->nextObj->lastObj=cur_obj->lastObj;
	}
	else
	{
		cur_obj->lastObj->nextObj=NULL;
	}
	free(cur_obj);
	return objs;

}
int deleteAllObjects(OBJECT *objs)
{
	OBJECT *cur_obj = objs;
	while(cur_obj->nextObj != NULL)
	{
		cur_obj = cur_obj->nextObj;
	}
	while(cur_obj->lastObj != NULL)
	{
		cur_obj=cur_obj->lastObj;
		free(cur_obj->nextObj);
	}

	return 1;
}


EVENT *newEvent(int frameToBreak,EVENTTYPE type,OBJECT *obj)
{
	EVENT *newEvent = (EVENT *)malloc(sizeof(EVENT));
	newEvent->frameToBreak=frameToBreak;
	newEvent->type=type;
	newEvent->obj=obj;

	return newEvent;
}
EVENT *newEventChain(void)
{
	EVENT *newEVT = (EVENT *)malloc(sizeof(EVENT));
	newEVT->lastEvent = NULL;
	newEVT->nextEvent = NULL;
	return newEVT;
}
int addEvent(EVENT *events,EVENT *newEvent)
{
	EVENT *cur_evt = events;
	while(cur_evt->nextEvent != NULL)
	{
		cur_evt = cur_evt->nextEvent;
	}
	newEvent->lastEvent = cur_evt;
	newEvent->nextEvent = NULL;
	cur_evt->nextEvent = newEvent;
	return 1;
}
EVENT * deleteEvent(EVENT *events,EVENT *event){
	EVENT *cur_evt = events->nextEvent;
	while(cur_evt != event && cur_evt != NULL)
	{
		cur_evt = cur_evt->nextEvent;
	}
	if(cur_evt != event ) return events;
	if(cur_evt->nextEvent != NULL)
	{
		cur_evt->lastEvent->nextEvent=cur_evt->nextEvent;
		cur_evt->nextEvent->lastEvent=cur_evt->lastEvent;
	}
	else
	{
		cur_evt->lastEvent->nextEvent=NULL;
	}
	free(cur_evt);
	return events;
}
int deleteAllEvents(EVENT *events){
	EVENT *cur_evt = events;
	while(cur_evt->nextEvent != NULL)
	{
		cur_evt = cur_evt->nextEvent;
	}
	while(cur_evt->lastEvent != NULL)
	{
		cur_evt=cur_evt->lastEvent;
		free(cur_evt->nextEvent);
	}

	return 1;
}

MOUSEPOINT *newMousePoint(int x,int y)
{
	MOUSEPOINT *newPoint = (MOUSEPOINT *)malloc(sizeof(MOUSEPOINT));
	newPoint->x=x;
	newPoint->y=y;

	return newPoint;
}
MOUSEPOINT *newMousePointChain(void)
{
	MOUSEPOINT *newPoint = (MOUSEPOINT *)malloc(sizeof(MOUSEPOINT));
	newPoint->lastPoint=NULL;
	newPoint->nextPoint=NULL;

	return newPoint;
}
int addPoint(MOUSEPOINT *points,MOUSEPOINT *newPoint)
{
	MOUSEPOINT *cur_point = points;
	while(cur_point->nextPoint != NULL)
	{
		cur_point = cur_point->nextPoint;
	}
	newPoint->lastPoint = cur_point;
	newPoint->nextPoint = NULL;
	cur_point->nextPoint = newPoint;
	return 1;
}
int pointsLength(MOUSEPOINT *points)
{
	int len=0;
	MOUSEPOINT *cur_point = points;
	while(cur_point->nextPoint != NULL)
	{
		len++;
		cur_point = cur_point->nextPoint;
	}
	return len;
}
int deleteAllPoints(MOUSEPOINT *points)
{
	MOUSEPOINT *cur_point = points->nextPoint;
	while(cur_point->nextPoint != NULL)
	{
		cur_point = cur_point->nextPoint;
	}
	while(cur_point->lastPoint != NULL)
	{
		cur_point=cur_point->lastPoint;
		free(cur_point->nextPoint);
	}

	return 1;
}