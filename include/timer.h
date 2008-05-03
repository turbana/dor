#ifndef __TIMER_H
#define __TIMER_H
#include "isrs.h"

void timer_phase(unsigned int);
void timer_tick(struct regs *);
void timer_init();

#endif
