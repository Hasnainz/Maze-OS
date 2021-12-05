#include "timer.h"
#include "isr.h"
#include "types.h"

u32 tick = 0;

void sleep(u32 n){
  u32 tick_max = tick + n;
  do{
    //nothing
  }while(tick_max > tick);
}

u32 get_ticks(){
  return tick;
}

static void timer_callback(registers_t regs) {
    tick++;
}

void init_timer(u32 freq) {
    /* Install the function we just wrote */
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    u16 divisor = 1193180 / freq;
    //We need to split the divisor into a high and low section
    u8 low  = (u8)(divisor & 0xFF);
    u8 high = (u8)( (divisor >> 8) & 0xFF);
    /* Send the command */
    port_byte_out(0x43, 0x36); /* Command port */
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}

