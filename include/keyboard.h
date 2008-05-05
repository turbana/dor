#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "isrs.h"
#include "types.h"

void keyboard_handler(struct regs *);
void keyboard_init();

#endif
