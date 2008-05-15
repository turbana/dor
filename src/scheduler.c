#include "scheduler.h"
#include "screen.h"
#include "panic.h"
#include "asm.h"
#include "paging.h"
#include "sys.h"

/* tasks need stacks that are paged aligned */
u8int task_stacks[TASK_MAX_COUNT][4096] __attribute__((aligned (4096)));
struct task all_tasks[TASK_MAX_COUNT];
u32int cur_task = -1;

u32int
task_switch(u32int last_esp) {
	u32int i = cur_task;
	u32int found_task = 0;

	/* save current task's esp */
	all_tasks[i].esp = last_esp;

	/* if the task finished it's timeslice mark it as ready */
	if(all_tasks[i].status == TS_RUNNING) {
		all_tasks[i].status = TS_READY;
	}

	/* if we just ran the idle task pretend we ran task 1 */
	if(cur_task == 0) {
		i = 1;
	}

	/* find the next ready task (round robin) ... */
	do {
		i = (i + 1) % TASK_MAX_COUNT;
		if(i == 0) i = 1;

		if(all_tasks[i].status == TS_READY) {
			found_task = 1;
			break;
		}
	} while(i != cur_task);

	/* if we didn't find a ready task: run the idle task */
	if(!found_task) {
		i = 0;
	}

	/* ... and set that task to running */
	all_tasks[i].status = TS_RUNNING;
	cur_task = i;

	/* return the new task's esp */
	return all_tasks[i].esp;
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

	all_tasks[i].addr_space = addr_space_create();

	/* initialize task's kernel stack */
	stack = (u32int *)(task_stacks[i] + TASK_STACK_SIZE);
	*--stack = 0x202;			/* EFLAGS */
	*--stack = 0x08;			/* CS */
	*--stack = (u32int)entry;	/* EIP */

	*--stack = 0x0;				/* EDI */
	*--stack = 0x0;				/* ESI */
	*--stack = 0x0;				/* EBP */
	*--stack = 0x0;				/* ESP (ignored)
								 * http://www.pagetable.com/?p=8#comment-27 */

	*--stack = 0x0;				/* EBX */
	*--stack = 0x0;				/* EDX */
	*--stack = 0x0;				/* ECX */
	*--stack = 0x0;				/* EAX */

	*--stack = 0x10;			/* DS */
	*--stack = 0x10;			/* ES */
	*--stack = 0x10;			/* FS */
	*--stack = 0x10;			/* GS */

	/* CR3 */
	*--stack = (u32int)VIRT_TO_PHYS(all_tasks[i].addr_space->page_directory);

	all_tasks[i].esp = (u32int)stack;
	all_tasks[i].status = TS_READY;
}

void
block(u32int pid) {
	all_tasks[pid].status = TS_BLOCKED;
}

void
unblock(u32int pid) {
	all_tasks[pid].status = TS_READY;
}

void
_idle_task() {
	while(1) {
		kprintf(".");
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
