#include "sys.h"
#include "screen.h"
#include "gdt.h"
#include "idt.h"
#include "isrs.h"

void
k_entry(void) {
	gdt_install();
	idt_install();
	isrs_install();
	scr_init();
	scr_puts("Hello Ian.\n");

	/* test ISR handler */
	int n = 0;
	scr_putch(1 / n);
}
