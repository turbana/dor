#ifndef __H_ISRS
#define __H_ISRS

/* all ISRs are defined in boot.asm */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

/* stack frame struct */
struct regs {
	unsigned int gs, fs, es, ds;		/* segments */
	unsigned int edi, esi, ebp, esp;	/* stack pointers */
	unsigned int ebx, edx, ecx, eax;	/* registers */
	unsigned int int_no, err_code;		/* interrupt number and error code */
	unsigned int eip, cs, eflags, useresp, ss; /* processor flags */
};

void isrs_install();
void fault_handler(struct regs *);

#endif
