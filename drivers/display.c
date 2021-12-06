#include "display.h"
#include "../kernel/maze.h"

//Where the draw memory location is - notice that it is different to the value
//in the print.asm file because there we were in text mode but here we are in graphics
//mode
unsigned char *vid = (unsigned char *) 0xa0000;

//Draws a pixel on the scren coordinates x,y with the specified colour
void pixel(int pos_x, int pos_y, unsigned char VGA_Colour){
  vid[(pos_y << 8) + (pos_y << 6) + pos_x] = VGA_Colour;
}


//Just fills the screen with a colour - used for testing and not in use at the moment
void fill_screen(unsigned char colour){
  int i, j;
  for(i = 0; i < WIDTH; i++)
    for(j = 0; j < HEIGHT; j++)
      pixel(i, j, colour);
}

//Converting int array to pixels
//0 = wall
//1 = been before
//2 = passage
//3 = robot
//4 = exit

void draw_maze(u8 maze[WIDTH][HEIGHT]){
  int i, j;
  for(i = 0; i < WIDTH; ++i){
    for(j = 0; j < HEIGHT; ++j){
      switch(maze[i][j]){
        case 0:
          pixel(i, j, WALL_COLOUR);
          break;
        case 1:
          pixel(i, j, BEEN_BEFORE_COLOUR);
          break;
        case 2:
          pixel(i, j, PASSAGE_COLOUR);
          break;
        case 3:
          pixel(i, j, ROBOT_COLOUR);
          break;
        case 4:
          pixel(i, j, EXIT_COLOUR);
          break;
        case 5:
          pixel(i, j, SOLVED_BACKTRACK_COLOUR);
          break;
        default:
          pixel(i, j, ERROR_COLOUR);
          break;
      }
    }
  }
}

//These functions were used to test drawing to the screen with lines and shapes
//and are not currently in use

// Bresenhams line drawing algorithm
void line(int x1, int y1, int x2, int y2, unsigned char colour){
  int i,dx,dy,sdx,sdy,absx, absy,x,y,px,py;

  dx = x2 - x1;
  if(dx < 0){
    sdx = -1;
    absx = -dx;
  }
  else{
    sdx = 1;
    absx = dx;
  }

  dy = y2 - y1;
  if(dy < 0){
    sdy = -1;
    absy = -dy;
  }
  else{
    sdy = 1;
    absy = dy;
  }
  x = absy >> 1;
  y = absx >> 1;
  px = x1;
  py = y1;
  vid[(py << 8) + (py << 6) + px] = colour;
  
  if(absx >= absy){
    for(i = 0; i < absx; i++){
      y += absy;
      if(y >= absx){
        y -= absx;
        py += sdy;
      }
      px += sdx;
      pixel(px, py, colour);
    }
  }
  else{
    for(i = 0; i < absy; i++){
      x += absx;
      if(x >= absy){
        x -= absy;
        px += sdx;
      }
      py += sdy;
      pixel(px, py, colour);
    }
  }

}

void polygon(int vertices,int* vert_coords, unsigned char colour){
  int i;
  for(i = 0; i < vertices-1; i++){
    line(vert_coords[i<<1], vert_coords[(i<<1)+1], 
        vert_coords[(i<<1)+2], vert_coords[(i<<1)+3], colour);
  }
  line(vert_coords[0], vert_coords[1], 
      vert_coords[(vertices<<1)-2], vert_coords[(vertices<<1)-1], colour);

}
