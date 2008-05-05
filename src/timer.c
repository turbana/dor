#include "timer.h"
#include "sys.h"
#include "irq.h"
#include "screen.h"

u32int timer_ticks = 0;

void
timer_phase(u32int hz) {
	u32int divisor = 1193180 / hz;		/* calculate divisor */
	outb(0x43, 0x36);				/* send command of 0x36 */
	outb(0x40, divisor & 0xFF);		/* send low byte of divisor */
	outb(0x40, divisor >> 8);		/* send high byte of divisor */
}

void
timer_handler(struct regs *registers) {
	timer_ticks++;
	registers = 0;
#ifdef TICK_REMINDER
	static u8int color = COLOR_BLACK;

	if(timer_ticks % 100 == 0) {
		color = (color + 1) % 8;
		scr_set_color(COLOR_GREY, COLOR_BLACK);
		scr_puts("One ");
		scr_set_color(color | COLOR_BRIGHT, COLOR_BLACK);
		scr_puts("second");
		scr_set_color(COLOR_GREY, COLOR_BLACK);
		scr_puts(" has passed.\n");
	}
#endif
}

void timer_init() {
	irq_install_handler(0, timer_handler);	/* install our tick function */
	timer_phase(100);						/* set the clock to 100mhz */
}
