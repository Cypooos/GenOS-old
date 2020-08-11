#include <graphics/VGATUI.h>
#include <common/types.h>
#include <common/asm.h>
#include <common/converter.h>
#include <stdarg.h> // for printf() multiples args

static uint16* FrameBuffer = (uint16*)0xb8000;

void printf(char* string, ...) {

  va_list args;
  va_start(args, string);

  uint8 color = 0x0F;
  for(int i = 0; string[i] != '\0'; ++i)
  {
    if (string[i] == '%') {
      if (string[i+1] == '%')
        {putchar('%',color);i++;}
      else if (string[i+1] == '\0')
        {putchar('%',color);}
      else {
        if ((string[i+1] == 'h' || string[i+1] == 'H') && string[i+2] != '\0'){
          if (string[i+1] == 'H') {
            putchar('0',color);
            putchar('x',color);
          }
          switch(string[i+2]){
            case '1':{
              int x = va_arg(args, int);
              char* t = uint8ToStr((uint8)x<<4);
              putchar(t[0],color);
              i+=2;break;
            }
            case '2':{
              int x = va_arg(args, int);
              char* t = uint8ToStr((uint8)x);
              putchar(t[0],color);
              putchar(t[1],color);
              i+=2;break;
            }
            case '4':{
              int x = va_arg(args, int);
              char* t = uint16ToStr((uint16)x);
              putchar(t[0],color);
              putchar(t[1],color);
              putchar(t[2],color);
              putchar(t[3],color);
              i+=2;break;
            }
            case '8':{
              int x = va_arg(args, int);
              char* t = uint32ToStr((uint32)x);
              putchar(t[0],color);
              putchar(t[1],color);
              putchar(t[2],color);
              putchar(t[3],color);
              putchar(t[4],color);
              putchar(t[5],color);
              putchar(t[6],color);
              putchar(t[7],color);
              i+=2;break;
            }
            default:putchar('%',color);break;
          }
        } else if (string[i+1] == 'b' && string[i+2] != '\0'){
          switch(string[i+2]){
            // case '2':{
            //   int x = va_arg(args, int);
            //   char* t = uint8ToBinStr((uint8)x<<6);
            //   printf(t);
            //   i+=2;break;
            // }
            // case '1':{
            //   int x = va_arg(args, int);
            //   char* t = uint8ToBinStr((uint8)x);
            //   putchar(t[0]);
            //   i+=2;break;
            // }
            // case '4':{
            //   int x = va_arg(args, int);
            //   char* t = uint8ToBinStr((uint8)(x<<4));
            //   printf(t);
            //   i+=2;break;
            // }
            // case '8':;
            //   int x = va_arg(args, int);
            //   char* t = uint8ToBinStr((uint8)x);
            //   printf(t);
            //   i+=2;break;
            default:putchar('%',color);break;
          }
        } else if (string[i+1] == 's'){
          printf(va_arg(args, char*));
          i++;
        }
      }
    }
    else if (string[i] == '$'){
      if (string[i+1] == '$')
        {putchar('$',color);i++;}
      else if(string[i+1]=='!') {color = 0x0F;i++;} // reset ink
      else if ((string[i+1] == '\0' || string[i+2] == '\0'))
        {putchar('$',color);}
      else {
        {
          char* foo = "00";
          foo[0] = string[i+1];
          foo[1] = string[i+2];
          color = strToHex(foo);
          color = (uint8)(color); // 0xFF00 => 0xABCD .A => Background B => forground
          i+=2;
        }
      }
    } else {putchar(string[i],color);}
  }
  va_end(args);
}


void putchar(char char_,uint8 color) {
  static uint8 x=0, y=0;

  if (char_ == '\n'){
    x = 0;y++;
  }
  else {
    FrameBuffer[80*y+x] = (color << 8) | char_;
    x++;
  }
  if(x >= 80) {x = 0; y++; }
  if(y >= 25) {scrollup(2); x=0;y-=2; }
  move_cursor(x,y);
}


void move_cursor (uint8 x, uint8 y)
{
    uint16 c_pos;

    c_pos = y * 80 + x;
    outb(0x3d4, 0x0f);
    outb(0x3d5, (uint8) c_pos);
    outb(0x3d4, 0x0e);
    outb(0x3d5, (uint8) (c_pos >> 8));
}


void clearScreen()
{
  for(uint8 y = 0; y < 25; y++)
    for(uint8 x = 0; x < 80; x++)
      FrameBuffer[80*y+x] = (0x0F00) | ' ';
}

void scrollup(uint8 n)
{
  for(uint8 y = 0; y < 25-n; y++)
    for(uint8 x = 0; x < 80; x++)
      FrameBuffer[80*y+x] = FrameBuffer[80*(y+n)+x];
}
