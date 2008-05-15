#include "panic.h"
#include "screen.h"
#include "sys.h"
#include "asm.h"

void
panic(const char *message) {
	scr_set_color(COLOR_RED | COLOR_BRIGHT, COLOR_BLACK);
	kprintf(message);
	ASM("cli\n\t"
		"hlt");
}
