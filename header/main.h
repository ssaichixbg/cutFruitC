#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <bios.h>
#include "CF\header\model.h"
#include "CF\header\logic.h"
#include "CF\header\cfgrap~1.h"
#include "CF\header\IO.h"

const short int CLOCK_F=10;
const short int FPS_HIGH =60; /*high speed mode*/
const short int FPS_NORMAL =30; /*normal speed mode*/
const short int FPS_LOW =10; /*low speed mode*/
const short int FPS_EXLOW =2; /*extreme low speed mode*/

const short int MOUSE_TRAIL_RADIUS =10; /*radius of mouse trail*/
const short int MOUSE_TRAIL_MAX_LENGTH =200; /*max length of speed*/
const short int G =9;
/*global variables*/
int FRAME=0;
short int OVER=0;
short int USER_POINT;/*user game point*/
short int FPS;/*game rate*/
short int TIME;/*game */
short int UNCUTTED_FRUIT;/*fruit not be cutted*/
short int GENERATED_FRUIT[5]={0,0,0,0,0};/*0-- 1-- 2-- 3-- 4-- 5-- 6--*/
UITYPE CURRENT_UI;
OBJECT *OBJECTS;
EVENT *EVENTS;
MOUSEPOINT *MOUSE_TRIAL;


/*----------------------function defination----------------------*/

/*----------------------main.c----------------------*/
int main(int argc, char const *argv[]);/*program system main function*/
void init(void);/*initialization*/
void cfexit(void);/*exit program*/
void circulation(void);/*game main circulation function*/
void welcomeUI(void);/*display welcome UI*/
void gameUI(void);/*display game UI*/
