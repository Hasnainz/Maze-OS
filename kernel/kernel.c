#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../drivers/display.h"
#include "../cpu/types.h"
#include "maze.h"
#include "util.h"
#include "kernel.h"


void init() {
  isr_install();
  asm volatile("sti");
  init_timer(10000);
  init_keyboard();
}

void main() {
  init();
  u8 maze[WIDTH][HEIGHT];

  generate_maze(maze);
  draw_maze(maze);

  solve_maze(maze);
  draw_maze(maze);

}

