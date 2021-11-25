#include "../drivers/display.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "random.h"

//Converting int array to pixels
//0 = wall
//1 = been before
//2 = passage
//3 = robot
//4 = exit


//0, 1, 2 ...
//320, 321, 322 ...

int maze[WIDTH][HEIGHT];

void main() {
  isr_install();
  clear_screen(0x43);
  generate_maze();
}

int is_legal(int x, int y){
  return (x > 0 && x < WIDTH-1 && y > 0 && y < HEIGHT-1);
}

//Generates a maze with integers using prims algorithm
void generate_maze(){
  int frontier_index = 0;
   
  //Start with a grid of full walls
  int i, j;
  for(i = 0; i < WIDTH; i++){
    for(j = 0; j < HEIGHT; j++){
      maze[i][j] = 0;
    }
  }
  //Pick a cell and compute its frontier cells -> 
  //a frontier cell of a cell is a cell with distance 2, is value 0, within the grid



  //While the list of frontier cells isn't empty:
  //Pick a random frontier cells from our list of frontier cells.
  //Let neighbors(frontierCell) = all cells in distance 2 in state passage. pick
  //a random neighbor and connect the frontier cell with the neighbor by setting 
  //the cell in-between to state Passage. Compute the frontier cells fo the chosen frontier
  //cell and add them to the frontier list. Remove the chose frontier cell from the list
  //of frontier cells.

  //Put the robot top-left and the exit bottom-right
  maze[1][1] = 3; 
  maze[318][198] = 4;
  draw_maze(maze);
}


