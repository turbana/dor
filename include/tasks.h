#ifndef __TASKS_H
#define __TASKS_H

#include "asm.h"

/* to get struct regs */
#include "isrs.h"

struct task {
	struct regs registers;
	unsigned int pid;
	enum {
		TS_RUNNING,
		TS_READY,
		TS_BLOCKED
	} status;
};

void task_save(struct task *task);
void task_load(struct task *task);

#endif
