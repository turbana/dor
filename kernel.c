#include "sys.h"
#include "screen.h"
#include "gdt.h"

void
k_entry(void) {
	gdt_install();
	scr_init();
	scr_puts("Hello Ian.");
}
