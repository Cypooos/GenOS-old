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



  gdt_load_basic(10);
  printf("[KERNEL] Second GDT setup.\n");

  struct gdtdesc gdt_desc[10];
  memcpy((char *) gdt_desc,(char *) GDT_POINTER.base, GDT_POINTER.limit);
  for(int i = 0; i<10; i++)
  {
    gdt_print_desc(gdt_desc[i]);
    printf("\n");
  }


	while (1);
};
