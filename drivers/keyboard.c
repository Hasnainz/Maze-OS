#include "keyboard.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "../kernel/random.h"
#include "../kernel/kernel.h"
#include "../kernel/maze.h"

//Handles the keyboard inputs - we currently only deal with an up and down arrow key
//because this maze environment doesn't do anything special and doesn't have any 
//extra inputs. This directly interacts with the maze environment by adding and subtracting
//time from how long the gap is between draw animations

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

