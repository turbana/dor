#include "kalloc.h"

struct memnode {
	u8int  message[4];
	size_t size;
	struct memnode *next;
	struct memnode *prev;
} __attribute__((packed));

struct memnode *free_list;
struct memnode *used_list;

#include "screen.h"

void *
kalloc(size_t size) {
	struct memnode *cur, *used, new;
	u8int *loc;
	u8int *addr = 0;

	/* find the first node that will fit the alloc */
	for(cur = free_list->next; cur != free_list; cur = cur->next) {
		if(cur->size >= size + sizeof(struct memnode)) {
			break;
		}
	}

	/* was the needed room found, or did we wrap around? */
	if(cur != free_list) {
		/* make our new free node */
		memcpy(new.message, "FREE", 4);

		/* the size is whatever it used to be minus the new alloc */
		new.size = cur->size - (size + sizeof(struct memnode));

		/* where is it going in memory? */
		loc = (u8int *)cur + size + sizeof(struct memnode);

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
		addr = (u8int *)cur + sizeof(struct memnode);

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
	}
	
	return addr;
}

void
kfree(void *ptr) {
	struct memnode *cur, *free;
	u32int ssize = sizeof(struct memnode);
#ifdef KALLOC_CLEAR
	u8int *loc;
#endif

	/* if ptr is NULL: noop */
	if(ptr == (void *)0) return;

	/* search for ptr in used list */
	cur = used_list->next;
	while((u8int *)cur + ssize != ptr && cur != used_list) {
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
	memset((u8int *)cur + ssize, 0, cur->size);
#endif

	/* we can merge left if the previous pointer is not the free list and
	 * the previous pointer plus it's size is the current pointer */
	while(cur->prev != free_list &&
			(u8int *)(cur->prev) + cur->prev->size + ssize == (u8int *)cur) {
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
		cur->prev->size += ssize + cur->size;
#ifdef KALLOC_CLEAR
		loc = (u8int *)cur->prev;
		memset(cur, 0, ssize);
		cur = (struct memnode *)loc;
#else
		cur = cur->prev;
#endif
	}

	/* we can merge right if the next pointer is not the free list and
	 * the current pointer plus it's size is the next pointer */
	while(cur->next != free_list &&
			(u8int *)cur + cur->size + ssize == (u8int *)(cur->next)) {
#ifdef KALLOC_CLEAR
		loc = (u8int *)(cur->next);
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
kalloc_init(u8int *heap_start) {
	struct memnode node;
	size_t ssize = sizeof(struct memnode);

	/* dummy head node for our used list */
	memcpy(node.message, "USED", 4);	/* debugging identifier */
	node.size = 0;						/* we're just a dummy node */
	/* wrap around to our self */
	node.prev = (struct memnode *)heap_start;
	node.next = (struct memnode *)heap_start;
	memcpy(heap_start, &node, ssize);

	/* dummy head node for our free list */
	memcpy(node.message, "FREE", 4);
	node.size = 0;
	node.prev = (struct memnode *)(heap_start + ssize);
	node.next = (struct memnode *)(heap_start + ssize * 2);
	memcpy(heap_start + ssize, &node, ssize);

	/* first node in our free list */
	memcpy(node.message, "FREE", 4);
	/* everything, but these 3 nodes are free */
	node.size = KMEM_SIZE - ssize * 3;
	node.prev = (struct memnode *)(heap_start + ssize);
	node.next = (struct memnode *)(heap_start + ssize);
	memcpy(heap_start + ssize * 2, &node, ssize);

	used_list = (struct memnode *)heap_start;
	free_list = (struct memnode *)(heap_start + ssize);
}
