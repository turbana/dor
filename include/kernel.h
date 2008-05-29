#ifndef __KERNEL_H
#define __KERNEL_H

#include "multiboot.h"
#include "types.h"

void k_entry(u32int magic, struct multiboot_header *mboot);

#endif
