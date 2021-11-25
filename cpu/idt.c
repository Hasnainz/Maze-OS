#include "idt.h"

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

//Sets the gates of the interrupts
void set_idt_gate(int n, u32 handler){
  idt[n].low_offset = low_16(handler);
  idt[n].sel = KERNEL_CS;
  idt[n].always_0 = 0;
  idt[n].flags = 0x8E;
  idt[n].high_offset = high_16(handler);
}

void set_idt() {
  idt_reg.base = (u32) &idt; //The address of the idt is the base
  idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1; //The address of the top is the limit
  
  //Load Interrupt descriptor table
  __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}
