#include "maze.h"
#include "../cpu/timer.h"
#include "../drivers/display.h"
#include "random.h"
#include "../cpu/types.h"
#include "kernel.h"

//The maze environment rewritten in C. This only generates mazes using prims.
//This also uses the solution to exercise 2 to solve the maze and then the 
//same stack to backtrack the solution (method b of the grand finale) because
//space is important here (because I didn't manage to set up paging). This 
//doesn't solve mazes with loops which isn't a problem for this environment because
//prims doesn't create loops. We also don't have issues with the shortest path 
//because prims should always make perfect mazes.

//Manages the time gap between animations
u16 sleep_time = 100;

void sleep_time_add(){
  if(sleep_time == 2500)
    return;
  sleep_time += 100;
}
void sleep_time_subtract(){
  if(sleep_time == 0)
    return;
  sleep_time -= 100;
}

typedef struct {
    int x;
    int y;
} pos;

//Essentially IRobot.WALL, added error(used for testing) and backtracking values 
#define WALL 0
#define BEEN_BEFORE 1
#define PASSAGE 2
#define ROBOT 3
#define EXIT 4
#define SOLVED_BACKTRACK 5
#define ERROR 6

//Prims start -----------------------------------------------------------------------
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
  if(x > 0 && x < WIDTH-1 && y > 0 && y < HEIGHT-1){
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


//Uses prims algorithm to make a random mst
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
  //Pick our tile to start prims from (I'm using (1,1) for consistancy) 
  maze[i][j] = PASSAGE;

  //Add the possible walls that prims could stretch to the following array
  pos pos_walls[1000];
  int index = 0;
  index = add_walls(maze, pos_walls, index, i, j);
  draw_maze(maze);
  while(index > 0){
    //Pick a random cell from the list of frontier cells
    pos w = pos_walls[rand_int(index)];
    //Connect it to a random valid neighbour
    pos neighbours[] = { {w.x,w.y-2}, {w.x,w.y+2}, {w.x-2,w.y}, {w.x+2,w.y} };

    do{
      k = rand_int(4);
      if(legal(neighbours[k].x, neighbours[k].y)){
        if(maze[neighbours[k].x][neighbours[k].y] == PASSAGE){
          maze[(w.x+neighbours[k].x) >> 1][(w.y+neighbours[k].y) >> 1] = PASSAGE;
          maze[w.x][w.y] = PASSAGE;

          //Draw the pixels directly for speed instead of regenerating the entire maze
          pixel((w.x+neighbours[k].x) >> 1, (w.y+neighbours[k].y) >> 1, PASSAGE_COLOUR);
          pixel(w.x, w.y, PASSAGE_COLOUR);

          index = add_walls(maze, pos_walls, index, w.x, w.y); 
          index = remove_wall(pos_walls, index, w); 
          if(sleep_time > 0)
            sleep(sleep_time/50);
          break; 
        } 
      } 
    }while(1); 
  } 
  //Set the start tile
  maze[1][1] = ROBOT;
  pixel(1, 1, ROBOT_COLOUR);
  //Set the target tile 
  maze[WIDTH-3][HEIGHT-3] = EXIT;
  pixel(WIDTH-3, HEIGHT-3, EXIT);
}
//prims end ----------------------------------------------------------------------------

//Sets the maze back to its starting state (for testing purposes)
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
  maze[WIDTH-3][HEIGHT-3] = EXIT;
}

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

//Moves the robot in a cardinal direction
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
//Sees what environment are in what direction
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

//Counts the given type of environment around the robot.
u8 count_env(u8 maze[WIDTH][HEIGHT], pos robot, u8 type){
  u8 i;
  int c = 0;
  for(i = UP; i <= LEFT; i++){
    if(look(maze, robot, i) == type){
      c += 1;
    }
  }
  return c;
}

// How the robot should behave in a deadend
u8 deadend(u8 maze[WIDTH][HEIGHT], pos robot){
  u8 i;
  for(i = UP; i <= LEFT; i++){
    if(look(maze, robot, i) != WALL){
      return i;
    }
  }
  return -1;
}

//What the robot does in a dead end
u8 corridor(u8 maze[WIDTH][HEIGHT], pos robot, u8 current_heading){
  if(look(maze, robot, current_heading) != WALL){
    return current_heading;
  }
  else if(look(maze, robot, (current_heading+1)%4) != WALL){
    return (current_heading+1)%4;
  }
  else{
    return (current_heading+3)%4;
  }
}
// Pop a value off a stack
u8 pop(u8 stack[], u16 *stack_top){
  (*stack_top)--;
  return stack[*stack_top];
}

// Push a value onto a stack
void push(u8 val, u8 stack[], u16 *stack_top){
  stack[*stack_top] = val;
  (*stack_top)++;
}

//What we do at a junction
u8 junction(u8 maze[WIDTH][HEIGHT], pos robot, u8 explore_stack[], u16 *exp_st_top,
            u8 current_heading){

  if (count_env(maze, robot, BEEN_BEFORE) <= 1) {
    push(current_heading, explore_stack, exp_st_top);
  }
  
  if(count_env(maze, robot, PASSAGE) > 0){
    while(1){
      u8 rand = (u8)rand_int(4);
      if(look(maze, robot, rand) == PASSAGE){
        return rand;
      }
    }
  }
  return (pop(explore_stack, exp_st_top)+2)%4;
}

//Handles where we need to go next
u8 explore_control(u8 maze[WIDTH][HEIGHT], pos robot, u8 explore_stack[], 
                   u16 *exp_st_top, u8 current_heading){
  u8 dir;
  int exits = 4 - count_env(maze, robot, WALL);
  switch(exits) {
    case 0:
    case 1:
      return deadend(maze, robot);
    case 2:
      return corridor(maze, robot, current_heading);
    case 3:
    case 4:
      return junction(maze, robot, explore_stack, exp_st_top, current_heading);
    default:
      return -1;
  }
}

//A different type of movement when backtracking our solve 
pos solved_move(u8 maze[WIDTH][HEIGHT], u8 dir, pos robot){
  maze[robot.x][robot.y] = SOLVED_BACKTRACK;
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

u8 explore_stack[3000];

//Handles our backtrack to the start
u8 control_backtrack(u8 maze[WIDTH][HEIGHT], pos robot, u8 current_heading, u16 *exp_st_top){
  u8 been_before = count_env(maze, robot, BEEN_BEFORE);
  u8 exits = 4 - count_env(maze, robot, WALL);
  u8 dir = UP;
  //If we are at the final tile, find the only available tile to start our path
  if(robot.x == WIDTH-3 && robot.y == HEIGHT-3){
    u8 i;
    for(i = UP; i <= LEFT; i++){
      if(look(maze, robot, i) == BEEN_BEFORE)
        return i;
    }
  }
  //Then handle corridors and junctions appropriately
  else if (exits == 2){
    //Same as the previous corridor handling
    if(look(maze, robot, current_heading) != WALL){
      return current_heading;
    }
    else if(look(maze, robot, (current_heading+1)%4) != WALL){
      return (current_heading+1)%4;
    }
    else{
      return (current_heading+3)%4;
    }
  }
  else{
    //We want the reverse direction of the stack when tracing our route
    return (pop(explore_stack, exp_st_top)+2)%4;
  }
}

//Handles our route tracing
void trace_root(u8 maze[WIDTH][HEIGHT], u16 *exp_st_top) {
  pos robot = {WIDTH-3, HEIGHT-3};
  u8 current_heading = UP;
  //Keep going until we reach the start
  while(robot.x != 1 || robot.y != 1){
    //Find our direction
    u8 direction = control_backtrack(maze, robot, current_heading, exp_st_top);

    current_heading = direction;
    //Move in our selected direction
    robot = solved_move(maze, direction, robot);

    //Draw the maze out
    draw_maze(maze);
    sleep(sleep_time/5);
  }

}

//Solves the maze and traces the root
void solve_maze(u8 maze[WIDTH][HEIGHT]) {
  u16 exp_st_top = 0;

  u8 current_heading = UP;
  pos robot = {1, 1};

  while(robot.x != WIDTH-3 || robot.y != HEIGHT-3){
    u8 direction = explore_control(maze, robot, explore_stack, &exp_st_top, current_heading);

    current_heading = direction;
    robot = move(maze, direction, robot);

    if(sleep_time == 0){
      continue;
    }
    draw_maze(maze);
    sleep(sleep_time/9);
  }
  draw_maze(maze);
  sleep(10000);
  trace_root(maze, &exp_st_top);
  draw_maze(maze);
  sleep(10000);
}
