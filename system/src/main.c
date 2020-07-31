#include <common/types.h>
#include <graphics/VGATUI.h>
#include <gdt.h>

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



	while (1);
};
