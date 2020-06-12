#include <graphics/VGATUI.h>
#include <common/converter.h>
#include <common/types.h>
static uint16_t* VGATUI_VideoMemory = (uint16_t*)0xb8000;

void printf(char* string) {

  static uint8_t x=0,y=0;

  for(int i = 0; string[i] != '\0'; ++i)
  {
    switch(string[i])
    {
      case '\n':
        x = 0;
        y++;
        break;
      default:
        VGATUI_VideoMemory[80*y+x] = (VGATUI_VideoMemory[80*y+x] & 0xFF00) | string[i];
        x++;
        break;
    }

    if(x >= 80)
    {
      x = 0;
      y++;
    }

    if(y >= 25)
    {
      for(y = 0; y < 25; y++)
        for(x = 0; x < 80; x++)
          VGATUI_VideoMemory[80*y+x] = (VGATUI_VideoMemory[80*y+x] & 0xFF00) | ' ';
      x = 0;
      y = 0;
    }
  }
}

void clearScreen()
{
  for(uint8_t y = 0; y < 25; y++)
    for(uint8_t x = 0; x < 80; x++)
      VGATUI_VideoMemory[80*y+x] = (0x0F00) | ' ';
}


void write(char* text, uint8_t startx, uint8_t starty, int ink)
{
  int color = ink;
  uint8_t x = startx;
  uint8_t y = starty;
  for(int i = 0; text[i] != '\0'; ++i)
  {
    switch(text[i])
    {
      case '$':
        {
          if(text[i+1]=='$') {VGATUI_VideoMemory[80*y+x] = color | text[i];i++;break;} // if not double
          if (text[i+1]=='\0' || text[i+2]=='\0') {VGATUI_VideoMemory[80*y+x] = color | text[i];break;} // if not error
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
        VGATUI_VideoMemory[80*y+x] = color | text[i];
        break;
    }
    x++;
    if(x >= 80){x = startx;y++;}
    if(y >= 25) {return;}
  }
}



void invert(uint8_t x, uint8_t y)
{
  VGATUI_VideoMemory[80*y+x] = (VGATUI_VideoMemory[80*y+x] & 0x0F00) << 4 | (VGATUI_VideoMemory[80*y+x] & 0xF000) >> 4 | (VGATUI_VideoMemory[80*y+x] & 0x00FF);
}



void colorTest(int offset)
{
  for (int x = 0; x < 16; x++)
    for (int y = 0; y < 16; y++)
      write("FOO",x*4,y+offset,x*16+y << 8);
}
