#include "CF\header\main.h"

typedef void interrupt (*FP)(void);
FP old8h;
static int COUNT=0;

int main(int argc, char const *argv[])
{
   init();
   init_mouse();
   while(!OVER)
   {
   }

}
void interrupt int8h(void)
{
   if(bioskey(1)!=0)
      if(bioskey(0)==27)
         OVER=1;
      
   if(OVER)
   {
      cfexit();
      (*old8h)();
      return;
   }
   circulation();
   (*old8h)();
}
void init(void)
{
   set_vesa_mode_1024x768x8bit();
   drawBackground();
   USER_POINT=0;/*user game point*/
   FPS=FPS_NORMAL;/*game rate*/
   TIME=0;/*game */
   UNCUTTED_FRUIT=0;/*fruit not be cutted*/


   CURRENT_UI = WELCOME_UI;
   OBJECTS = NULL;
   EVENTS = NULL;
   MOUSE_TRIAL = NULL;

   OBJECTS = newObjectChain();
   EVENTS = newEventChain(); 
   MOUSE_TRIAL = newMousePointChain();
   
   /*set clock interrupt*/
   outportb(0x43, 0x36);
   outportb(0x40, 1193180/CLOCK_F & 0xFF);
   outportb(0x40, 1193180/CLOCK_F >> 8);
   old8h = getvect(8);
   setvect(8, int8h);
}
void cfexit(void)
{
   /*reset clock interrurpt*/
   setvect(8, old8h);
   outportb(0x43, 0x36);
   outportb(0x40, 0);
   outportb(0x40, 0);
}
void circulation(void)
{
   int i,j;
   COUNT++;
   FRAME++;

   if(CURRENT_UI == WELCOME_UI)
   {
      welcomeUI();
   }
   else
   {
      gameUI();
   }

   if(COUNT==CLOCK_F)
   {
      COUNT = 0;
   }


}
void welcomeUI(void)
{
   calPosition();
   processEvents();
   buildEvent();
   move_mouse();
   drawScreen();
   
}
void gameUI(void)
{

}