#include "screen.h"
#include "sys.h"

/* VGA tutorial: http://www.brackeen.com/home/vga */

static unsigned short *vidmem;		/* location of video memory */
static int attributes = 0x0700;		/* default to grey on black */
static int crs_y = 0;				/* cursor starts in top-left */
static int crs_x = 0;

void
scr_scroll(void) {
	unsigned short blank, temp;

	/* we only need to scroll when the cursor has gone off the screen */
	if(crs_y >= 25) {
		/* blank space */
		blank = 0x20 | (COLOR_BLACK<<8) | COLOR_GREY;

		/* find the amount of lines we need to scroll */
		temp = crs_y - 25 + 1;
		/* move the video memory back 'temp' lines */
		memcpy(vidmem, vidmem + temp * 80, (25 - temp) * 80 * 2);
		/* blank out the new bottom lines */
		memsetw(vidmem + (25 - temp) * 80, blank, 80);
		/* move our cursor to the last line */
		crs_y = 24;
	}
}

void
scr_update_csr(void) {
	unsigned short location;

	location = crs_y * 80 + crs_x;

	/* tell the VGA controller to move the cursor to the specified location */
	outb(0x3D4, 14);
	outb(0x3D5, location >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, location);
}

void
scr_clear(void) {
	unsigned short blank;
	int row;

	blank = 0x20 | (COLOR_BLACK<<8) | COLOR_GREY;

	for(row=0; row < 25; row++) {
		memsetw(vidmem + row * 80, blank, 80);
	}

	crs_y = crs_x = 0;
	scr_update_csr();
}

void
scr_putch(char value) {
	unsigned short *ptr;
	
	if(value == 0x08) {			/* backspace */
		if(crs_x != 0) {
			crs_x--;
		}
	} else if(value == 0x09) {	/* tab */
		crs_x = (crs_x + 8) & ~7;
	} else if(value == 0x0D) {	/* carriage return */
		crs_x = 0;
	} else if(value == 0x0A) {	/* newline */
		crs_x = 0;
		crs_y++;
	} else if(value >= 0x20) {	/* regular character */
		ptr = vidmem + (crs_y * 80 + crs_x);
		*ptr = value | attributes;
		crs_x++;
	}

	/* did we scroll off the right side? */
	if(crs_x >= 80) {
		crs_x = 0;
		crs_y++;
	}

	scr_scroll();
	scr_update_csr();
}

void
scr_puts(char *string) {
	char *ptr = string;
	while(*ptr != '\0') {
		scr_putch(*ptr++);
	}
}

void
scr_putp32(void *ptr) {
	char char_map[] = {'0', '1', '2', '3', '4', '5', '6', '7',
					   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	unsigned int mem = (unsigned int)ptr;
	char row[9];
	int col = 0;

	row[8] = '\0';

	row[col++] = char_map[((unsigned int)mem >> 28) & 0xF];
	row[col++] = char_map[((unsigned int)mem >> 24) & 0xF];
	row[col++] = char_map[((unsigned int)mem >> 20) & 0xF];
	row[col++] = char_map[((unsigned int)mem >> 16) & 0xF];
	row[col++] = char_map[((unsigned int)mem >> 12) & 0xF];
	row[col++] = char_map[((unsigned int)mem >> 8) & 0xF];
	row[col++] = char_map[((unsigned int)mem >> 4) & 0xF];
	row[col++] = char_map[(unsigned int)mem & 0xF];

	scr_puts(row);
}

void
scr_set_color(unsigned char foreground, unsigned char background) {
	attributes = ((background << 4) | (foreground & 0x0F)) << 8;
}

void
screen_init(void) {
	vidmem = (unsigned short *)VID_MEM;
	scr_clear();
}
