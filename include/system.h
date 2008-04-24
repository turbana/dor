#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "types.h"

void *
memcpy(void *source, const void *dest, size_t count);

void *
memset(void *dest, char value, size_t count);

unsigned short *
memsetw(unsigned short *dest, unsigned short value, size_t count);

size_t
strlen(const char *string);

unsigned char
inportb(unsigned short _port);

void
outportb(unsigned short _port, unsigned char _data);

#endif
