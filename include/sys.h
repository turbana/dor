#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "types.h"

void *
memcpy(void *source, const void *dest, size_t count);

void *
memset(void *dest, u8int value, size_t count);

u16int *
memsetw(u16int *dest, u16int value, size_t count);

size_t
strlen(const u8int *string);

u8int
inb(u16int _port);

void
outb(u16int _port, u8int _data);

void
dump_mem(void *low_mem, size_t length);

#endif
