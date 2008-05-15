#ifndef __TEST_KALLOC_H
#define __TEST_KALLOC_H

#include "types.h"
#include "sys.h"

#define FAIL(message) \
	scr_set_color(COLOR_RED | COLOR_BRIGHT, COLOR_BLACK); \
	kprintf("\rFAIL\n%s\n", message); \
	scr_set_color(COLOR_GREY, COLOR_BLACK); \
	dump_mem(KMEM_LOW, 256); \
	return 0;

#define PASS() \
	scr_set_color(COLOR_GREEN | COLOR_BRIGHT, COLOR_BLACK); \
	kprintf("\rPASS\n"); \
	scr_set_color(COLOR_GREY, COLOR_BLACK); \
	return 1;

u32int test_kalloc_suite();

#endif
