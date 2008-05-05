#include "idt.h"
#include "sys.h"

void
idt_set_gate(u8int num, u32int base, u16int segment, u8int flags) {
	/* setup base address */
	idt[num].base_low = (base & 0xFFFF);
	idt[num].base_high = (base >> 16) & 0xFFFF;

	/* setup segment and flags */
	idt[num].segment = segment;
	idt[num].always0 = 0;
	idt[num].flags = flags;
}

void
idt_init() {
	/* setup the IDT pointer */
	idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
	idtp.base = (u32int)&idt;

	/* clear out the IDT */
	memset(&idt, 0, sizeof(struct idt_entry) * 256);

	idt_load();
}
