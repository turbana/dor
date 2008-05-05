#include "panic.h"
#include "screen.h"
#include "asm.h"

void
panic(char *message) {
	scr_set_color(COLOR_RED | COLOR_BRIGHT, COLOR_BLACK);
	scr_puts(message);
	ASM("cli\n\t"
		"hlt");
}
