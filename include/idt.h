#ifndef __IDT_H
#define __IDT_H

#include "types.h"

struct idt_entry {
	u16int	base_low;
	u16int	segment;
	u8int	always0;
	u8int	flags;
	u16int	base_high;
} __attribute__((packed));

struct idt_ptr {
	u16int limit;
	u32int base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load();				/* defined in boot.asm */

void idt_set_gate(u8int num, u32int base, u16int segment, u8int flags);
void idt_init();

#endif
