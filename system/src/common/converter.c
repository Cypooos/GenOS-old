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
