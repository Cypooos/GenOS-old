#ifndef FILE__GRAPHICS__VGATUI_H_
#define FILE__GRAPHICS__VGATUI_H_

#include <common/types.h>

static uint16* VideoMemory ;

void printf(char* string);
void printf_hex(uint8 hex);
void printf_hex32(uint32 hex);
void printf_bin(uint8 hex);

void print_bytes(void *ptr, int size);
void print_bits(void *ptr, int size);

void clearScreen();
void write(char* text, uint8 startx, uint8 starty, int ink);
void invert(uint8 x, uint8 y);

void colorTest();


#endif
