#include <common/types.h>
#include <common/asm.h>
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
  uint8 i;
  static int lshift_enable;
  static int rshift_enable;
  static int alt_enable;
  static int ctrl_enable;

  do {
    i = inb(0x64);
  } while ((i & 0x01) == 0);

  i = inb(0x60);
  i--;

  //// putcar('\n'); dump(&i, 1); putcar(' ');

  if (i < 0x80) {         /* touche enfoncée */
    switch (i) {
      case 0x29:
        lshift_enable = 1;
        break;
      case 0x35:
        rshift_enable = 1;
        break;
      case 0x1C:
        ctrl_enable = 1;
        break;
      case 0x37:
        alt_enable = 1;
        break;
      default:
        printf("[$03interrupt$!] key: %h2, shift=%h2\n", i, rshift_enable || lshift_enable);
    }
  } else {                /* touche relâchée */
    i -= 0x80;
    switch (i) {
      case 0x29:
        lshift_enable = 0;
        break;
      case 0x35:
        rshift_enable = 0;
        break;
      case 0x1C:
        ctrl_enable = 0;
        break;
      case 0x37:
        alt_enable = 0;
        break;
    }
  }
}
