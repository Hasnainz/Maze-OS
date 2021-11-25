#include "keyboard.h"
#include "ports.h"
#include "../cpu/isr.h"

static void keyboard_callback(registers_t regs) {
  //The PIC leaves scancode in 0x60
  u8 scancode = port_byte_in(0x60);

}

// n = new maze
// s = start/stop maze
//  
//
//
//
//
//

void init_keyboard() {
  register_interrupt_handler(IRQ1, keyboard_callback);
}
