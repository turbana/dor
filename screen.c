#include "system.h"
#include "screen.h"

/* VGA tutorial: http://www.brackeen.com/home/vga */

void
scroll(void) {
	unsigned short blank, temp;

	/* we only need to scroll when the cursor has gone off the screen */
	if(crs_y >= 25) {
		/* blank space */
		blank = 0x20 | (COLOR_BLACK<<8) | COLOR_WHITE;

		/* find the amount of lines we need to scroll */
		temp = crs_y - 25 + 1;
		/* move the video memory back 'temp' lines */
		memcpy(vidmem, vidmem + temp * 80, (25 - temp) * 80 * 2);
		/* blank out the new bottom lines */
		memsetw(vidmem + (25 - temp) * 80, blank, 80);
		/* move our cursor to the last line */
		csr_y = 24;
	}
}

void
move_csr(void) {
	unsigned short location;

	location = csr_y * 80 + csr_x;

	/* tell the VGA controller to move the cursor to the specified location */
	outportb(0x3D4, 14);
	outportb(0x3D5, temp >> 8);
	outportb(0x3D4, 15);
	outportb(0x3D5, temp);
}

void
clear(void) {
	unsigned short blank;
	int row;

	blank = 0x20 | (COLOR_BLACK<<8) | COLOR_WHITE;

	for(row=0; row < 25; row++) {
		memsetw(vidmem + i * 80, blank, 80);
	}

	csr_y = csr_x = 0;
	move_csr();
}

void
putch(unsigned char value) {

}
