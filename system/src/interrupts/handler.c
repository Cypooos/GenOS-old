#include <common/types.h>
#include <graphics/VGATUI.h>
#include <interrupts/handler.h>

void isr_default_int(void)
{
  printf("[$03interrupt$!] no_binded\n");
}

void isr_clock_int(void)
{
  static int tic = 0;
  static int sec = 0;
  tic++;
  if (tic % 100 == 0) {
    sec++;
    printf("[$03interrupt$!] clock: %H8sec\n",sec);
    tic = 0;
  }
}

void isr_kbd_int(void)
{
  printf("[$03interrupt$!] key\n");
}
