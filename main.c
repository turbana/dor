#include "system.h"
#include "screen.h"

int
main(void) {
	init_screen();
	puts("Hello Ian.\n");
	/* spin baby, spin! */
	for(;;);
	return 0;
}
