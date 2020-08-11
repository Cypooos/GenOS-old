#ifndef FILE__GDT_H_
#define FILE__GDT_H_

//#define GDTBASE 0x0
//#define GDTSIZE 0xFF

#include <common/types.h>

struct gdtdesc {
  uint16 limit0_15;
  uint16 base0_15;
  uint8 base16_23;
  uint8 acces;// acces = type(4), segmentType(1), ring(2), isPhysical(1)
  //uint8 type:4;
  //uint8 segmentType:1;
  //uint8 ring:2;
  //uint8 isPhysical:1;
  uint8 limit16_19:4;
  uint8 data:4;
  // data = AVL-nothing yet-(1), a 0, DB(1), isPage(1)
  //uint8 AVL:1;
  //uint8 zero:1;
  //uint8 DB:1;
  //uint8 isPage:1;
  uint8 base24_31;
} __attribute__ ((packed));

struct gdtptr {
    uint16 limit;
    uint32 base;
} __attribute__ ((packed));


void gdt_set_descriptor(uint32 base, uint32 limite, uint8 acces, uint8 other,struct gdtdesc *desc);
void gdt_load(struct gdtdesc *gdt_desc);

void gdt_load_basic();

uint32 gdt_get_limit(struct gdtdesc *desc);
uint32 gdt_get_base(struct gdtdesc *desc);

struct gdtptr GDT_POINTER;

#endif
