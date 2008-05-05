#ifndef __PAINC_H
#define __PANIC_H

/* the following are used to provide panic with the filename and line number of
 * the caller of panic().
 */
#define __ESCAPE(x) #x
#define __STRING(x) __ESCAPE(x)
#define __LOCATION  __FILE__ ":" __STRING(__LINE__) " "

void panic(const char *message);
#define PANIC(msg) panic(__LOCATION msg)

#endif
