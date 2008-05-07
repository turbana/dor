#include "idt.h"
#include "isrs.h"
#include "screen.h"
#include "asm.h"

/* exception error messages */
const char *exception_messages[] = {
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",

	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved"
};


void
isrs_init() {
	idt_set_gate(0,  (u32int)isr0, 0x08, 0x8E);
	idt_set_gate(1,  (u32int)isr1, 0x08, 0x8E);
	idt_set_gate(2,  (u32int)isr2, 0x08, 0x8E);
	idt_set_gate(3,  (u32int)isr3, 0x08, 0x8E);
	idt_set_gate(4,  (u32int)isr4, 0x08, 0x8E);
	idt_set_gate(5,  (u32int)isr5, 0x08, 0x8E);
	idt_set_gate(6,  (u32int)isr6, 0x08, 0x8E);
	idt_set_gate(7,  (u32int)isr7, 0x08, 0x8E);
	idt_set_gate(8,  (u32int)isr8, 0x08, 0x8E);
	idt_set_gate(9,  (u32int)isr9, 0x08, 0x8E);

	idt_set_gate(10, (u32int)isr10, 0x08, 0x8E);
	idt_set_gate(11, (u32int)isr11, 0x08, 0x8E);
	idt_set_gate(12, (u32int)isr12, 0x08, 0x8E);
	idt_set_gate(13, (u32int)isr13, 0x08, 0x8E);
	idt_set_gate(14, (u32int)isr14, 0x08, 0x8E);
	idt_set_gate(15, (u32int)isr15, 0x08, 0x8E);
	idt_set_gate(16, (u32int)isr16, 0x08, 0x8E);
	idt_set_gate(17, (u32int)isr17, 0x08, 0x8E);
	idt_set_gate(18, (u32int)isr18, 0x08, 0x8E);
	idt_set_gate(19, (u32int)isr19, 0x08, 0x8E);

	idt_set_gate(20, (u32int)isr20, 0x08, 0x8E);
	idt_set_gate(21, (u32int)isr21, 0x08, 0x8E);
	idt_set_gate(22, (u32int)isr22, 0x08, 0x8E);
	idt_set_gate(23, (u32int)isr23, 0x08, 0x8E);
	idt_set_gate(24, (u32int)isr24, 0x08, 0x8E);
	idt_set_gate(25, (u32int)isr25, 0x08, 0x8E);
	idt_set_gate(26, (u32int)isr26, 0x08, 0x8E);
	idt_set_gate(27, (u32int)isr27, 0x08, 0x8E);
	idt_set_gate(28, (u32int)isr28, 0x08, 0x8E);
	idt_set_gate(29, (u32int)isr29, 0x08, 0x8E);

	idt_set_gate(30, (u32int)isr30, 0x08, 0x8E);
	idt_set_gate(31, (u32int)isr31, 0x08, 0x8E);
}

void
fault_handler(struct regs *registers) {
	if(registers->int_no < 32) {
		scr_set_color(COLOR_RED | COLOR_BRIGHT, COLOR_BLACK);
		scr_puts(exception_messages[registers->int_no]);
		scr_puts(" Exception. Halting System!\n");
		scr_set_color(COLOR_GREY, COLOR_BLACK);
		register_dump(registers);
		ASM("cli\n\t"
			"hlt");
	}
}

void
register_dump(struct regs *registers) {
	u32int reg;

	/* base registers */
	scr_puts("EAX=");
	scr_putp32((u32int *)registers->eax);

	scr_puts(" EBX=");
	scr_putp32((u32int *)registers->ebx);

	scr_puts(" ECX=");
	scr_putp32((u32int *)registers->ecx);

	scr_puts(" EDX=");
	scr_putp32((u32int *)registers->edx);
	scr_putch('\n');

	/* other registers */
	scr_puts("ESI=");
	scr_putp32((u32int *)registers->esi);

	scr_puts(" EDI=");
	scr_putp32((u32int *)registers->edi);

	scr_puts(" EBP=");
	scr_putp32((u32int *)registers->ebp);

	scr_puts(" ESP=");
	scr_putp32((u32int *)registers->esp);
	scr_putch('\n');

	scr_puts("EIP=");
	scr_putp32((u32int *)registers->eip);

	scr_puts(" EFL=");
	scr_putp32((u32int *)registers->eflags);
	scr_putch('\n');

	/* control registers */
	ASM("mov %%cr0, %0" : "=g" (reg));
	scr_puts("CR0=");
	scr_putp32((u32int *)reg);

	ASM("mov %%cr2, %0" : "=g" (reg));
	scr_puts(" CR2=");
	scr_putp32((u32int *)reg);

	ASM("mov %%cr3, %0" : "=g" (reg));
	scr_puts(" CR3=");
	scr_putp32((u32int *)reg);

	ASM("mov %%cr4, %0" : "=g" (reg));
	scr_puts(" CR4=");
	scr_putp32((u32int *)reg);

	scr_putch('\n');

	/* segment registers */
	scr_puts("CS =");
	scr_putp32((u32int *)registers->cs);

	scr_puts(" DS =");
	scr_putp32((u32int *)registers->ds);

	scr_puts(" SS =");
	scr_putp32((u32int *)registers->ss);

	scr_puts("\nES =");
	scr_putp32((u32int *)registers->es);

	scr_puts(" GS =");
	scr_putp32((u32int *)registers->gs);

	scr_puts(" FS =");
	scr_putp32((u32int *)registers->fs);
	scr_putch('\n');

	/* others */
	scr_puts("INT=");
	scr_putp32((u32int *)registers->int_no);

	scr_puts(" ERR=");
	scr_putp32((u32int *)registers->err_code);
	scr_putch('\n');

	scr_puts("uSP=");
	scr_putp32((u32int *)registers->useresp);
	scr_putch('\n');
}
