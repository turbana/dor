#include "vmm.h"
#include "kalloc.h"
#include "sys.h"

#include "screen.h"

extern u32int kernel_pd[1024];	/* defined in paging.c */

struct addr_space *
addr_space_create() {
	struct addr_space *as;

	as = (struct addr_space *)kalloc(sizeof(struct addr_space));

	/* eventually we need to set up our own page tables as well, but for now
	 * just use the kernel's page directory. */
	as->page_directory = kernel_pd;

	return as;
}

void
addr_space_destroy(struct addr_space *as) {
	if(as != (struct addr_space *)0) {
		if(as->page_directory != (u32int *)0) {
			kfree(as->page_directory);
			as->page_directory = (u32int *)0;
		}
		kfree(as);
	}
}
