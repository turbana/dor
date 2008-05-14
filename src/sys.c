#include "sys.h"
#include "screen.h"
#include "asm.h"
#include "panic.h"

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

u32int
strlen(const char *string) {
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

void __stack_chk_fail() {
	PANIC("__stack_chk_fail: Kernel stack corrupted");
}

void
_print_uhex(u32int num) {
	char char_map[] = {'0', '1', '2', '3', '4', '5', '6', '7',
					   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	scr_putch(char_map[(num >> 28) & 0xF]);
	scr_putch(char_map[(num >> 24) & 0xF]);
	scr_putch(char_map[(num >> 20) & 0xF]);
	scr_putch(char_map[(num >> 16) & 0xF]);
	scr_putch(char_map[(num >> 12) & 0xF]);
	scr_putch(char_map[(num >> 8)  & 0xF]);
	scr_putch(char_map[(num >> 4)  & 0xF]);
	scr_putch(char_map[num         & 0xF]);
}

static u32int _10_powers[] = {
	1, 10, 100, 1000, 10000, 100000,
	1000000, 10000000, 100000000, 1000000000,
	4294967295U	/* when printing 2**32-1 we need to mod by something larger */
};

void
_print_s32int(s32int num) {
	u32int val, digit, printing = 0;
	u32int foo = (u32int)num;

	if(num < 0) {
		scr_putch('-');
		foo = 0 - num;
	}

	for(digit = 10; digit > 0; digit--) {
		val = (foo % _10_powers[digit]) / _10_powers[digit - 1];
		if(val != 0 || printing || digit == 1) {
			printing = 1;
			scr_putch('0' + val);
		}
	}
}

/*
 * Supported modifiers:
 * 		%d	- s32int
 * 		%X	- uppercase hex of a u32int
 * 		%c	- char
 * 		%s	- string
 * 		%%	- percent (%)
 */
void
kprintf(const char *fmt, ...) {
	va_list args;
	u32int i;

	va_start(args, fmt);
	for(i = 0; i < strlen(fmt); i++) {
		if(fmt[i] != '%') {
			scr_putch(fmt[i]);
		} else {
			i++;
			switch(fmt[i]) {

			case '%':
				scr_putch('%');
				break;

			case 'X': case 'p':
				_print_uhex(va_arg(args, u32int));
				break;

			case 'd':
				_print_s32int(va_arg(args, s32int));
				break;

			case 'c':
				scr_putch(va_arg(args, int));
				break;

			case 's':
				scr_puts(va_arg(args, char *));
				break;
			}
		}
	}
	va_end(args);
}
