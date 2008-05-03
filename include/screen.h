#ifndef __SCREEN_H
#define __SCREEN_H

/* standard colors */
/* to get bright colors or with COLOR_BRIGHT */
#define COLOR_BLACK		0
#define COLOR_BLUE		1
#define COLOR_GREEN		2
#define COLOR_CYAN		3
#define COLOR_RED		4
#define COLOR_MAGENTA	5
#define COLOR_BROWN		6
#define COLOR_GREY		7

#define COLOR_BRIGHT	8

#define VID_MEM			0xB8000

void scr_scroll(void);
void scr_update_csr(void);
void scr_clear(void);
void scr_putch(char value);
void scr_puts(char *text);
void scr_set_color(unsigned char foreground, unsigned char background);
void screen_init(void);
void scr_putp32(void *ptr);

#endif
