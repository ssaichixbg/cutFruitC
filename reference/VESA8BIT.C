/****************************************************/
/* Demo for VESA(1024*768*8bit) Graphics Library    */
/* copyright (c) Black White, April 1, 2013.        */
/* email: iceman@zju.edu.cn                         */
/* This program is for teaching purpose only,       */
/* if you want to reference it, please keep         */
/* the above copyright information.                 */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <bios.h>
#include "vesa8bit.h"
main()
{
   int x=1024/2-100, y;
   int x1, y1, x2, y2;
   byte color;
   set_vesa_mode_1024x768x8bit();    /* 切换显卡到1024x768x256色模式 */
   display_8bit_bmp("pic256_6.bmp"); /* 显示一张图片 */
   bioskey(0);

   set_vesa_mode_1024x768x8bit();    /* 重新切换到1024x768x256色模式, 相当于清屏 */
   vesa_puts(1024/2-9*8, 768/2, 14, "\x01\x03\x04Hello,world!\x05\x06\x02");
   bioskey(0);                       /* 在屏幕中央显示字符串, 其中\x开头的是特殊字符 */

   set_vesa_mode_1024x768x8bit();    /* 重新切换到1024x768x256色模式, 相当于清屏 */
   for(y=768/2-100; y<768/2+100; y++)/* 在屏幕中央画一个红色的矩形 */
   {
      vesa_line(x, y, x+200, y, 4);
   }
   bioskey(0);

   randomize();
   do                                /* 画随机直线 */
   {
      x1 = random(1024);
      x2 = random(1024);
      y1 = random(768);
      y2 = random(768);
      color = random(256);
      vesa_line(x1, y1, x2, y2, color);
   } while (bioskey(1) == 0);
   bioskey(0);

   set_vesa_mode_1024x768x8bit();    /* 清屏 */
   do                                /* 画随机点 */
   { 
      x1 = random(1024);
      y1 = random(768);
      color = random(256);
      vesa_putpixel(x1, y1, color);
   } while (bioskey(1) == 0);
   bioskey(0);

   text();                           /* 切换到文本模式 */
}
