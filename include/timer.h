#ifndef __H_TIMER
#define __H_TIMER
#include "isrs.h"

void timer_phase(unsigned int);
void timer_tick(struct regs *);
void timer_init();

#endif
