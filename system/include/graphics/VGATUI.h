#ifndef FILE__GRAPHICS__VGATUI_H_
#define FILE__GRAPHICS__VGATUI_H_

#include <common/types.h>

static uint16* FrameBuffer;

void printf(char* string, ...);
void putchar(char char_,uint8 color);
void move_cursor (uint8 x, uint8 y);

void clearScreen();
void scrollup(uint8 n);

#endif
