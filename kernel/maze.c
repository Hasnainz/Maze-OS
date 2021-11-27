#include "maze.h"
#include "../cpu/timer.h"
#include "../drivers/display.h"
#include "random.h"
#include "../cpu/types.h"
#include "kernel.h"

typedef struct {
    int x;
    int y;
} wall;

//0 = wall
//1 = been before
//2 = passage
//3 = robot
//4 = exit

int contains(wall pos_walls[], int index, int x, int y){
  int i = 0;
  while(i < index){
    if(pos_walls[i].x == x && pos_walls[i].y == y){
      return i;
    }
    i++;
  }
  return -1;
}

//The tile can be a possible candidate of being primmed if it is in the bounds,
//if it is a wall and if it is not already in the list.
int legal(int x, int y){
  if(x > 0 && x < WIDTH && y > 0 && y < HEIGHT){
    return 1;  
  }
  return 0;
}

int add_walls(u8 maze[WIDTH][HEIGHT], wall pos_walls[], int index, int x, int y){
  int i;
  for(i = x - 2; i <= x + 2; i += 4){
    if(legal(i, y) && contains(pos_walls, index, i, y) == -1){
      if(maze[i][y] == 0){
        pos_walls[index].x = i;
        pos_walls[index].y = y;
        index++;

      }
    }
  }
  for(i = y - 2; i <= y + 2; i += 4){
    if(legal(x, i) && contains(pos_walls, index, x, i) == -1){
      if(maze[x][i] == 0){
        pos_walls[index].x = x;
        pos_walls[index].y = i;
        index++;
      }
    }
  }
  return index;
}

int remove_wall(wall pos_walls[], int index, wall w){
  int i = contains(pos_walls, index, w.x, w.y);
  --index;
  for(;i < index; i++){
    pos_walls[i] = pos_walls[i+1];
  }
  return index;
}


//Uses prims algorithm to make a random Minimum Spanning Tree 
void generate_maze(u8 maze[WIDTH][HEIGHT]) {
  //Start with a maze of walls
  int i, j, k;
  for(i = 0; i < WIDTH; i++){
    for(j = 0; j < HEIGHT; j++){
      maze[i][j] = 0;
    }
  }

  i = 1;
  j = 1;
  //Pick our tile to start prims from 
  maze[i][j] = 2;


  //Add the possible walls that prims could stretch to
  wall pos_walls[1000];
  int index = 0;
  index = add_walls(maze, pos_walls, index, i, j);

  draw_maze(maze);
  while(index > 0){
    //Pick a random cell from the list of frontier cells
    wall w = pos_walls[rand_int(index)];
    //Connect it to a random valid neighbour
    wall neighbours[] = { {w.x,w.y-2}, {w.x,w.y+2}, {w.x-2,w.y}, {w.x+2,w.y} };

    do{
      k = rand_int(4);
      if(legal(neighbours[k].x, neighbours[k].y)){
        if(maze[neighbours[k].x][neighbours[k].y] == 2){
          maze[(w.x+neighbours[k].x) >> 1][(w.y+neighbours[k].y) >> 1] = 2;
          maze[w.x][w.y] = 2;
          index = add_walls(maze, pos_walls, index, w.x, w.y);
          index = remove_wall(pos_walls, index, w);
          break;
        }
      }
    }while(1);
    draw_maze(maze);
  }
  //Set the start and end tiles:
  maze[1][1] = 3;
  maze[WIDTH-1][HEIGHT-1] = 4;
  draw_maze(maze);
}

void solve_maze(int maze[WIDTH][HEIGHT]) {

}
