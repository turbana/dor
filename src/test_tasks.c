#include "test_tasks.h"
#include "scheduler.h"
#include "screen.h"
#include "asm.h"

void pause() {
	ASM("hlt");
}

void task1() {
	while(1) {
		scr_putch('1');
		unblock(3);
		block(2);
		block(4);
		pause();
	}
}

void task2() {
	while(1) {
		scr_putch('2');
		unblock(4);
		block(3);
		block(1);
		pause();
	}
}

void task3() {
	while(1) {
		scr_putch('3');
		unblock(2);
		block(1);
		pause();
	}
}

void task4() {
	while(1) {
		scr_putch('4');
		unblock(1);
		pause();
	}
}
