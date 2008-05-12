#ifndef __VMM_H
#define __VMM_H

#include "types.h"
#include "mm.h"
#include "paging.h"

struct addr_space {
	u32int *page_directory;
	u32int **page_tables;
};

struct addr_space *
addr_space_create();

void
addr_space_destroy(struct addr_space *as);

#endif
