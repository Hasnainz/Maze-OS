#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../drivers/display.h"
#include "../cpu/types.h"
#include "maze.h"
#include "kernel.h"

void init() {
  isr_install();
  asm volatile("sti");
  init_timer(4000);
  init_keyboard();
}

//The keyboard handler sends its interrupt handlers here.
void s_button(){
  fill_screen(0x0d);
}

void m_button(){
  u8 maze[WIDTH][HEIGHT];
  generate_maze(maze);
  fill_screen(0x0e);
}

void main() {
  init();
  m_button();
}


