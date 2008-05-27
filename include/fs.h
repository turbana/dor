#ifndef __FS_H
#define __FS_H

#include "types.h"


/* directory entry. returned by readdir() */
struct dirent {
	char name[128];				/* directory name */
	u32int ino;					/* inode */
};


/* filesystem node */
struct fs_node {
	char name[128];				/* filename */
	u32int flags;				/* flags (includes type) */
	u32int inode;				/* inode */
	u32int length;				/* filesize */
	u32int impl;				/* implementation defined */
	struct fs_node *ptr;		/* node pointer (used by symlinks and
								   mountpoints) */
	/* function pointers */
	u32int (*read)(struct fs_node *node, u32int offset, u32int size,
				   u8int *buffer);
	u32int (*write)(struct fs_node *node, u32int offset, u32int size,
				    u8int *buffer);
	u32int (*open)(struct fs_node *node);
	u32int (*close)(struct fs_node *node);
	struct dirent *(*readdir)(struct fs_node *node, u32int index);
	struct fs_node *(*finddir)(struct fs_node *node, char *name);
};


/* filesystem node types */
#define FS_FILE			0x01
#define FS_DIRECTORY	0x02
#define FS_CHARDEVICE	0x03
#define FS_BLOCKDEVICE	0x04
#define FS_PIPE			0x05
#define FS_SYMLINK		0x06
/* FS_MOUNTPOINT defined as 0x08 so it can be OR'd to fs_DIRECTORY */
#define FS_MOUNTPOINT	0x08


/* standard filesystem functions */
u32int
read_fs(struct fs_node *node, u32int offset, u32int size, u8int *buffer);

u32int
write_fs(struct fs_node *node, u32int offset, u32int size, u8int *buffer);

u32int
open_fs(struct fs_node *node);

u32int
close_fs(struct fs_node *node);

struct dirent *
readdir_fs(struct fs_node *node, u32int index);

struct fs_node *
finddir_fs(struct fs_node *node, char *name);


extern struct fs_node *fs_root;


#endif
