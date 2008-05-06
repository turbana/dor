#include "scheduler.h"
#include "screen.h"

#include "test_tasks.h"

struct task all_tasks[TASK_MAX_COUNT];

struct task *cur_task;

void
schedule() {
	static u32int current = 0;

	if(task_save(cur_task) != 0) {
		scr_putch('!');
		return;
	}

	if(cur_task->status == TS_RUNNING) {
		cur_task->status = TS_READY;
	}

	do {
		current = (current + 1) % TASK_MAX_COUNT;
		cur_task = &all_tasks[current];
	} while(cur_task->status != TS_READY);

	cur_task->status = TS_RUNNING;
	task_load(cur_task);
}

void idle() {
	schedule();
}

void
scheduler_init() {
	int i;
	static u8int task_stacks[TASK_MAX_COUNT][TASK_STACK_SIZE];
	u32int functions[TASK_MAX_COUNT] = {
		(u32int)idle,
		(u32int)task1,
		(u32int)task2,
		(u32int)task3,
		(u32int)task4
	};

	for(i=0; i<TASK_MAX_COUNT; i++) {
		task_save(&all_tasks[i]);
		all_tasks[i].registers.esp = (u32int)(task_stacks[i+1] -1);
		all_tasks[i].registers.esp = 0x42;
		all_tasks[i].registers.eip = functions[i];
		all_tasks[i].registers.eflags = 0x200; /* interrupt enable flag */
		all_tasks[i].status = TS_READY;
	}

	cur_task = &all_tasks[0];
}
