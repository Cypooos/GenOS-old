#include <gdt.h>
#include <common/types.h>
#include <common/memMap.h>
#include <graphics/VGATUI.h>


void gdt_set_descriptor(uint32 base, uint32 limit, uint8 acces, uint8 data, struct gdtdesc *desc)
{
  desc->limit0_15 = (limit & 0x0FFFF);
  desc->limit16_19 = (base & 0xF0000) >> 16;

  desc->base0_15 = (base & 0x0000FFFF);
  desc->base16_23 = (base & 0x00FF0000) >> 16;
  desc->base24_31 = (base & 0xFF000000) >> 24;

  desc->acces = acces;
  desc->data = (data & 0xF);
};

void gdt_print_desc(struct gdtdesc *desc)
{
  printf("BASE ");
  printf_hex32(gdt_get_base(desc));
  printf(" LIMIT ");
  printf_hex32(gdt_get_limit(desc));
  printf(" ACCES ");
  printf_hex(desc->acces);
  //printf_bin(desc->acces);
  printf(" DATA ");
  printf_hex((uint8)desc->data);
  //printf_bin((uint8)desc->data);
}

uint32 gdt_get_limit(struct gdtdesc *desc)
{
  uint32 limit = (desc->limit0_15 & 0xFFFF) | ((desc->limit16_19 & 0xF) << 16);
  return limit;
};

uint32 gdt_get_base(struct gdtdesc *desc)
{
  uint32 base = (desc->base0_15 & 0x0000FFFF) | ((desc->base16_23 & 0xFF) << 16) | ((desc->base24_31 & 0xF) << 24);
  return base;
};

void gdt_load(struct gdtdesc *gdt_desc)
{

  memcpy((char *) GDT_POINTER.base, (char *) gdt_desc, GDT_POINTER.limit);

  asm("lgdtl (GDT_POINTER)");

  asm("   movw $0x10, %ax \n \
    movw %ax, %ds       \n \
    movw %ax, %es       \n \
    movw %ax, %fs       \n \
    movw %ax, %gs       \n \
    ljmp $0x08, $temp   \n \
    temp:               \n");
};

void gdt_load_basic(int size)
{
  struct gdtdesc gdtdes[size];

  gdt_set_descriptor(0x0, 0x0, 0x0, 0x0, &gdtdes[0]); // first is empty
  gdt_set_descriptor(0x0, 0xFFFFF, 0x9B, 0x0D, &gdtdes[1]);      /* code */
  gdt_set_descriptor(0x0, 0xFFFFF, 0x93, 0x0D, &gdtdes[2]);      /* data */
  gdt_set_descriptor(0x0, 0x0, 0x97, 0x0D, &gdtdes[3]);          /* stack */

  GDT_POINTER.base = 0x0;
  GDT_POINTER.limit = size * 8;


};
