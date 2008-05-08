#ifndef __TASKS_H
#define __TASKS_H

#include "types.h"

struct task {
	u32int esp;
	u32int pid;
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
