#ifndef __MM_H
#define __MM_H

#include "types.h"

#define K_RESERVED 576

u32int frame_aquire();
void frame_release(u32int frame);
u32int frame_naquire(u32int *frames, u32int count);
void mm_init(u32int ram_size);

#endif
