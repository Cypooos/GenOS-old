#ifndef FILE__COMMON__CONVERTER_H_
#define FILE__COMMON__CONVERTER_H_
#include <common/types.h>
int strToHex(char* text);

char* uint8ToStr(uint8 hex);
char* uint16ToStr(uint16 hex);
char* uint32ToStr(uint32 hex);

#endif
