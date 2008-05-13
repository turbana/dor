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
#include "mm.h"

#include "test_kalloc.h"
#include "test_tasks.h"

void
k_entry(void) {
	paging_init();
	gdt_init();
	kalloc_init();
	screen_init();

	if(!test_kalloc_suite()) {
		ASM("cli\n\t"
			"hlt");
	}

	idt_init();
	isrs_init();
	irq_init();
	timer_init();
	mm_init(32 * 1024 * 1024);
	scheduler_init();
	keyboard_init();

	scr_puts("Hello Ian.\n");

	/* setup some test tasks */
	task_create(task1);
	task_create(task2);
	task_create(task3);
	task_create(task4);
	task_create(task5);
	task_create(task6);
	task_create(task7);

	/* enable interrupts and start scheduling tasks */
	scheduler_start();	/* never exits */
}
