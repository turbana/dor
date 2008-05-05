#include "sys.h"
#include "screen.h"
#include "asm.h"

void *
memcpy(void *dest, const void *source, size_t count) {
	const u8int *sptr = (const u8int *)source;
	char *dptr = (char *)dest;
	while(count-- > 0) *dptr++ = *sptr++;
	return dest;
}

void *
memset(void *dest, u8int value, size_t count) {
	u8int *ptr = (u8int *)dest;
	while(count-- > 0) *ptr++ = value;
	return dest;
}

u16int *
memsetw(u16int *dest, u16int value, size_t count) {
	u16int *ptr = (u16int *)dest;
	while(count-- > 0) *ptr++ = value;
	return dest;
}

size_t
strlen(const u8int *string) {
	size_t size;
	for(size=0; *string != '\0'; string++) size++;
	return size;
}

u8int
inb(u16int _port) {
	u8int ret;
	ASM("inb %1, %0"
		: "=a" (ret)
		: "dN" (_port));
	return ret;
}

void
outb(u16int _port, u8int _data) {
	ASM("outb %1, %0"
		::
		"dN" (_port),
		"a"  (_data));
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
	u8int *ptr = (u8int *)low_mem;
	u8int *high = ptr + length - 1;
	u8int val;
	u32int col = 0;
	u32int rcol = 61;

	memset(row, ' ', 81);
	row[80] = '\0';

	scr_puts("Memory dump of 0x");
	scr_putp32(low_mem);
	scr_puts(" to 0x");
	scr_putp32(high);
	scr_puts(":\n");

	do {
		if(col == 0) {
			/* print the memory address */
			scr_putp32(ptr);
			col = 9;
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
			scr_puts(row + 8);
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
