#ifndef __KALLOC_H
#define __KALLOC_H

#include "sys.h"

/* kfree zeros out freed memory when KALLOC_CLEAR is defined */
#define KALLOC_CLEAR

/* 16mb for the kernel heap */
#define KMEM_LOW	((u8int *)0xC0140000)
#define KMEM_HIGH	((u8int *)0xC1140000)

void *kalloc(size_t size);
void kfree(void *ptr);
void kalloc_init();

#endif
