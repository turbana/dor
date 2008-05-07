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
