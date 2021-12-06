#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../drivers/display.h"
#include "../cpu/types.h"
#include "maze.h"
#include "kernel.h"
#include "random.h"

// Inits interrupts, timer, keyboard and seeds the random number generator
void init() {
  isr_install();
  asm volatile("sti");
  init_timer(10000);
  init_keyboard();
  sgenrand(get_ticks());
}

//The main program
void main() {
  init();
  u8 maze[WIDTH][HEIGHT];

  generate_maze(maze);
  draw_maze(maze);

  solve_maze(maze);
  draw_maze(maze);
  // Until the system is shut down, continuously generates and solves random mazes
  while(1){
    generate_maze(maze);
    sleep(1000);  
    solve_maze(maze);
    sleep(1000);  
  }

}

