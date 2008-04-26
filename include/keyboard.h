#ifndef __H_KEYBOARD
#define __H_KEYBOARD
#include "isrs.h"

void keyboard_handler(struct regs *);
void keyboard_install();

#endif
