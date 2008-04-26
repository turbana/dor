#include "sys.h"
#include "screen.h"
#include "gdt.h"
#include "idt.h"

void
k_entry(void) {
	gdt_install();
	idt_install();
	scr_init();
	scr_puts("Hello Ian.");
}
