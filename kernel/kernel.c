#include "../drivers/screen.h"


void main() {
  unsigned char *vid = (unsigned char *) 0xa0000;
  vid[0] = 100;
}
