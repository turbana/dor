#include "sys.h"
#include "screen.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "isrs.h"
#include "timer.h"

void
k_entry(void) {
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	timer_install();
	scr_init();

	__asm__ __volatile__ ("sti");

	scr_puts("Hello Ian.\n");

	for(;;);		/* spin baby, spin */
}
