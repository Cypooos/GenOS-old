#include <common/converter.h>
#include <common/types.h>

int strToHex(char* text)
{
  int end = 0;
  for(int i = 0; text[i] != '\0'; ++i)
  {
    end = end << 4;
    switch(text[i])
    {
      case '0':break;
      case '1':end++;break;
      case '2':end+=2;break;
      case '3':end+=3;break;
      case '4':end+=4;break;
      case '5':end+=5;break;
      case '6':end+=6;break;
      case '7':end+=7;break;
      case '8':end+=8;break;
      case '9':end+=9;break;
      case 'A':case 'a':end+=10;break;
      case 'B':case 'b':end+=11;break;
      case 'C':case 'c':end+=12;break;
      case 'D':case 'd':end+=13;break;
      case 'E':case 'e':end+=14;break;
      case 'F':case 'f':end+=15;break;
      default:break;
    }
  }
  return end ;
};

char* uint8ToStr(uint8 hex)
{
  char* foo = "XX";
  const char* alpha = "0123456789ABCDEF";
  foo[1] = alpha[(hex & 0x0F)];
  foo[0] = alpha[(hex &0xF0) >>4];
  return foo;
}

char* uint16ToStr(uint16 hex)
{
  char* foo = "XXXX";
  const char* alpha = "0123456789ABCDEF";
  foo[3] = alpha[(hex & 0x000F)];
  foo[2] = alpha[(hex & 0x00F0) >>4];
  foo[1] = alpha[(hex & 0x0F00) >>8];
  foo[0] = alpha[(hex & 0xF000) >>12];
  return foo;
}

char* uint32ToStr(uint32 hex)
{
  char* foo = "XXXXXXXX";
  const char* alpha = "0123456789ABCDEF";
  foo[7] = alpha[(hex & 0x0000000F)];
  foo[6] = alpha[(hex & 0x000000F0)>>4];
  foo[5] = alpha[(hex & 0x00000F00)>>8];
  foo[4] = alpha[(hex & 0x0000F000)>>12];
  foo[3] = alpha[(hex & 0x000F0000)>>16];
  foo[2] = alpha[(hex & 0x00F00000)>>20];
  foo[1] = alpha[(hex & 0x0F000000)>>24];
  foo[0] = alpha[(hex & 0xF0000000)>>28];
  return foo;
}
