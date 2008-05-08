#include "mm.h"
#include "kalloc.h"
#include "sys.h"
#include "panic.h"

u8int *bitmap;
u32int buckets;

u32int
frame_aquire() {
	u32int byte, bit;

	for(byte = K_RESERVED; byte < buckets; byte++) {
		if(bitmap[byte] < 0xFF) {
			for(bit = 0; bit < 8; bit++) {
				if(((bitmap[byte] >> bit) & 1) == 0) {
					bitmap[byte] |= 1 << bit;
					return (byte * 8 + bit) * 4096;
				}
			}
		}
	}

	return 0;
}

void
frame_release(u32int frame) {
	u32int byte, bit;

	byte = (frame / 4096) / 8;
	bit  = (frame / 4096) % 8;

	if(byte < K_RESERVED) {
		PANIC("Tried to free page frame within kernel reserved memory");
	}

	bitmap[byte] &= ~(1 << bit);
}

u32int
frame_naquire(u32int *frames, u32int count) {
	u32int i;
	for(i = 0; i < count; i++) {
		frames[i] = frame_aquire();
		if(frames[i] == 0) break;
	}
	return i;
}

void
mm_init(u32int ram_size) {
	buckets = ram_size / 4096 / 8;
	bitmap = (u8int *)kalloc(buckets);
	memset(bitmap, 0, buckets);

	/* first 18mb of memory is reserved for the kernel */
	memset(bitmap, 0xFF, K_RESERVED);
}
