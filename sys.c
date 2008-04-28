#include "sys.h"
#include "screen.h"

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

/* Memory Dump Ruler:
0         1         2         3         4         5         6         7
01234567890123456789012345678901234567890123456789012345678901234567890123456789
00200000 65 65 65 65  65 65 65 65  65 65 65 65  65 65 65 65  AAAA AAAA AAAA AAAA
00200010 65 65 65 65  65 65 65 65  65 65 65 65  65 65 65 65  AAAA AAAA AAAA AAAA
*/
void
dump_mem(void *low_mem, size_t length) {
	char char_map[] = {'0', '1', '2', '3', '4', '5', '6', '7',
					   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	char row[81];
	unsigned char *ptr = (unsigned char *)low_mem;
	unsigned char *high = low_mem + length - 1;
	char val;
	int col = 0;
	int rcol = 61;

	memset(row, ' ', 81);
	row[80] = '\0';

	do {
		/* print the memory address */
		if(col == 0) {
			row[col++] = char_map[((unsigned int)ptr >> 28) & 0xF];
			row[col++] = char_map[((unsigned int)ptr >> 24) & 0xF];
			row[col++] = char_map[((unsigned int)ptr >> 20) & 0xF];
			row[col++] = char_map[((unsigned int)ptr >> 16) & 0xF];
			row[col++] = char_map[((unsigned int)ptr >> 12) & 0xF];
			row[col++] = char_map[((unsigned int)ptr >> 8) & 0xF];
			row[col++] = char_map[((unsigned int)ptr >> 4) & 0xF];
			row[col++] = char_map[(unsigned int)ptr & 0xF];
			col++;
		}

		/* hex value */
		row[col++] = char_map[(*ptr >> 4) & 0x0F];	/* high nibble */
		row[col++] = char_map[*ptr & 0x0F];			/* low nibble */

		/* space */
		col++;

		/* ASCII character */
		if(32 <= *ptr && *ptr <= 127) val = *ptr;
		else						  val = '.';
		row[rcol++] = val;

		/* extra spacing */
		switch(col) {
			case 21: case 34: case 47:
				col++;
				rcol++;
				break;
		}

		/* check for end of line */
		if(col == 60) {
			scr_puts(row);
			/* reset the line */
			memset(row, ' ', 80);
			col = 0;
			rcol = 61;
		}
	} while(ptr++ < high);

	/* if we had extra characters print what we have left */
	if(col > 0) {
		scr_puts(row);
	}
}
