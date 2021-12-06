
#ifndef TIMER_H
#define TIMER_H
#include "types.h"
void sleep(u32 n);
u32 get_ticks();
void init_timer(u32 freq);

#endif
