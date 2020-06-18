#include <graphics/VGATUI.h>
#include <common/converter.h>
#include <common/types.h>

static uint16* VideoMemory = (uint16*)0xb8000;


void printf_hex(uint8 hex)
{
  printf(hexToStr(hex));
}

void printf_hex32(uint32 hex)
{
  printf(hexToStr((uint8)(hex & 0xFF)));
  printf(hexToStr((uint8)(hex & 0xFF00 >> 8)));
  printf(hexToStr((uint8)(hex & 0xFF0000 >> 16)));
  printf(hexToStr((uint8)(hex & 0xFF000000 >> 24)));
}


void printf_bin(uint8 hex)
{
  printf(binToStr(hex));
}


void print_bytes(void *ptr, int size)
{
  uint8 *p = ptr;
  int i;
  for (i=0; i<size; i++) {
    printf(hexToStr(p[i]));
    printf(" ");
  }
}

void print_bits(void *ptr, int size)
{
  uint8 *p = ptr;
  int i;
  for (i=0; i<size; i++) {
    printf(hexToStr(p[i]));// temporary
    //printf(binToHex(p[i]));
    printf(" ");
  }
  printf("\n");
}

void printf(char* string) {

  static uint8 x=0,y=0;

  for(int i = 0; string[i] != '\0'; ++i)
  {
    switch(string[i])
    {
      case '\n':
        x = 0;y++;
        break;
      default:
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | string[i];
        x++;
        break;
    }

    if(x >= 80){x = 0;y++;}

    if(y >= 25)
    {
      for(y = 0; y < 25; y++)
        for(x = 0; x < 80; x++)
          VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
      x = 0;
      y = 0;
    }
  }
}

void clearScreen()
{
  for(uint8 y = 0; y < 25; y++)
    for(uint8 x = 0; x < 80; x++)
      VideoMemory[80*y+x] = (0x0F00) | ' ';
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
          if(text[i+1]=='$') {VideoMemory[80*y+x] = color | text[i];i++;break;} // if not double
          if (text[i+1]=='\0' || text[i+2]=='\0') {VideoMemory[80*y+x] = color | text[i];break;} // if not error
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
        VideoMemory[80*y+x] = color | text[i];
        break;
    }
    x++;
    if(x >= 80){x = startx;y++;}
    if(y >= 25) {return;}
  }
}



void invert(uint8 x, uint8 y)
{
  VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4 | (VideoMemory[80*y+x] & 0xF000) >> 4 | (VideoMemory[80*y+x] & 0x00FF);
}



void colorTest(int offset)
{
  for (int x = 0; x < 16; x++)
    for (int y = 0; y < 16; y++)
      write(1,x*4,y+offset,x*16+y << 8);
}
