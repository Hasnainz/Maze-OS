#include "random.h"
#include "../cpu/timer.h"

//Using the PIT clock as a seed - we can generate random
//numbers because we don't have any library to do it for us
int randInt(int range){
  unsigned int seed = (214013*tick+2531011); 
  return ((seed>>16)&0x7FFF) % range;
}
