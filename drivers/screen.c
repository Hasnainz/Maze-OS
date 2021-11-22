#include "screen.h"
#include "ports.h"

//Functions local to the program

int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_screen_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

//Kernal Print

void kprint_at(char *message, int col, int row){
  int offset;
  if(col >= 0 && row >= 0){
    offset = get_screen_offset(col, row);
  }
  else{
    offset = get_cursor_offset();
    row = get_offset_row(offset);
    col = get_offset_col(offset);
  }
  int i = 0;
  while(message[i] != 0){
    offset = print_char(message[i++], col, row, BLACK_ON_WHITE);
    //Get the row and column for the next iteration
    row = get_offset_row(offset);
    col = get_offset_col(offset);
  }
}

void kprint(char *message){
  kprint_at(message, -1, -1);
}

/*
 * Directly interacts with the video memory 
 *
 * If row and column are negative then we can print at the current
 * cursor location. Returns the offset of where the next character should
 * go, sets the video cursor to the returned offset
 *
 */

//We can write a print_char() function by displaying a 
//single char to a specific column and row on the screen

int print_char(char character, int col, int row, char attribute_byte) {
  // Create a char pointer to the start of video memory
  unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
  //if the byte is 0 then assume the default style
  if(!attribute_byte){
    attribute_byte = WHITE_ON_BLACK;
  }

  //We get a red E if the coordinates aren't correct for error checking
  if (col >= MAX_COLS || row >= MAX_ROWS){
    vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
    vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = RED_ON_WHITE;
    return get_screen_offset(col, row);
  }


  // Get the video memory offset for the screen location
  int offset;
  //If the column and row are non-negative then we can use them for the offset
  if(col >= 0 && row >= 0){
    offset = get_screen_offset(col, row);
  }
  else{
    offset = get_cursor_offset();
  }
  //If we get a newline character then we want to ofset to the end of the current row
  //so we can advance to the first column of the next row
  if(character == '\n') { 
    row = get_offset_row(offset); 
    offset = get_screen_offset(0, row+1);
  }
  //Otherwise, write the character and its attribute byte(its colour) to the video 
  //memory at our calculated offset  
  else{
    vidmem[offset] = character;
    vidmem[offset + 1] = attribute_byte;
    //Update the offset by 2 bytes because (character + attribute are 1 byte each)
    offset += 2;
  }
  //Update the cursor so it is ready for the next input 
  set_cursor_offset(offset);
  return offset; //Return where the cursor will be 
}

int get_cursor_offset(){
  //Use the VGA (video graphics array) ports to get the current cursor position
  //get the high byte of the cursor offset (data 14)
  //get the low byte (data 15)
  port_byte_out(REG_SCREEN_CTRL, 14);
  int offset = port_byte_in(REG_SCREEN_DATA) << 8;// << 8 is  1000000b;
  port_byte_out(REG_SCREEN_CTRL, 15);
  offset += port_byte_in(REG_SCREEN_DATA);
  return offset * 2; //Position * size of the character cell
}

void set_cursor_offset(int offset){
  offset /= 2;
  port_byte_out(REG_SCREEN_CTRL, 14);
  port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
  port_byte_out(REG_SCREEN_CTRL, 15);
  port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));

}

void clear_screen(){
  int screen_size = MAX_COLS * MAX_ROWS;
  int i; 
  char *screen = (char *) VIDEO_ADDRESS;
  for(i = 0; i < screen_size; i++){
    screen[i*2] = ' ';
    screen[i*2 + 1] = WHITE_ON_BLACK;
  }
  set_cursor_offset(get_cursor_offset(0, 0));
}

//Maps row and column coordinates to a memory location
int get_screen_offset(int col, int row){
  return 2 * (row * MAX_COLS + col);
}

int get_offset_row(int offset){
  return offset / (2 * MAX_COLS);
}
int get_offset_col(int offset){
  return (offset - (get_offset_row(offset) * 2 * MAX_COLS))/2;
}

