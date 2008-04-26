#include "sys.h"
#include "screen.h"

void
k_entry(void) {
	scr_init();
	scr_puts("Hello Ian.");
}
