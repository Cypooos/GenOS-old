#include <interrupts/idt.h>
#include <common/types.h>
#include <common/memMap.h>
#include <interrupts/handler.h>
#include <graphics/VGATUI.h>

void _asm_default_int(void);
void _asm_irq_0(void);
void _asm_irq_1(void);

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

	/* Initialisation de la structure pour IDTR */
	IDT_DATA.limit = IDTSIZE * 8;
	IDT_DATA.base = IDTBASE;

	/* Recopie de la IDT a son adresse */
	memcpy((char *) IDT_DATA.base, (char *) IDT, IDT_DATA.limit);

	/* Chargement du registre IDTR */
	asm("lidtl (IDT_DATA)");
}
