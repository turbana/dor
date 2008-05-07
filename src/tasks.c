#include "tasks.h"

/* defined in boot.asm */
extern u32int __task_save(struct regs state);
extern void   __task_load(struct regs state);

u32int
task_save(struct task *task) {
	return __task_save(task->registers);
}

void
task_load(struct task *task) {
/*	__task_load(task->registers);*/
	u32int *esp;

	task->registers.eax = 1;

	esp = (u32int *)task->registers.esp;
	*esp-- = task->registers.esp;
	*esp-- = task->registers.eflags;
	ASM("mov %%cs, %0\n\t"
		:
		"=m" (*esp--));
	*esp = task->registers.eip;
	task->registers.esp = (u32int)esp;
	ASM("mov %0, %%esp\n\t"
		"popa\n\t"
		"movl -20(%%esp), %%esp\n\t"
		"iret\n\t"
		::
		"m" (task->registers));
}
