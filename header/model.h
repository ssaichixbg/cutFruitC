/*UI type*/
typedef enum UITYPE
{
	WELCOME_UI,
	GAME_UI
}UITYPE;
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
	BOMB
};
/*single object node*/
typedef struct OBJECT
{
	OBJECT *lastObj;
	OBJECT *nextObj;
	OBJTYPE type;    
	int x;
	int y;
	int rotation;	
	int rateX;
	int rateY;
	int rotationRate;
}OBJECT; 
/*event type*/
typedef enum EVENTTYPE
{
	GENERATE,
	DELETE,
	CUTTED,
	CLEAR_MOUSE_TRAIL
}
/*event node*/
typedef struct EVENT
{
	EVENTS *lastEvent;
	EVENTS *nextEvent;
	int timeToBreak;
	EVENTTYPE type;
	OBJECT *obj;
}EVENT;
/*mouse trail node*/
typedef struct MOUSEPOINT
{
	MOUSEPOINT *lastPoint;
	MOUSEPOINT *nextPoint;
	int x,
	int y
}MOUSEPOINT;
/*picture data*/
typedef struct PICTURE
{
	int x,
	int y,
	short int **data;
}PICTURE;


OBJECT *newObjectChain(void);
int addObject(OBJECT *newObj);
int deleteObject(OBJECT *obj);
int deleteAllObjects(OBJECT *objs);

EVENT *newEventChain(void);
int addEvent(EVENT *newEvent);
int deleteEvent(EVENT *event);
int deleteAllEvents(EVENT *events);

MOUSEPOINT *newMousePointChain(void);
int addPoint(MOUSEPOINT *newPoint);
int pointsLength(MOUSEPOINT *points)
int deletePoint(MOUSEPOINT *point);
int deleteAllPoints(MOUSEPOINT *points);
