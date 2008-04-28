#include "sys.h"
#include "screen.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "isrs.h"
#include "timer.h"
#include "keyboard.h"
#include "kalloc.h"

void
k_entry(void) {
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	scr_init();
	kalloc_init();
	timer_install();

	keyboard_install();

	__asm__ __volatile__ ("sti");

	scr_puts("Hello Ian.\n");

	char *ptr1 = (char *)kalloc(16);
	char *ptr2 = (char *)kalloc(16);
	if(ptr1 != 0) memset(ptr1, '1', 16);
	if(ptr2 != 0) memset(ptr2, '2', 16);
	kfree(ptr2);
	char *ptr3 = (char *)kalloc(32);
	if(ptr3 != 0) memset(ptr3, '3', 32);
	kfree(ptr1);

	scr_puts("ptr1 = 0x");
	scr_putp32(ptr1);
	scr_puts("\nptr2 = 0x");
	scr_putp32(ptr2);
	scr_puts("\nptr3 = 0x");
	scr_putp32(ptr3);
	scr_putch('\n');
	dump_mem(KMEM_LOW, 256);

	for(;;);		/* spin baby, spin */
}
