#include "sys.h"
#include "screen.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "isrs.h"

void
k_entry(void) {
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	scr_init();

	scr_puts("Hello Ian.\n");
}
