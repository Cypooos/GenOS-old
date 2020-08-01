#include <interrupts/idt.h>
#include <common/types.h>
#include <common/memMap.h>
#include <graphics/VGATUI.h>

extern _asm_default_int;
extern _asm_irq_0;
extern _asm_irq_1;

void idt_init_desc(uint16 select, uint32 offset, uint16 type, struct idtdesc* desc) {
    desc->offset0_15 = (offset & 0xffff);
    desc->select = select;
    desc->type = type;
    desc->offset16_31 = (offset & 0xffff0000) >> 16;
    return;
}

void idt_init(void) {
  for (int i = 0; i < IDTSIZE; i++)                                   /* defaut */
        idt_init_desc(0x08, (uint32) _asm_default_int, INTGATE, &IDT[i]);

  idt_init_desc(0x08, (uint32) _asm_irq_0, INTGATE, &IDT[32]);      /* horloge */
  idt_init_desc(0x08, (uint32) _asm_irq_1, INTGATE, &IDT[33]);      /* clavier */
}
