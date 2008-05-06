#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include "types.h"
#include "tasks.h"

#define TASK_STACK_SIZE 512
#define TASK_MAX_COUNT 5

void schedule();
void scheduler_init();

#endif
