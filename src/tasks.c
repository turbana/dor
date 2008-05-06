#include "tasks.h"

/* defined in boot.asm */
extern void __task_save(struct regs state);
extern void __task_load(struct regs state);

void
task_save(struct task *task) {

}

void
task_load(struct task *task) {

}
