#ifndef FILE__GRAPHICS__VGATUI_H_
#define FILE__GRAPHICS__VGATUI_H_

#include <common/types.h>

static uint16_t* VGATUI_VideoMemory;

void printf(char* string);

void clearScreen();
void write(char* text, uint8_t startx, uint8_t starty, int ink);
void invert(uint8_t x, uint8_t y);

void colorTest();


#endif
