#include "test_kalloc.h"
#include "kalloc.h"
#include "screen.h"
#include "sys.h"

u32int
test_kalloc_same_alloc() {
	void *loc1, *loc2;

	loc1 = kalloc(128);
	if(loc1 == (void *)0) {
		FAIL("kalloc(128) returned NULL");
	}
	kfree(loc1);
	loc2 = kalloc(128);
	if(loc1 != loc2) {
		kfree(loc2);
		FAIL("kalloc, free, kalloc did not return same address");
	}
	kfree(loc2);
	PASS();
}

u32int
test_kalloc_merge() {
	void *locations[64];
	void *loc1, *loc2;
	u32int i;

	/* make lots of small allocs */
	for(i = 0; i < 64; i++) {
		if((locations[i] = kalloc(16)) == (void *)0) {
			FAIL("kalloc(16) returned NULL");
		}
	}

	/* free even allocs */
	for(i = 0; i < 64; i += 2) {
		kfree(locations[i]);
	}

	/* free odd allocs */
	for(i = 1; i < 64; i += 2) {
		kfree(locations[i]);
	}

	/* make large alloc */
	loc1 = kalloc(16 * 64);
	if(loc1 == (void *)0) {
		FAIL("kalloc(16 * 64) returned NULL");
	}

	/* large alloc should be in the same location as first small alloc */
	if(loc1 != locations[0]) {
		FAIL("kalloc(16 * 64) does not equal first kalloc(16)");
	}

	/* make second small alloc */
	loc2 = kalloc(16);
	if(loc2 == (void *)0) {
		FAIL("last kalloc(16) returned NULL");
	}

	/* last small alloc should be contiguous with larger alloc */
	if((u8int *)loc2 != (u8int *)loc1 + 16 * 65) { /* 65 to account for kalloc bookkeeping */
		kfree(loc1);
		kfree(loc2);
		FAIL("last small alloc does not come directly after first alloc");
	}
	kfree(loc1);
	kfree(loc2);
	PASS();
}

u32int
test_kalloc_large_alloc() {
	void *ptr = kalloc(0xF0000000);
	if(ptr != (void *)0) {
		FAIL("kalloc(0xF0000000) returned non-NULL");
	}
	PASS();
}

u32int
run_test(const char *name, u32int (*test)(void)) {
	scr_set_color(COLOR_GREY, COLOR_BLACK);
	scr_puts("     | ");
	scr_puts(name);

	return test();
}

u32int
test_kalloc_suite() {
	u32int ret_val = 1;
	char *names[] = {"same alloc", "stress merge", "large alloc"};
	u32int (*funcs[])(void) = {test_kalloc_same_alloc, test_kalloc_merge,
							   test_kalloc_large_alloc};
	u32int num_tests = 3;
	u32int i;

	scr_puts("Test suite for kalloc:\n");
	for(i = 0; i < num_tests; i++) {
		if(!run_test(names[i], funcs[i])) {
			ret_val = 0;
			break;
		}
	}
	scr_set_color(COLOR_GREY, COLOR_BLACK);
	return ret_val;
}
