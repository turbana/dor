#include "sys.h"
#include "screen.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "isrs.h"
#include "timer.h"
#include "keyboard.h"
#include "kalloc.h"

#include "test_kalloc.h"

void
k_entry(void) {
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	scr_init();
	kalloc_init();
	timer_install();

	keyboard_install();

	__asm__ __volatile__ ("sti");

	scr_puts("Hello Ian.\n");

	test_kalloc_suite();

	for(;;);		/* spin baby, spin */
}
