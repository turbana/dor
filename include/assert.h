#ifndef __ASSERT_H
#define __ASSERT_H

#include "panic.h"

#define ASSERT(cond) if(!(cond)) PANIC("ASSERT FAILURE (" __STRING(cond) ")")

#endif
