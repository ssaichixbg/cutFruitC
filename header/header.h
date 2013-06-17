#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <bios.h>
#ifndef _HEADER_H_
#define _HEADER_H_ 
/*Data Structure*/

/*UI type*/
typedef enum UITYPE
{
	WELCOME_UI,
	GAME_UI
} UITYPE;
/*the type of object*/
typedef enum OBJTYPE
{
	BANANA_NORMAL,
	BANANA_NORMAL_LEFT,
	BANANA_NORMAL_RIGHT,
	BANANA_COLD,
	BANANA_COLD_LEFT,
	BANANA_COLD_RIGHT,
	BANANA_HOT,
	BANANA_HOT_LEFT,
	BANANA_HOT_RIGHT,
	BANANA_X2,
	BANANA_X2_LEFT,
	BANANA_X2_RIGHT,
	ORANGE,
	ORANGE_LEFT,
	ORANGE_RIGHT,
	APPLE,
	APPLE_LEFT,
	APPLE_RIGHT,
	BOMB,
	NONE
}OBJTYPE;
/*single object node*/
typedef struct OBJECT
{
	struct OBJECT *lastObj;
	struct OBJECT *nextObj;
	OBJTYPE type;
	int x;
	int y;
	int rotation;
	int rateX;
	int rateY;
	int rotationRate;
	int old_x;
	int old_y;
	unsigned char old_byte[40][40];
}OBJECT;
/*event type*/
typedef enum EVENTTYPE
{
	GENERATE,
	DELETE,
	CUTTED,
	CLEAR_MOUSE_TRAIL
}EVENTTYPE;
/*event node*/
typedef struct EVENT
{
	struct EVENT *lastEvent;
	struct EVENT *nextEvent;
	int frameToBreak;
	EVENTTYPE type;
	OBJECT *obj;
}EVENT;
/*mouse trail node*/
typedef struct MOUSEPOINT
{
	struct MOUSEPOINT *lastPoint;
	struct MOUSEPOINT *nextPoint;
	int x;
	int y;
	unsigned char old_color;
}MOUSEPOINT;
/*picture data*/
typedef struct PICTURE
{
	int x;
	int y;
	short int **data;
}PICTURE;



/*global variables*/
extern int FRAME;
extern short int OVER;
extern short int USER_POINT;/*user game point*/
extern short int FPS;/*game rate*/
extern short int TIME;/*game */
extern short int UNCUTTED_FRUIT;/*fruit not be cutted*/
extern short int GENERATED_FRUIT[5];/*0-- 1-- 2-- 3-- 4-- 5-- 6--*/
extern enum UITYPE CURRENT_UI;
extern struct OBJECT *OBJECTS;
extern struct EVENT *EVENTS;
extern struct MOUSEPOINT *MOUSE_TRIAL;
extern const short int CLOCK_F;
extern const short int G;
/*----------------------function defination----------------------*/

/*----------------------main.c----------------------*/
extern int main(int argc, char const *argv[]);/*program system main function*/
extern void init(void);/*initialization*/
extern void cfexit(void);/*exit program*/
extern void circulation(void);/*game main circulation function*/
extern void welcomeUI(void);/*display welcome UI*/
extern void gameUI(void);/*display game UI*/


/*----------------------cfgraphics.c----------------------*/
extern void drawBackground(void);
extern void drawObject(OBJECT *obj);
extern void drawPic(OBJECT *obj,unsigned char pic[][]);
extern void clearPic(OBJECT *obj);
extern void drawPoint(int x,int y,int radious,unsigned char color);
extern unsigned char getPixl(int x,int y);
extern void calPosition(void);


/*----------------------model.c----------------------*/
extern OBJECT *newObj(int x,int y,int r,int rx,int ry,int rR,OBJTYPE type);
extern OBJECT *newObjectChain(void);
extern int addObject(OBJECT *objs,OBJECT *newObj);
extern OBJECT * deleteObject(OBJECT *objs,OBJECT *obj);
extern int deleteAllObjects(OBJECT *objs);

extern EVENT *newEVENT(int frameToBreak,EVENTTYPE type,OBJECT *obj);
extern EVENT *newEventChain(void);
extern int addEvent(EVENT *events,EVENT *newEvent);
extern EVENT * deleteEvent(EVENT *events,EVENT *event);
extern int deleteAllEvents(EVENT *events);

extern MOUSEPOINT *newMousePoint(int x,int y);
extern MOUSEPOINT *newMousePointChain(void);
extern int addPoint(MOUSEPOINT *points,MOUSEPOINT *newPoint);
extern int pointsLength(MOUSEPOINT *points);
extern int deleteAllPoints(MOUSEPOINT *points);


/*----------------------logic.h----------------------*/
extern void mousePressed(int x,int y);
extern void drawMouseTrail(MOUSEPOINT *point);
extern void clearMouseTrail(void);

extern void processEvents(void);
extern void buildEvent(void);
extern void drawScreen(void);

extern void bombCutted(OBJECT *bomb);
extern void normalFruitCutted(OBJECT *frt);
extern void coldBananaCutted(OBJECT *coldBanana);
extern void hotBananaCutted(OBJECT *hotBanana);
extern void X2BananaCutted(OBJECT *X2banana);
/*----------------------IO.h----------------------*/
extern void get_mouse_status(int *px, int *py, int *pbutton);
#endif