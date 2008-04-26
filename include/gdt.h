#ifndef __H_GDT
#define __H_GDT

struct gdt_entry {
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__((packed));		/* ensure GCC doesn't optimize this struct */

struct gdt_ptr {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

struct gdt_entry gdt[3];
struct gdt_ptr gp;

extern void gdt_flush();		/* defined in boot.asm */

void gdt_set_gate(int, unsigned long, unsigned long, unsigned char,
				  unsigned char);
void gdt_install();

#endif
