#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "types.h"
#include "stdarg.h"

void *
memcpy(void *dest, const void *source, size_t count);

void *
memset(void *dest, u8int value, size_t count);

u16int *
memsetw(u16int *dest, u16int value, size_t count);

u32int
strlen(const char *string);

void *
strcpy(char *dest, char *src);

u32int
strcmp(char *left, char *right);

u8int
inb(u16int _port);

void
outb(u16int _port, u8int _data);

void
dump_mem(void *low_mem, size_t length);

void
kprintf(const char *fmt, ...);

/* GCC puts in calls to __stack_chk_fail whenever it detects that the stack has
 * been corrupted. We should panic as the stack is no longer valid.
 */
void
__stack_chk_fail();

#endif
