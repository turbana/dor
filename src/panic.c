#include "panic.h"
#include "screen.h"

void
panic(char *message) {
	scr_set_color(COLOR_RED | COLOR_BRIGHT, COLOR_BLACK);
	scr_puts(message);
	__asm__ __volatile("cli\n\thlt");
}
