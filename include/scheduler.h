#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include "types.h"
#include "tasks.h"

#define TASK_STACK_SIZE 512
#define TASK_MAX_COUNT 32

void scheduler_init();
void scheduler_start();
void task_create(void (*task)());
u32int task_switch(u32int last_esp);
void block(u32int pid);
void unblock(u32int pid);

#endif
