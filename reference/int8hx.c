#include <dos.h>
typedef void interrupt (*FP)(void);
FP old8h;
int over=0;
void interrupt int8h(void)
{
   static int count=0;
   static char buf[4]={'0', 0x17, '0', 0x17};
   static int second=0;
   int i;
   char far *p = (char far *)0xB8000000;
   count++;
   if(count<18)
      goto EOI;
   count = 0;
   buf[0] = second/10+'0';
   buf[2] = second%10+'0';
   for(i=0; i<4; i++)
      p[i] = buf[i];
   second++;
   if(second>=20)
      over = 1;
EOI:
   (*old8h)();
}

main()
{
   old8h = getvect(8);
   setvect(8, int8h);
   while(!over)
      ;
   setvect(8, old8h);
   puts("Done");
}