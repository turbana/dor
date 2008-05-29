#ifndef __INITRD_H
#define __INITRD_H

#include "types.h"
#include "fs.h"


#define INITRD_MAGIC 0xDEED


struct initrd_file_header {
	u16int magic;
	char name[32];
	u32int offset;
	u32int length;
};


struct fs_node *
initrd_init(u32int location);

void
initrd_list();


#endif
