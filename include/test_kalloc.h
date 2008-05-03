#ifndef __TEST_KALLOC_H
#define __TEST_KALLOC_H

#define FAIL(message) \
	scr_set_color(COLOR_RED | COLOR_BRIGHT, COLOR_BLACK); \
	scr_puts("\rFAIL\n"); \
	scr_puts(message); \
	scr_putch('\n'); \
	scr_set_color(COLOR_GREY, COLOR_BLACK); \
	dump_mem(KMEM_LOW, 256); \
	return 0;

#define PASS() \
	scr_set_color(COLOR_GREEN | COLOR_BRIGHT, COLOR_BLACK); \
	scr_puts("\rPASS\n"); \
	return 1;

int test_kalloc_suite();

#endif
