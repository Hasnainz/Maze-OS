#ifndef MAZE_H
#define MAZE_H

#define WIDTH 320
#define HEIGHT 200

#include "../cpu/types.h"

void sleep_time_add();
void sleep_time_subtract();
void generate_maze();
void solve_maze();
void reset_solve(u8 maze[WIDTH][HEIGHT]);

#endif
