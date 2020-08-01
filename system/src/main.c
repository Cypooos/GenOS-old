#include <common/types.h>
#include <graphics/VGATUI.h>
#include <gdt.h>
#include <interrupts/idt.h>

int main(void);

void _start(void)
{
  clearScreen();
	printf("[KERNEL] Kernel started.\n");
	main();
};

int main(void)
{
	printf("[KERNEL] Main process launch.\n");

  gdt_print();

  gdt_load_basic(5);
  printf("[KERNEL] Second GDT setup.\n");

  gdt_print();
  printf("[KERNEL] Setting up interrupts.\n");
  idt_init();
  printf("[KERNEL] System ready !.\n");


	while (1);
};
