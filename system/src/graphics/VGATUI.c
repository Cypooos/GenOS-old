#include <graphics/VGATUI.h>
#include <common/converter.h>
#include <common/types.h>
#include <stdarg.h>

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

void write(char* text, uint8 startx, uint8 starty, int ink)
{
  int color = ink;
  uint8 x = startx;
  uint8 y = starty;
  for(int i = 0; text[i] != '\0'; ++i)
  {
    switch(text[i])
    {
      case '$':
        {
          if(text[i+1]=='$') {FrameBuffer[80*y+x] = color | text[i];i++;break;} // if not double
          if (text[i+1]=='\0' || text[i+2]=='\0') {FrameBuffer[80*y+x] = color | text[i];break;} // if not error
          if(text[i+1]=='!') {color = ink;i--;} // if not reset
          else {
            char* foo = "00";
            foo[0] = text[i+1];
            foo[1] = text[i+2];
            color = strToHex(foo);
            color = color << 8; // 0xFF00 => 0xABCD .A => Background B => forground
          }
          i+=2;x--;
          break;
        }
      case '\n':
        x = startx-1; y++;break;
      default:
        FrameBuffer[80*y+x] = color | text[i];
        break;
    }
    x++;
    if(x >= 80){x = startx;y++;}
    if(y >= 25) {return;}
  }
}



void invert(uint8 x, uint8 y)
{
  FrameBuffer[80*y+x] = (FrameBuffer[80*y+x] & 0x0F00) << 4 | (FrameBuffer[80*y+x] & 0xF000) >> 4 | (FrameBuffer[80*y+x] & 0x00FF);
}



void colorTest(int offset)
{
  for (int x = 0; x < 16; x++)
    for (int y = 0; y < 16; y++)
      putchar('#',x*16+y << 8);
}
