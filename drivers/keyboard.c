#include "keyboard.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../kernel/random.h"
#include "../kernel/kernel.h"
#include "../kernel/maze.h"

static void keyboard_callback(registers_t regs) {
  /* The PIC leaves the scancode in port 0x60 */
  u8 scancode = port_byte_in(0x60);
  switch (scancode) {
    case 0x48:
      sleep_time_add();
      break;
    case 0x50:
      sleep_time_subtract();
      break;
    default:
      break;
  }
}

void init_keyboard() {
  register_interrupt_handler(IRQ1, keyboard_callback); 
}

