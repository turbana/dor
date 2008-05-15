#include "test_tasks.h"
#include "scheduler.h"
#include "sys.h"
#include "asm.h"

void pause() {
	ASM("hlt");
}

void task1() {
	while(1) {
		kprintf("1");
		unblock(3);
		block(2);
		block(4);
		pause();
	}
}

void task2() {
	while(1) {
		kprintf("2");
		unblock(4);
		block(3);
		block(1);
		pause();
	}
}

void task3() {
	while(1) {
		kprintf("3");
		unblock(2);
		block(1);
		pause();
	}
}

void task4() {
	while(1) {
		kprintf("4");
		unblock(1);
		pause();
	}
}

u32int
fib(u32int n) {
	if(n < 2) {
		return n;
	}
	return fib(n-2) + fib(n-1);
}

void task5() {
	u32int i, n;
	while(1) {
		for(i=1; i<50; i++) {
			n = fib(i);
			kprintf("\nThe fibonacci number of %d is %d\n", i, n);
		}
	}
}
