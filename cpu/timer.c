#include "timer.h"
#include "isr.h"
#include "types.h"
#include "../drivers/ports.h"

u32 tick;

static void timer_callback(registers_t regs) {
  tick++;
}

void init_timer(u32 freq){
  register_interrupt_handler(IRQ0, timer_callback);

  //Programmable interrupt controller clocks in at
  //1193180 Hz
  u32 divisor = 1193180 / freq;
  u8 low = (u8)(divisor & 0xFF);
  u8 high = (u8)( (divisor >> 8) & 0xFF);

  port_byte_out(0x43, 0x36);
  port_byte_out(0x40, low);
  port_byte_out(0x40, high);
}
