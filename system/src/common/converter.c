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
    }
  }
  return end ;
};


char* binToStr(uint8 hex)
{
    char* bin[8];
    int x;
    uint8 h = hex;
    for(x=0;x<8;x++)
    {
        bin[x] = h & 0x80 ? '1' : '0';
        h <<= 1;
    }
    bin[8] = '\0';
    return bin;
}

char* hexToStr(uint8 hex)
{
  char* foo = "XX";
  const char* alpha = "0123456789ABCDEF";
  foo[0] = alpha[(hex & 0x0F)];
  foo[1] = alpha[(hex &0xF0) >>4];
  return foo;
}
