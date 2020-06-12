#include <common/types.h>
#include <graphics/VGATUI.h>

int main(void);

void _start(void)
{
  clearScreen();
	printf("[KERNEL] Kernel started.\n");
	main();
}

int main(void)
{
	printf("[KERNEL] Main process launch.\n");

  colorTest(2);

	while (1);
}
