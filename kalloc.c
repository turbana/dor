#include "kalloc.h"

struct memnode {
	char message[4];
	size_t size;
	struct memnode *next;
	struct memnode *prev;
} __attribute__((packed));

struct memnode *free_list;
struct memnode *used_list;

void *
kalloc(size_t size) {
	struct memnode *cur, *used, new;
	unsigned char *loc;
	void *addr = (void *)0;

	for(cur = free_list->next; cur != free_list; cur = cur->next) {
		/* does this node have room for the alloc, and the next free node? */
		if(cur->size >= size + sizeof(struct memnode)) {
			/* make our new free node */
			memcpy(new.message, "FREE", 4);

			/* the size is whatever it used to be minus the new alloc */
			new.size = cur->size - (size + sizeof(struct memnode));

			/* where is it going in memory? */
			loc = (unsigned char *)cur + size + sizeof(struct memnode);

			/* insert it into the free list */
			new.prev = cur->prev;
			cur->prev->next = (struct memnode *)loc;
			new.next = cur->next;
			cur->next->prev = (struct memnode *)loc;

			/* copy it to memory */
			memcpy(loc, &new, sizeof(struct memnode));

			/* set current node to used */
			memcpy(cur->message, "USED", 4);
			cur->size = size;

			/* return the current location */
			addr = (void *)cur + sizeof(struct memnode);

			/* add used node to used list, sorted by memory address */

			/* find node after we need to insert the used node */
			used = used_list->next;
			while(used < cur && used != used_list) {
				used = used->next;
			}

			/* insert node */
			used->prev->next = cur;
			cur->prev = used->prev;
			used->prev = cur;
			cur->next = used;

			break;
		}
	}
	
	return addr;
}

void
kfree(void *ptr) {
	struct memnode *cur, *free;
	int ssize = sizeof(struct memnode);
#ifdef KALLOC_CLEAR
	void *loc;
#endif

	/* if ptr is NULL: noop */
	if(ptr == (void *)0) return;

	/* search for ptr in used list */
	cur = used_list->next;
	while((void *)cur + ssize != ptr && cur != used_list) {
		cur = cur->next;
	}

	/* if it was not found: noop */
	if(cur == used_list) return;

	/* remove from used list */
	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;

	/* find position in free list */
	free = free_list->next;
	while(free < cur && free != free_list) {
		free = free->next;
	}

	/* set as free and add to free list */
	memcpy(cur->message, "FREE", 4);
	free->prev->next = cur;
	cur->prev = free->prev;
	free->prev = cur;
	cur->next = free;

#ifdef KALLOC_CLEAR
	memset((void *)cur + ssize, 0, cur->size);
#endif

	/* we can merge left if the previous pointer is not the free list and
	 * the previous pointer plus it's size is the current pointer */
	if(cur->prev != free_list &&
			(void *)(cur->prev) + cur->prev->size + ssize == cur) {
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
		cur->prev->size += ssize + cur->size;
#ifdef KALLOC_CLEAR
		memset(cur, 0, ssize);
#endif
	}

	/* we can merge right if the next pointer is not the free list and
	 * the current pointer plus it's size is the next pointer */
	if(cur->next != free_list &&
			(void *)cur + cur->size + ssize == cur->next) {
#ifdef KALLOC_CLEAR
		loc = (void *)(cur->next);
#endif
		cur->size += cur->next->size + ssize;
		cur->next->next->prev = cur;
		cur->next = cur->next->next;
#ifdef KALLOC_CLEAR
		memset(loc, 0, ssize);
#endif
	}
}

void
kalloc_init() {
	struct memnode node;
	size_t ssize = sizeof(struct memnode);

	/* dummy head node for our used list */
	memcpy(node.message, "USED", 4);	/* debugging identifier */
	node.size = 0;						/* we're just a dummy node */
	/* wrap around to our self */
	node.prev = (struct memnode *)KMEM_LOW;
	node.next = (struct memnode *)KMEM_LOW;
	memcpy(KMEM_LOW, &node, ssize);

	/* dummy head node for our free list */
	memcpy(node.message, "FREE", 4);
	node.size = 0;
	node.prev = (struct memnode *)(KMEM_LOW + ssize);
	node.next = (struct memnode *)(KMEM_LOW + ssize * 2);
	memcpy(KMEM_LOW + ssize, &node, ssize);

	/* first node in our free list */
	memcpy(node.message, "FREE", 4);
	/* everything, but these 3 nodes are free */
	node.size = KMEM_HIGH - KMEM_LOW - ssize * 3;
	node.prev = (struct memnode *)(KMEM_LOW + ssize);
	node.next = (struct memnode *)(KMEM_LOW + ssize);
	memcpy(KMEM_LOW + ssize * 2, &node, ssize);

	used_list = (struct memnode *)KMEM_LOW;
	free_list = (struct memnode *)(KMEM_LOW + ssize);
}
