#include "gdt.h"

void
gdt_set_gate(u32int num, u32int base, u32int limit, u8int access, u8int gran) {
	/* setup base address */
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;

	/* setup descriptor limits */
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = ((limit >> 16) & 0x0F);

	/* setup granularity and access */
	gdt[num].granularity |= (gran & 0xF0);
	gdt[num].access = access;
}

void
gdt_init() {
	/* setup GDT pointer */
	gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
	gp.base = (u32int)&gdt;

	/* NULL descriptor */
	gdt_set_gate(0, 0, 0, 0, 0);

	/* code segment */
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

	/* data segment */
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	gdt_flush();
}
