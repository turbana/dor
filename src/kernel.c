#include "kernel.h"
#include "sys.h"
#include "screen.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "isrs.h"
#include "timer.h"
#include "keyboard.h"
#include "kalloc.h"
#include "paging.h"
#include "asm.h"
#include "scheduler.h"

#include "test_kalloc.h"
#include "test_tasks.h"

void
k_entry(void) {
	paging_init();
	gdt_init();
	idt_init();
	isrs_init();
	irq_init();
	screen_init();
	kalloc_init();
	timer_init();
	scheduler_init();
	keyboard_init();

	scr_puts("Hello Ian.\n");

	test_kalloc_suite();

	/* setup some test tasks */
	task_create(task1);
	task_create(task2);
	task_create(task3);
	task_create(task4);

	/* enable interrupts and start scheduling tasks */
	scheduler_start();	/* never exits */
}
