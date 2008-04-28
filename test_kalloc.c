#include "test_kalloc.h"
#include "kalloc.h"
#include "screen.h"
#include "sys.h"

int
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

int
test_kalloc_merge() {
	void *locations[64];
	void *loc1, *loc2;
	int i;

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

	/* last small alloc should be contiguos with larger alloc */
	if(loc2 != loc1 + 16 * 65) { /* 65 to account for kalloc bookkeeping */
		kfree(loc1);
		kfree(loc2);
		FAIL("last small alloc does not come directly after first alloc");
	}
	kfree(loc1);
	kfree(loc2);
	PASS();
}

int
run_test(char *name, int (*test)(void)) {
	scr_set_color(COLOR_GREY, COLOR_BLACK);
	scr_puts("Testing ");
	scr_puts(name);
	scr_puts("... ");

	return test();
}

int
test_kalloc_suite() {
	char *names[] = {"same alloc", "stress merge"};
	int (*funcs[])(void) = {test_kalloc_same_alloc, test_kalloc_merge};
	int num_tests = 2;
	int i;

	scr_puts("Test suite for kalloc:\n");
	for(i = 0; i < num_tests; i++) {
		if(!run_test(names[i], funcs[i])) return 0;
	}
	return 1;
}
