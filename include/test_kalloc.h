#ifndef __H_TEST_KALLOC
#define __H_TEST_KALLOC

#define FAIL(message) \
	scr_set_color(COLOR_RED | COLOR_BRIGHT, COLOR_BLACK); \
	scr_puts("FAILED\n"); \
	scr_puts(message); \
	scr_putch('\n'); \
	scr_set_color(COLOR_GREY, COLOR_BLACK); \
	dump_mem(KMEM_LOW, 256); \
	return 0;

#define PASS() \
	scr_set_color(COLOR_GREEN | COLOR_BRIGHT, COLOR_BLACK); \
	scr_puts("PASS\n"); \
	return 1;

int test_kalloc_suite();

#endif
