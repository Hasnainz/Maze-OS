#ifndef DISPLAY
#define DISPLAY

#include "../cpu/types.h"
#include "../kernel/maze.h"

//0 = wall
//1 = been before
//2 = passage
//3 = robot
//4 = exit
#define WALL_COLOUR 0x11
#define BEEN_BEFORE_COLOUR 0x1e
#define PASSAGE_COLOUR 0x34
#define ROBOT_COLOUR 0x21
#define EXIT_COLOUR 0x2f
#define SOLVED_BACKTRACK_COLOUR 0x27
#define ERROR_COLOUR 0x28

void draw_maze(u8 maze[WIDTH][HEIGHT]);
void fill_screen(unsigned char colour);
void pixel(int pos_x, int pos_y, unsigned char VGA_Colour);
void line(int x1, int y1, int x2, int y2, unsigned char colour);
void polygon(int vertices,int* vert_coords, unsigned char colour);

#endif
