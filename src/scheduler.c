#include "scheduler.h"
#include "screen.h"
#include "panic.h"
#include "asm.h"

struct task all_tasks[TASK_MAX_COUNT];
u8int task_stacks[TASK_MAX_COUNT][TASK_STACK_SIZE];
u32int cur_task = 0;

u32int
task_switch(u32int last_esp) {
	struct task *current = all_tasks + cur_task;
	u32int i = cur_task;

	/* save current task's esp */
	current->esp = last_esp;

	/* if the task finished it's timeslice mark it as ready */
	if(current->status == TS_RUNNING) {
		current->status = TS_READY;
	}

	/* find the next ready task (round robin) ... */
	do {
		i = (i + 1) % TASK_MAX_COUNT;
		current = &all_tasks[i];
	} while(current->status != TS_READY);

	/* ... and set that task to running */
	current->status = TS_RUNNING;
	cur_task = i;

	/* return the new task's esp */
	return current->esp;
}

void
task_create(void (*entry)()) {
	u32int *stack;
	u32int i;

	/* find the first open task slot */
	for(i=0; i<TASK_MAX_COUNT; i++) {
		if(all_tasks[i].status == TS_KILLED) {
			break;
		/* fail if we reached the end and didn't find a killed task */
		} else if(i == TASK_MAX_COUNT - 1) {
			PANIC("Task limit reached");
		}
	}

	/* initialize task's kernel stack */
	stack = (u32int *)(task_stacks[i] + TASK_STACK_SIZE);
	*--stack = 0x202;			/* EFLAGS */
	*--stack = 0x08;			/* CS */
	*--stack = (u32int)entry;	/* EIP */

	*--stack = 0x0;				/* EDI */
	*--stack = 0x0;				/* ESI */
	*--stack = 0x0;				/* EBP */
	*--stack = 0x0;				/* NULL */

	*--stack = 0x0;				/* EBX */
	*--stack = 0x0;				/* EDX */
	*--stack = 0x0;				/* ECX */
	*--stack = 0x0;				/* EAX */

	*--stack = 0x10;			/* DS */
	*--stack = 0x10;			/* ES */
	*--stack = 0x10;			/* FS */
	*--stack = 0x10;			/* GS */

	all_tasks[i].esp = (u32int)stack;
	all_tasks[i].status = TS_READY;
}

void
_idle_task() {
	while(1) {
		scr_putch('.');
		ASM("hlt");
	}
}

void
scheduler_init() {
	u32int i;

	/* all the task slots are ready to be used */
	for(i=0; i<TASK_MAX_COUNT; i++) {
		all_tasks[i].status = TS_KILLED;
	}

	task_create(_idle_task);
}

void
scheduler_start() {
	ASM("push %0\n\t"		/* push pointer to idle task */
		"sti\n\t"			/* turn on interrupts */
		"ret\n\t"			/* jump to the idle task */
		::
		"Ir" (_idle_task));
}
