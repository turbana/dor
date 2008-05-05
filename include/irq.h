#ifndef __IRQ_H
#define __IRQ_H

#include "isrs.h"

typedef void (*irq_func_t)(struct regs *);

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void irq_install_handler(u32int, irq_func_t);
void irq_uninstall_handler(u32int);
void irq_remap();
void irq_init();
void irq_handler(struct regs *);

#endif
