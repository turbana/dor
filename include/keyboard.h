#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include "isrs.h"

void keyboard_handler(struct regs *);
void keyboard_init();

#endif
