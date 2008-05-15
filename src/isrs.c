#include "idt.h"
#include "isrs.h"
#include "screen.h"
#include "asm.h"
#include "sys.h"

#include "scheduler.h"
extern u32int cur_task;

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
		kprintf("%s Exception. Halting System!\n",
				exception_messages[registers->int_no]);
		scr_set_color(COLOR_GREY, COLOR_BLACK);
		register_dump(registers);
		ASM("cli\n\t"
			"hlt");
	}
}

void
register_dump(struct regs *registers) {
	u32int cr0, cr2, cr3, cr4;

	/* read in control registers */
	ASM("mov %%cr0, %0" : "=g" (cr0));
	ASM("mov %%cr2, %0" : "=g" (cr2));
	ASM("mov %%cr3, %0" : "=g" (cr3));
	ASM("mov %%cr4, %0" : "=g" (cr4));

	/* base registers */
	kprintf("EAX=%X EBX=%X ECX=%X EDX=%X\n"
			"ESI=%X EDI=%X EBP=%X ESP=%X\n"
			"EIP=%X EFL=%X\n"
			"CR0=%X CR2=%X CR3=%X CR4=%X\n"
			"CS =%X DS =%X SS =%X\n"
			"ES =%X GS =%X FS =%X\n"
			"INT=%X ERR=%X\n"
			"uSP=%X TID=%d\n",
			registers->eax, registers->ebx, registers->ecx, registers->edx,
			registers->esi, registers->edi, registers->ebp, registers->esp,
			registers->eip, registers->eflags,
			cr0, cr2, cr3, cr4,
			registers->cs, registers->ds, registers->ss,
			registers->es, registers->gs, registers->fs,
			registers->int_no, registers->err_code,
			registers->useresp, cur_task);
}
