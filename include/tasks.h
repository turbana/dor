#ifndef __TASKS_H
#define __TASKS_H

#include "asm.h"
#include "types.h"

/* to get struct regs */
#include "isrs.h"

struct task {
	struct regs registers;
	unsigned int pid;
	enum {
		TS_RUNNING,
		TS_READY,
		TS_BLOCKED,
		TS_KILLED
	} status;
};

u32int task_save(struct task *task);
void   task_load(struct task *task);

struct task *task_new(void *entry);
void         task_remove(struct task *task);

#endif
