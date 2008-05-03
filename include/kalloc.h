#ifndef __KALLOC_H
#define __KALLOC_H

#include "sys.h"

/* kfree zeros out freed memory when KALLOC_CLEAR is defined */
#define KALLOC_CLEAR

/* kernel memory lives between 2mb and 10mb */
#define KMEM_LOW	((unsigned char *)0x200000)
#define KMEM_HIGH	((unsigned char *)0xA00000)

void *kalloc(size_t size);
void kfree(void *ptr);
void kalloc_init();

#endif
