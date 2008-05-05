#ifndef __TIMER_H
#define __TIMER_H

#include "isrs.h"
#include "types.h"

/*
 * Define TICK_REMINDER to have colored text printed to the screen every
 * second. Pretty.
 */
/*#define TICK_REMINDER*/

void timer_phase(u32int phase);
void timer_tick(struct regs *);
void timer_init();

#endif
