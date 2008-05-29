#include "initrd.h"
#include "sys.h"
#include "assert.h"


struct initrd_file_header *file_headers;
u32int file_count;
struct fs_node root_nodes[33];


static u32int
initrd_read(struct fs_node *node, u32int offset, u32int size, u8int *buffer)
{
	struct initrd_file_header *header = &file_headers[node->inode - 1];

	/* check valid size/offset */
	if(offset > header->length) return 0;

	/* truncate size if needed */
	if(offset + size > header->length) {
		size = header->length - offset;
	}

	memcpy(buffer, (u8int *)(header->offset + offset), size);
	return size;
}


static struct dirent *
initrd_readdir(struct fs_node *node, u32int index)
{
	static struct dirent entry;

	/* ensure that node is the root node */
	if(node != &root_nodes[0]) return NULL;

	/* check valid index */
	if(index > file_count || index == 0) return NULL;

	strcpy(entry.name, root_nodes[index].name);
	entry.ino = root_nodes[index].inode;

	return &entry;
}


static struct fs_node *
initrd_finddir(struct fs_node *node, char *name)
{
	u32int i;

	/* ensure that node is the root node */
	if(node != &root_nodes[0]) return NULL;

	/* search for file that matches name */
	for(i = 1; i <= file_count; i++) {
		if(!strcmp(name, root_nodes[i].name)) {
			return &root_nodes[i];
		}
	}

	return NULL;
}


void
initrd_list()
{
	u32int i, count;
	u8int buffer[256];
	struct fs_node *node, *root;
	struct dirent *entry;

	kprintf("Listing Initial Ramdisk:\n");

	i = 1;
	root = &root_nodes[0];

	while((entry = readdir_fs(root, i++)) != NULL) {
		node = finddir_fs(root, entry->name);
		ASSERT(node != NULL);

		if(node->flags & FS_DIRECTORY) {
			kprintf("Found directory %s\n", node->name);
		} else {
			kprintf("Found file %s (%d bytes):\n", node->name, node->length);
			count = read_fs(node, 0, 255, buffer);
			buffer[count] = '\0';
			kprintf("\t%s\n", buffer);
		}
	}
}


struct fs_node *
initrd_init(u32int location)
{
	struct initrd_file_header *header;
	struct fs_node *root, *node;
	u32int i;

	file_count = *(u32int *)location;
	file_headers = (struct initrd_file_header *)(location + sizeof(u32int));

	root = &root_nodes[0];
	strcpy(root->name, "root");
	root->flags = FS_DIRECTORY;
	root->inode = 0;
	root->length = 0;
	root->impl = 0;
	root->ptr = NULL;
	root->read = NULL;
	root->write = NULL;
	root->open = NULL;
	root->close = NULL;
	root->readdir = &initrd_readdir;
	root->finddir = &initrd_finddir;
/*dump_mem((void *)location, 64);
kprintf("%d\n", file_count);
kprintf("0x%X\n", file_headers);*/

	for(i = 0; i < file_count; i++) {
		node = &root_nodes[i + 1];
		header = &file_headers[i];

/*kprintf("0x%X\n", header->magic);*/
		ASSERT(header->magic == INITRD_MAGIC);

		/* offset is relative to start of initrd, make it absolute */
		header->offset += location;

		strcpy(node->name, header->name);

		node->length = header->length;
		node->flags = FS_FILE;
		node->inode = i + 1;
		node->impl = 0;
		node->ptr = NULL;
		node->read = &initrd_read;
		node->write = NULL;
		node->open = NULL;
		node->close = NULL;
		node->readdir = NULL;
		node->finddir = NULL;
	}

	return root;
}
