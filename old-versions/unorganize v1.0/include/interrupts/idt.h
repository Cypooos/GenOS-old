#ifndef FILE__INTERRUPT__IDT_H_
#define FILE__INTERRUPT__IDT_H_
#include <common/types.h>


#define IDTBASE  0x800
#define IDTSIZE  0xFF
#define INTGATE  0x8E00

struct idtdesc {
    uint16 offset0_15;
    uint16 select;
    uint16 type;
    uint16 offset16_31;
} __attribute__ ((packed));

struct idtdata {
	uint16 limit;
	uint32 base;
} __attribute__ ((packed));

void init_idt_desc(uint16 select, uint32 offset, uint16 type, struct idtdesc* desc);
void idt_init(void);


struct idtdata IDT_DATA;
struct idtdesc IDT[IDTSIZE];

#endif
