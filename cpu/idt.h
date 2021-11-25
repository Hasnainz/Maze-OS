#ifndef IDT_H
#define IDT_H

#include "types.h"

//Segment selectors
#define KERNEL_CS 0x08

//How the interrupt handler is stored
typedef struct {
  u16 low_offset; //Lower 16 bits of the handler function address
  u16 sel; //Kernel segment selector
  u8 always_0;
  u8 flags;
  u16 high_offset; //Upper 16 bits of the address to jump to
} __attribute__((packed)) idt_gate_t;

//A pointer to an array of interrupt handlers
typedef struct {
  u16 limit;
  u32 base;

} __attribute__((packed)) idt_register_t;


//The number of interupts in our interrupt descriptor table
#define IDT_ENTRIES 256
extern idt_gate_t idt[IDT_ENTRIES];
extern idt_register_t idt_reg;

void set_idt_gate(int n, u32 handler);
void set_idt();

#endif
