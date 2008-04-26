#include "sys.h"

void *
memcpy(void *dest, const void *source, size_t count) {
	const char *sptr = (const char *)source;
	char *dptr = (char *)dest;
	while(count-- > 0) *dptr++ = *sptr++;
	return dest;
}

void *
memset(void *dest, char value, size_t count) {
	char *ptr = (char *)dest;
	while(count-- > 0) *ptr++ = value;
	return dest;
}

unsigned short *
memsetw(unsigned short *dest, unsigned short value, size_t count) {
	unsigned short *ptr = (unsigned short *)dest;
	while(count-- > 0) *ptr++ = value;
	return dest;
}

size_t
strlen(const char *string) {
	size_t size;
	for(size=0; *string != '\0'; string++) size++;
	return size;
}

unsigned char
inb(unsigned short _port) {
	unsigned char ret;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (ret) : "dN" (_port));
	return ret;
}

void
outb(unsigned short _port, unsigned char _data) {
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}
