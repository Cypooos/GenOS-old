// common
#include <common/types.h>
// modules loading
#include <graphics/VGATUI.h>
#include <gdt.h>

int main(void);

void _start(void)
{
  clearScreen();
  printf("[kernel] loading gdt...");
	main();
};

int main(void)
{
  while (1) { }
};
