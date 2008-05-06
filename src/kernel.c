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

void
k_entry(void) {
/*int i,j;*/
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

	ASM("sti");

	scr_puts("Hello Ian.\n");

	test_kalloc_suite();

/*i = 0;
j = 1/i;
scr_putch(j);*/


	schedule();

	for(;;);		/* spin baby, spin */
}
