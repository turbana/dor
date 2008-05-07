#include "test_tasks.h"
#include "scheduler.h"
#include "screen.h"

void pause() {
	ASM("hlt");
}

void task1() {
	while(1) {
		scr_putch('1');
		pause();
	}
}

void task2() {
	while(1) {
		scr_putch('2');
		pause();
	}
}

void task3() {
	while(1) {
		scr_putch('3');
		pause();
	}
}

void task4() {
	while(1) {
		scr_putch('4');
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
			scr_puts("\nThe fibonacci number of ");
			scr_putp32((u32int *)i);
			scr_puts(" is ");
			scr_putp32((u32int *)n);
			scr_putch('\n');
		}
	}
}
