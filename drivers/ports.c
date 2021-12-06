#include "ports.h"

// Read a byte from a specified port
u8 port_byte_in (u16 port) {
    u8 result;

    // Inline assembler, this is slightly different from nasm
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

// Send a byte out of a port
void port_byte_out (u16 port, u8 data) {
    /* Notice how here both registers are mapped to C variables and
     * nothing is returned, thus, no equals '=' in the asm syntax 
     * However we see a comma since there are two variables in the input area
     * and none in the 'return' area
     */
    __asm__ __volatile__("out %%al, %%dx" : : "a" (data), "d" (port));
}

//Read a word from a specified port
u16 port_word_in (u16 port) {
    u16 result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

// Send a word our of a port
void port_word_out (u16 port, u16 data) {
    __asm__ __volatile__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
