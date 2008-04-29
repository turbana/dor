#include "keyboard.h"
#include "screen.h"
#include "irq.h"
#include "sys.h"

unsigned char kbdus[128] = {
/* US Keyboard  0     1     2     3     4     5     6     7     8     9 */
/*    0 */		0,    27,   '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',
/*    1 */		'9',  '0',  '-',  '=',  '\b', '\t', 'q',  'w',  'e',  'r',
/*    2 */		't',  'y',  'u',  'i',  'o',  'p',  '[',  ']',  '\n', 0,
/*    3 */		'a',  's',  'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',
/*    4 */		'\'', '`',  0,    '\\', 'z',  'x',  'c',  'v',  'b',  'n',
/*    5 */		'm',  ',',  '.',  '/',  0,    '*',  0,    ' ',  0,    0,
/*    6 */		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
/*    7 */		0,    0,    0,    0,    '-',  0,    0,    0,    '+',  0,
/*    8 */		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};

void
keyboard_handler(struct regs *registers) {
	unsigned char scancode;

	/* read from keyboard */
	scancode = inb(0x60);

	if(scancode & 0x80) {
		/* key released, noop */
	} else {
		/* key pressed */
		scr_putch(kbdus[scancode]);
	}
}

void keyboard_install() {
	irq_install_handler(1, keyboard_handler);
}
