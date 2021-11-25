/* How we read and write to our I/O address spaces
 * we use specific control registers - e.g. the floppy
 * disk controller has its DOR register mapped to IO
 * address 0x3f2 so we can switch it on by 
 *
 * mov dx, 0x3f2 ; must use dx to store the port address
 * in al, dx ; reading the contents of the port DOR to al
 * or al, 00001000b ; switch on the motor bit
 * out dx, al 
 */

unsigned char port_byte_in(unsigned short port) {
  //Reads a byte from a specificied port
  // =a (result) means put the contents of the al register in a variable valled result
  // d (port) means load edx with port
  // Different to NASM because gcc uses GAS assembly syntax
  unsigned char result;
  __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
  return result;
}
void port_byte_out(unsigned short port, unsigned char data){
  //a (data) means to load eax with data
  //d (port) means to load edx with port
  __asm__("out %%al, %%dx" : :"a" (data), "d" (port));
}
unsigned short port_word_in(unsigned short port) {
  unsigned short result;
  __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
  return result;
}
void port_word_out(unsigned short port, unsigned short data){
  __asm__("out %%ax, %%dx" : :"a" (data), "d" (port));
}
