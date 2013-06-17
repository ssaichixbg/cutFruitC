#include "CF\header\header.h"
#define UP 0x4800
#define DOWN 0x5000
#define LEFT 0x4B00
#define RIGHT 0x4D00
#define ESC 0x011B
#define W 32
#define H 30

unsigned short cursor_mask[] =
{
   /* 16 words of cursor mask */
   0x0000, /*0000000000000000*/
   0x4000, /*0100000000000000*/
   0x6000, /*0110000000000000*/
   0x7000, /*0111000000000000*/
   0x7800, /*0111100000000000*/
   0x7C00, /*0111110000000000*/
   0x7E00, /*0111111000000000*/
   0x7F00, /*0111111100000000*/
   0x7C00, /*0111110000000000*/
   0x4600, /*0100011000000000*/
   0x0600, /*0000011000000000*/
   0x0300, /*0000001100000000*/
   0x0300, /*0000001100000000*/
   0x0180, /*0000000110000000*/
   0x0180, /*0000000110000000*/
   0x00C0, /*0000000011000000*/
};
static struct 
{
   int x, y;
   byte shape[16][16];
} old_mouse;

int init_mouse(void);
void get_mouse_status(int *px, int *py, int *pbutton);
void clear_old_mouse(void);
void save_as_old_mouse(int x, int y);
void draw_mouse(int x, int y);
void move_mouse(void);

int init_mouse(void)
{
   int x, y;
   union REGS r;
   r.x.ax = 0;      /* check mouse driver */
   int86(0x33, &r, &r);
   if(r.x.ax != 0xFFFF)
      return 0;

   r.x.ax = 0x0007; /* set horizontal range */
   r.x.cx = 0;
   r.x.dx = 1024-1;
   int86(0x33, &r, &r);
   r.x.ax = 0x0008; /* set vertical range */
   r.x.cx = 0;
   r.x.dx = 768-1;
   int86(0x33, &r, &r);

   r.x.ax = 0x0004; /* position cursor */
   r.x.cx = 1024/2;
   r.x.dx = 768/2;
   int86(0x33, &r, &r);

   r.x.ax = 2;      /* hide cursor */
   int86(0x33, &r, &r);

   r.x.ax = 0x0003; /* get mouse's status */
   int86(0x33, &r, &r);
   x = r.x.cx;
   y = r.x.dx;
   save_as_old_mouse(x, y);
   return 1;
}
void get_mouse_status(int *px, int *py, int *pbutton)
{
   union REGS r;
   r.x.ax = 0x0003;
   int86(0x33, &r, &r);
   *px = r.x.cx;
   *py = r.x.dx;
   *pbutton = r.x.bx;
}
void clear_old_mouse(void)
{
   int i, j;
   int x, y;
   x = old_mouse.x;
   y = old_mouse.y;
   for(i=0; i<16; i++)
   {
      for(j=0; j<16; j++)
      {
	 	drawPoint(x+j, y+i, 0,old_mouse.shape[i][j]);
      }
   }
}

void save_as_old_mouse(int x, int y)
{
   int i, j;
   old_mouse.x = x;
   old_mouse.y = y;
   for(i=0; i<16; i++)
   {
      for(j=0; j<16; j++)
      {
         old_mouse.shape[i][j]=getPixl(x+j, y+i);
      }      
   }
}

void draw_mouse(int x, int y)
{
   int i, j;
   for(i=0; i<16; i++)
   {
      for(j=0; j<16; j++)
      {
         if(cursor_mask[i]>>(15-j) & 1)
            drawPoint(x+j, y+i,0,1);
      }
   }
}
void move_mouse(void)
{
   int x, y;
   union REGS r;
   r.x.ax = 0x0003;
   int86(0x33, &r, &r);
   x = r.x.cx;
   y = r.x.dx;
   if(x == old_mouse.x && y == old_mouse.y)
      return;
   clear_old_mouse();
   save_as_old_mouse(x, y);
   draw_mouse(x, y);
}