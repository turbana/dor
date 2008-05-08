#include "paging.h"
#include "asm.h"

/*
 * Identity map the first 20mb of physical memory. This is enough for the kernel
 * binary and heap space. We then map the first 20mb of kernel virtual memory
 * starting at 0xC0000000, this lets us far jump into the kernel loaded at high
 * memory.
 */
static u32int lowmem_pts[5][1024] __attribute__((aligned (4096)));
static u32int kernel_pd[1024] __attribute__((aligned (4096)));

void
write_page_entry(u32int *dest, struct page_entry *entry) {
	*dest = (entry->base_address & 0xFFFFF000) | entry->flags;
}

void
paging_init() {
	u32int i, j;
	struct page_entry dir_entry, table_entry;
	void *page_dir_ptr;

	table_entry.flags = PAGE_CACHED | PAGE_PRESENT | PAGE_RW;
	dir_entry.flags   = PAGE_CACHED | PAGE_PRESENT | PAGE_RW;

	for(j = 0; j < 5; j++) {
		for(i = 0; i < 1024; i++) {
			/* load lowmem page tables ... */
			table_entry.base_address = (j * 1024 * 4096) + (i * 4096);
			write_page_entry(&lowmem_pts[j][i], &table_entry);
			/* ... and clear the kernel page directory */
			kernel_pd[i] = 0;
		}
	}

	/* Load the kernel page directory with the low mem page table.
	 * As paging is not yet enabled we need a physical address for the base
	 * address. */
	j = 768;	/* page directory entry for 0xC0000000 */
	for(i = 0; i < 5; i++) {
		dir_entry.base_address = (u32int)VIRT_TO_PHYS(&lowmem_pts[i]);
		write_page_entry(kernel_pd + i, &dir_entry);
		write_page_entry(kernel_pd + j + i, &dir_entry);
	}

	/* This is here because we need a pointer to the kernel's page directory,
	 * but the pointer needs to live on the stack in order to use it in inline
	 * assembler. It also must by a physical address as paging hasn't yet been
	 * enabled when we use the pointer. */
	page_dir_ptr = VIRT_TO_PHYS(kernel_pd);

	ASM("mov %0, %%eax\n\t"
		"mov %%eax, %%cr3\n\t"		/* use kernel's PD */
		"mov %%cr0, %%eax\n\t"
		"orl $0x80000000, %%eax\n\t"
		"mov %%eax, %%cr0\n\t"		/* now turn it on! */
		::
		"m" (page_dir_ptr));
}
