#include "maze.h"
#include "../cpu/timer.h"
#include "../drivers/display.h"
#include "random.h"
#include "../cpu/types.h"
#include "kernel.h"
#include "util.h"


typedef struct {
    int x;
    int y;
} pos;

#define WALL 0
#define BEEN_BEFORE 1
#define PASSAGE 2
#define ROBOT 3
#define EXIT 4

int contains(pos pos_walls[], int index, int x, int y){
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

int add_walls(u8 maze[WIDTH][HEIGHT], pos pos_walls[], int index, int x, int y){
  int i;
  for(i = x - 2; i <= x + 2; i += 4){
    if(legal(i, y) && contains(pos_walls, index, i, y) == -1){
      if(maze[i][y] == WALL){
        pos_walls[index].x = i;
        pos_walls[index].y = y;
        index++;

      }
    }
  }
  for(i = y - 2; i <= y + 2; i += 4){
    if(legal(x, i) && contains(pos_walls, index, x, i) == -1){
      if(maze[x][i] == WALL){
        pos_walls[index].x = x;
        pos_walls[index].y = i;
        index++;
      }
    }
  }
  return index;
}

int remove_wall(pos pos_walls[], int index, pos w){
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
      maze[i][j] = WALL;
    }
  }

  i = 1;
  j = 1;
  //Pick our tile to start prims from 
  maze[i][j] = PASSAGE;

  //Add the possible walls that prims could stretch to
  pos pos_walls[1000];
  int index = 0;
  index = add_walls(maze, pos_walls, index, i, j);

  while(index > 0){
    //Pick a random cell from the list of frontier cells
    pos w = pos_walls[rand_int(index)];
    //Connect it to a random valid neighbour
    pos neighbours[] = { {w.x,w.y-2}, {w.x,w.y+2}, {w.x-2,w.y}, {w.x+2,w.y} };

    do{
      k = rand_int(4);
      if(legal(neighbours[k].x, neighbours[k].y)){
        if(maze[neighbours[k].x][neighbours[k].y] == PASSAGE){
          maze[(w.x+neighbours[k].x) >> 1][(w.y+neighbours[k].y) >> 1] = 2;
          maze[w.x][w.y] = PASSAGE;
          index = add_walls(maze, pos_walls, index, w.x, w.y);
          index = remove_wall(pos_walls, index, w);
          break;
        }
      }
      draw_maze(maze);
    }while(1);
  }
  //Set the start and end tiles:
  maze[1][1] = ROBOT;
  maze[WIDTH-1][HEIGHT-1] = EXIT;
}


void reset_solve(u8 maze[WIDTH][HEIGHT]){
  int i, j;
  for(i = 0; i < WIDTH; i++){
    for(j = 0; j < HEIGHT; j++){
      if(maze[i][j] != WALL) {
        maze[i][j] = PASSAGE;
      }
    }
  }
  maze[1][1] = ROBOT;
  maze[WIDTH-1][HEIGHT-1] = EXIT;
}

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

pos move(u8 maze[WIDTH][HEIGHT], u8 dir, pos robot){
  maze[robot.x][robot.y] = BEEN_BEFORE;
  pos new = {robot.x, robot.y};
  switch(dir){
    case UP:
      new.y -= 1; 
      maze[new.x][new.y] = ROBOT;
      return new;
    case RIGHT:
      new.x += 1;
      maze[new.x][new.y] = ROBOT;
      return new;
    case DOWN:
      new.y += 1;
      maze[new.x][new.y] = ROBOT;
      return new;
    case LEFT:
      new.x -= 1;
      maze[new.x][new.y] = ROBOT;
      return new;
    default:
      break;
  }
}
u8 look(u8 maze[WIDTH][HEIGHT], pos robot, u8 dir){
  switch(dir){
    case UP:
      return maze[robot.x][robot.y-1];
    case RIGHT:
      return maze[robot.x+1][robot.y];
    case DOWN:
      return maze[robot.x][robot.y+1];
    case LEFT:
      return maze[robot.x-1][robot.y];
    default:
      return -1;
  }
}

u8 count_env(u8 maze[WIDTH][HEIGHT], pos robot, u8 type){
  int i;
  int c = 0;
  for(i = UP; i < LEFT; i++){
    if(look(maze, robot, i) == type){
      c += 1;
    }
  }
  return c;
}

u8 deadend(u8 maze[WIDTH][HEIGHT], pos robot){
  int i;
  for(i = UP; i < LEFT; i++){
    if(look(maze, robot, i) != WALL){
      return i;
    }
  }
  return -1;
}

u8 corridor(u8 maze[WIDTH][HEIGHT], pos robot){
  int i;
  for(i = UP; i < LEFT; i++){
    if(look(maze, robot, i) != WALL && i != DOWN){
      return i;
    }
  }
}

u8 junction(u8 maze[WIDTH][HEIGHT], pos robot){
  u8 passages = count_env(maze, robot, PASSAGE);

  int i;
  for(i = UP; i < LEFT; i++){
    if(look(maze, robot, i) != WALL && i != DOWN){
      return i;
    }
  }
}

u8 explore_control(u8 maze[WIDTH][HEIGHT], pos robot){
  int exits = 4 - count_env(maze, robot, WALL);
  switch(exits) {
    case 0:
    case 1:
      return deadend(maze, robot);
    case 2:
      return corridor(maze, robot);
    case 3:
    case 4:
      return junction(maze, robot);
    default:
      return -1;
  }
}

u8 solve_stack[1000];
u8 solved = 0;

void solve_maze(u8 maze[WIDTH][HEIGHT]) {
  u8 explore_stack[1000];
  if(solved == 1){
    reset_solve(maze);
    return;
  }
  pos robot = {1, 1};
  while(robot.x != WIDTH-1 && robot.y != HEIGHT-1){
    u8 direction = explore_control(maze, robot);
    robot = move(maze, direction, robot);
    draw_maze(maze);
    sleep(200);
  }
  solved = 1;
}
