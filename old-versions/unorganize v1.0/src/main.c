#include <common/types.h>
#include <graphics/VGATUI.h>
#include <gdt.h>
#include <interrupts/idt.h>
#include <interrupts/pic.h>
#include <common/asm.h>

int main(void);

void _start(void)
{
  clearScreen();
	printf("[KERNEL] Kernel started.\n");

  printf("[KERNEL] Setting up interrupts.\n");
  idt_init();

  printf("[KERNEL] Setting up pic.\n");
  pic_init();

  printf("[KERNEL] Setting up gdt.\n");
  gdt_load_basic(5);


  asm("   movw $0x18, %ax \n \
          movw %ax, %ss \n \
          movl $0x20000, %esp");
	main();
};

int main(void)
{
	printf("[KERNEL] Main process launch.\n");

  sti;
  printf("[KERNEL] $0AInterrupts enabled!\n");


  while (1) { }
};
