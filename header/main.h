#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "IO.h"
#include "logic.h"
#include "sound.h"


##ifndef FPS_HIGH
#define FPS_HIGH 60
#define FPS_NORMAL 30 
#define FPS_LOW 10
#define FPS_EXLOW 2

#define MOUSE_TRAIL_RADIUS 10
#define MOUSE_TRALI_MAC_LENGTH 200
#endif

/*global variable*/
short int USER_POINT;
short int FPS;
short int TIME;
short int UNCUTTED_FRUIT;
short int GENERATED_FRUIT[5];/*0-- 1-- 2-- 3-- 4-- 5-- 6--*/

UITYPE CURRENT_UI;
OBJECT *OBJECTS;
EVENT *EVENTS;
MOUSEPOINT *MOUSE_TRIAL;

int main(int argc, char const *argv[]);
void init(void);
void exit(void);
void circulation(void);
void welcomeUI(void);
void gameUI(void);
