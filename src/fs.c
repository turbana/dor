#include "fs.h"

struct fs_node *fs_root = NULL;


u32int
read_fs(struct fs_node *node, u32int offset, u32int size, u8int *buffer)
{
	if(node == NULL) return 0;
	if(node->read == 0) return 0;
	return node->read(node, offset, size, buffer);
}


u32int
write_fs(struct fs_node *node, u32int offset, u32int size, u8int *buffer)
{
	if(node == NULL) return 0;
	if(node->write == 0) return 0;
	return node->write(node, offset, size, buffer);
}


u32int
open_fs(struct fs_node *node)
{
	if(node == NULL) return 0;
	if(node->open == 0) return 0;
	return node->open(node);
}


u32int
close_fs(struct fs_node *node)
{
	if(node == NULL) return 0;
	if(node->close == 0) return 0;
	return node->close(node);
}


struct dirent *
readdir_fs(struct fs_node *node, u32int index)
{
	if(node == NULL) return 0;
	if((node->flags & FS_DIRECTORY) != FS_DIRECTORY) return 0;
	if(node->readdir == 0) return 0;
	return node->readdir(node, index);
}


struct fs_node *
finddir_fs(struct fs_node *node, char *name)
{
	if(node == NULL) return 0;
	if((node->flags & FS_DIRECTORY) != FS_DIRECTORY) return 0;
	if(node->finddir == 0) return 0;
	return node->finddir(node, name);
}
