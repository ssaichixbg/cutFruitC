/****************************************************/
/* Mouse in Vesa Mode(1024*768*24bit)               */
/* copyright (c) Black White. Mar 15, 2012          */
/* email: iceman@zju.edu.cn                         */
/* This program is for teaching purpose only,       */
/* if you want to make reference to it, please keep */
/* the above copyright information.                 */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <dos.h>
#include <bios.h>
#include <alloc.h>

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned long int dword;

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

struct 
{
   int x, y;
   byte shape[16][16][3];
} old_mouse;

typedef struct
{
/* +000 */ char signature[4];
/* +004 */ word version;
/* +006 */ char far *oem_name_ptr;
/* +00A */ dword capability;
/* +00E */ word far *video_mode_ptr;
/* +012 */ word video_memory_blocks;
/* +014 */ word software_version;
/* +016 */ char far *vendor_name_ptr;
/* +01A */ char far *product_name;
/* +01E */ char far *product_revision;
/* +022 */ word vbe_version;
/* +024 */ word far *accelerated_video_mode_ptr;
/* +028 */ char reversed_for_vbe_implementation[0xD8];
/* +100 */ char oem_scratch_pad[0x100];
} VESA_INFO; /* total 0x200 bytes */

typedef struct
{
/* +00 */ word mode_attr;
/* +02 */ byte win_a_attr;
/* +03 */ byte win_b_attr;
/* +04 */ word win_granularity;
/* +06 */ word win_size;
/* +08 */ word win_a_seg;
/* +0A */ word win_b_seg;
/* +0C */ void (far *win_fun_ptr)(void);
/* +10 */ word bytes_per_line;
/* +12 */ word width_pixels;
/* +14 */ word height_pixels;
/* +16 */ byte char_width;
/* +17 */ byte char_height;
/* +18 */ byte planes;
/* +19 */ byte bits_per_pixel;
/* +1A */ byte banks;
/* +1B */ byte mem_model_type;
/* +1C */ byte bank_size;
/* +1D */ byte image_pages;
/* +1E */ byte zero_or_one;
/* +1F */ byte red_bits;
/* +20 */ byte red_bit_off;
/* +21 */ byte green_bits;
/* +22 */ byte green_bit_off;
/* +23 */ byte blue_bits;
/* +24 */ byte blue_bit_off;
/* +25 */ byte reserved_bits;
/* +26 */ byte reserved_bit_off;
/* +27 */ byte direct_color_mode_info;
/* +28 */ dword vbuf_phys_addr;
/* +2C */ dword off_screen_addr;
/* +30 */ word off_screen_mem_blocks;
/* +32 */ byte reserved[0xCE];
} MODE_INFO;

dword win_bytes;  /* maximum bytes in one mapping window */
byte huge *vp;    /* pointer to video buffer */
void (far * map_win_fun)(void); /* pointer to function for mapping window */
MODE_INFO * set_vesa_mode_1024x768x24bit(void);
VESA_INFO * get_vesa_info(void);
int get_mode_info(word mode, MODE_INFO far *mi);
int set_vesa_mode(word mode);
int map_window_by_interrupt(word win);
void map_window(word win);
MODE_INFO * set_vesa_mode_1024x768x24bit(void);

void get_dot(int x, int y, byte color[3]);
void put_dot(int x, int y, byte color[3]);
void draw_block(int x, int y, byte color[3]);
int  init_mouse(void);
void get_mouse_status(int *px, int *py, int *pbutton);
void clear_old_mouse(void);
void save_as_old_mouse(int x, int y);
void draw_mouse(int x, int y);
void move_mouse(void);
void text_mode(void);

int display_bmp(char *filename);

main()
{
   MODE_INFO *mi = NULL;
   int x, y, button;
   byte pink[3] = {0xFF, 0x00, 0xFF};   /* Red=255, Green=0, Blue=255 */
   mi = set_vesa_mode_1024x768x24bit(); /* switch to 1024*768*24bit video mode */
   if(mi == NULL)
   {
      puts("Cannot set video mode to 1024x768x24bit.");
      return;
   }
   map_win_fun = mi->win_fun_ptr;       /* fetch pointer to map_window() */
   vp = (byte huge *)((dword)mi->win_a_seg << 16); /* video pointer like A000:0000 */
   /* When we add an integer to a far pointer, the offset address contained in
      that pointer may exceed 0xFFFF while the associated segment address does
      not increment thus resulting in a wrong address. Here huge pointer is much
      better than far pointer in this situation, because huge pointer can adjust
      its segment address automatically.
    */
   win_bytes = mi->win_size * 0x400L;  /* win_size's unit is Kilobyte */
   if(display_bmp("pic.bmp") == 0)
   {
      puts("Cannot display bmp file!");
      return;
   }
   init_mouse();
   while(bioskey(1) == 0)
   {
      get_mouse_status(&x, &y, &button);
      if(button & 1) /* left button is clicked */
      {
         clear_old_mouse();
         draw_block(x, y, pink); /* draw a pink block at (x, y) */
         save_as_old_mouse(x, y);
         draw_mouse(x, y);
      }
      move_mouse();
   }
   text_mode();
   free(mi);
}

void text_mode(void)
{
   _AX=0x0003;
   geninterrupt(0x10);
}

VESA_INFO * get_vesa_info(void)
{
   VESA_INFO *p;
   struct REGPACK r;
   p = calloc(sizeof(VESA_INFO),1);
   if(p == NULL)
      return NULL;
   memset(&r, 0, sizeof(r));
   r.r_ax = 0x4F00;
   r.r_di = (dword)(void far *)p & 0xFFFF;
   r.r_es = (dword)(void far *)p >> 16;
   intr(0x10, &r);
   if((r.r_ax & 0xFF) != 0x4F)
   {
      free(p);
      p = NULL;
   }
   return p;
}

int get_mode_info(word mode, MODE_INFO far *mi)
{
   struct REGPACK r;
   r.r_ax = 0x4F01;
   r.r_cx = mode;
   r.r_di = (dword)mi & 0xFFFF;
   r.r_es = (dword)mi >> 16;
   intr(0x10, &r);
   return (r.r_ax & 0xFF) == 0x4F;
}

int set_vesa_mode(word mode)
{
   struct REGPACK r;
   r.r_ax = 0x4F02;
   r.r_bx = mode;
   intr(0x10, &r);
   return (r.r_ax & 0xFF) == 0x4F;
}

int map_window_by_interrupt(word win)
{ /* this function is slower than map_window() */
   struct REGPACK r;
   r.r_ax = 0x4F05;
   r.r_bx = 0;    /* set window=0; window A=0; */
   r.r_dx = win;
   intr(0x10, &r);
   return (r.r_ax & 0xFF) == 0x4F;
}

void map_window(word win)
{
   word old_bx;
   old_bx = _BX;
   _BX = 0;
   _DX = win;
   (*map_win_fun)();
   _BX = old_bx;
}

MODE_INFO * set_vesa_mode_1024x768x24bit(void)
{
   VESA_INFO *vi = NULL;
   MODE_INFO *mi = NULL;
   word mode, far *pmode;
   vi = get_vesa_info();
   if(vi == NULL)
      goto set_vesa_mode_error;
   mi = calloc(sizeof(MODE_INFO), 1);
   if(mi == NULL)
      goto set_vesa_mode_error;
   pmode = vi->video_mode_ptr;
   while(*pmode != 0xFFFF) /* goto the end of mode list */
      pmode++;
   while(--pmode >= vi->video_mode_ptr) /* search backward */
   {                                    /* for mode 1024x768x24bit */
      if(!get_mode_info(*pmode, mi))
         continue;
      if((mi->mode_attr & 0x99) != 0x99)
         continue;
      if(mi->width_pixels != 1024 || mi->height_pixels != 768 ||
         mi->bits_per_pixel != 24)
         continue;
      if(mi->bytes_per_line / mi->width_pixels != 24/8)
         continue;
      break;
   }
   if(pmode < vi->video_mode_ptr) /* search failed */
      goto set_vesa_mode_error;
   mode = *pmode;
   if(!set_vesa_mode(mode))
      goto set_vesa_mode_error;
   free(vi);
   return mi;
set_vesa_mode_error:
   if(vi != NULL)
      free(vi);
   if(mi != NULL)
      free(mi);
   return NULL;
}


int display_bmp(char *filename)
{
   FILE *fp = NULL;
   word win, offset;
   dword width, height, bmp_data_offset, bytes_per_line;
   int i, j;
   byte *p = NULL; /* pointer to a line of bmp data */
   p = malloc(1024L * 3);  /* memory for holding a line of bmp data */
   if(p == NULL)
      goto display_bmp_error;
   fp = fopen(filename, "rb");
   if(fp == NULL)
      goto display_bmp_error;
   fread(p, 1, 0x36, fp);     /* read BMP head */
   if(*(word *)p != 0x4D42)   /* check BMP signature */
      goto display_bmp_error;
   if(*(word *)(p+0x1C) != 24)
      goto display_bmp_error;
   width = *(dword *)(p+0x12);
   height = *(dword *)(p+0x16);
   bmp_data_offset = *(dword *)(p+0x0A);
   fseek(fp, bmp_data_offset, SEEK_SET); /* skip BMP head */

   bytes_per_line = (width * 3 + 3) / 4 * 4; /* must be multiple of 4 */
   for(i=height-1; i>=0; i--)          /* draw from bottom to top */
   {
      fread(p, 1, bytes_per_line, fp); /* read a line of bmp data */
      /* compute which window this line resides in */
      win = (i*1024L*3) / win_bytes;
      offset = (i*1024L*3) % win_bytes;
      if(offset + width*3 <= win_bytes)
      {    /* if this line is within one window */
         map_window(win);
         movedata((dword)(void huge *)p >> 16,
                  (dword)(void huge *)p & 0xFFFF,
                  (dword)(vp+offset) >> 16,
                  (dword)(vp+offset) & 0xFFFF,
                  width*3);
      }
      else /* if this line overrides two windows */
      {
         dword bytes_in_win, bytes_left;
         bytes_in_win = win_bytes - offset;
         map_window(win);
         movedata((dword)(byte huge *)p >> 16,
                  (dword)(byte huge *)p & 0xFFFF,
                  (dword)(vp+offset) >> 16,
                  (dword)(vp+offset) & 0xFFFF,
                  bytes_in_win);
         bytes_left = width*3 - bytes_in_win;
         map_window(win+1);
         movedata((dword)((byte huge *)p+bytes_in_win) >> 16,
                  (dword)((byte huge *)p+bytes_in_win) & 0xFFFF,
                  (dword)vp >> 16,
                  (dword)vp & 0xFFFF,
                  bytes_left);
      }
   }
   free(p);
   fclose(fp);
   return 1;
display_bmp_error:
   if(p != NULL)
      free(p);
   if(fp != NULL)
      fclose(fp);
   return 0;
}

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
   r.x.cx = 0;
   r.x.dx = 0;
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
	 put_dot(x+j, y+i, old_mouse.shape[i][j]);
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
         get_dot(x+j, y+i, old_mouse.shape[i][j]);
      }      
   }
}

void draw_mouse(int x, int y)
{
   int i, j;
   byte white[3]={0xFF,0xFF,0xFF};
   for(i=0; i<16; i++)
   {
      for(j=0; j<16; j++)
      {
         if(cursor_mask[i]>>(15-j) & 1)
            put_dot(x+j, y+i, 255);
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

void get_dot(int x, int y, byte color[3])
{
   word win, offset;
   byte *p = color;
   win = ((y*1024L+x)*3) / win_bytes;
   offset = ((y*1024L+x)*3) % win_bytes;
   if(offset + 3 <= win_bytes)
   {    /* if 3 colors of this dot are within one window */
      map_window(win);
      movedata((dword)(vp+offset) >> 16,
               (dword)(vp+offset) & 0xFFFF,
               (dword)(void huge *)p >> 16,
               (dword)(void huge *)p & 0xFFFF,
               3);
   }
   else /* if this dot overrides two windows */
   {
      dword bytes_in_win, bytes_left;
      bytes_in_win = win_bytes - offset;
      map_window(win);
      movedata((dword)(vp+offset) >> 16,
               (dword)(vp+offset) & 0xFFFF,
               (dword)(byte huge *)p >> 16,
               (dword)(byte huge *)p & 0xFFFF,               
               bytes_in_win);
      bytes_left = 3 - bytes_in_win;
      map_window(win+1);
      movedata((dword)vp >> 16,
               (dword)vp & 0xFFFF,
               (dword)((byte huge *)p+bytes_in_win) >> 16,
               (dword)((byte huge *)p+bytes_in_win) & 0xFFFF,               
               bytes_left);
   }
}

void put_dot(int x, int y, byte color[3])
{
   word win, offset;
   byte *p = color;
   win = ((y*1024L+x)*3) / win_bytes;
   offset = ((y*1024L+x)*3) % win_bytes;
   if(offset + 3 <= win_bytes)
   {    /* if 3 colors of this dot are within one window */
      map_window(win);
      movedata((dword)(void huge *)p >> 16,
               (dword)(void huge *)p & 0xFFFF,
               (dword)(vp+offset) >> 16,
               (dword)(vp+offset) & 0xFFFF,
               3);
   }
   else /* if this dot overrides two windows */
   {
      dword bytes_in_win, bytes_left;
      bytes_in_win = win_bytes - offset;
      map_window(win);
      movedata((dword)(byte huge *)p >> 16,
               (dword)(byte huge *)p & 0xFFFF,
               (dword)(vp+offset) >> 16,
               (dword)(vp+offset) & 0xFFFF,
               bytes_in_win);
      bytes_left = 3 - bytes_in_win;
      map_window(win+1);
      movedata((dword)((byte huge *)p+bytes_in_win) >> 16,
               (dword)((byte huge *)p+bytes_in_win) & 0xFFFF,
               (dword)vp >> 16,
               (dword)vp & 0xFFFF,
               bytes_left);
   }
}

void draw_block(int x, int y, byte color[3])
{  /* draw a 4x4 block at (x, y) */
   int i, j;
   for(i=0; i<4; i++)
   {
      for(j=0; j<4; j++)
      {
         put_dot(x+j, y+i, color);
      }
   }
}
