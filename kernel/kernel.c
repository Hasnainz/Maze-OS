#include "../drivers/screen.h"

void main() {
  clear_screen();
  kprint_at("Don't you wish you could write to the screen like me?", 0, 0);
  clear_screen();
  kprint_at("Cat's go meow", 0, 0);
}
