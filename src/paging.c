#include "paging.h"
#include "asm.h"

/* Setup a page table mapping the first 4mb of physical memory, then setup the
 * kernel's page directory with the first index and the index of high memory
 * both pointing to the low memory page table. Make sure that both the page
 * table and page directory are aligned on pages.
 */
static unsigned int lowmem_pt[1024] __attribute__((aligned (4096)));
static unsigned int kernel_pd[1024] __attribute__((aligned (4096)));

void
write_page_entry(unsigned int *dest, struct page_entry *entry) {
	*dest = (entry->base_address & 0xFFFFF000) | entry->flags;
}

void
paging_init() {
	int i;
	struct page_entry dir_entry, table_entry;

	table_entry.flags = PAGE_CACHED | PAGE_PRESENT | PAGE_RW;
	dir_entry.flags   = PAGE_CACHED | PAGE_PRESENT | PAGE_RW;

	for(i = 0; i < 1024; i++) {
		/* load lowmem page table ... */
		table_entry.base_address = i * 4096;
		write_page_entry(lowmem_pt + i, &table_entry);
		/* ... and clear the kernel page directory */
		kernel_pd[i] = 0;
	}

	/* Load the kernel page directory with the low mem page table.
	 * As paging is not yet enabled we need a physical address for the base
	 * address. */
	dir_entry.base_address = (unsigned int)VIRT_TO_PHYS(lowmem_pt);
	write_page_entry(kernel_pd + 0, &dir_entry);
	write_page_entry(kernel_pd + 768, &dir_entry);

	/* This is here because we need a pointer to the kernel's page directory,
	 * but the pointer needs to live on the stack in order to use it in inline
	 * assembler. It also must by a physical address as paging hasn't yet been
	 * enabled when we use the pointer. */
	void *page_dir_ptr = VIRT_TO_PHYS(kernel_pd);

	ASM("mov %0, %%eax\n\t"
		"mov %%eax, %%cr3\n\t"		/* use kernel's PD */
		"mov %%cr0, %%eax\n\t"
		"orl $0x80000000, %%eax\n\t"
		"mov %%eax, %%cr0\n\t"		/* now turn it on! */
		::
		"m" (page_dir_ptr));
}
