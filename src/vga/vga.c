#include "vga.h"

#include "std/types.h"

#define DEFAULT_COLOR 0x07

static uint16_t cy = 0;
static uint16_t cx = 0;
static uint8_t col = DEFAULT_COLOR; // light grey on black

static uint16_t* buf = (uint16_t*)0xB8000;

#define setcolor(c) col = c
#define setcursor(x, y) cx, cy = (x), (y)
#define vga_entry(i, ch) buf[i] = ((uint8_t)(ch) | (uint16_t)col << 8)

void vga_cls() {
	//setcolor(DEFAULT_COLOR);
	for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		vga_entry(i, ' ');
	setcursor(0, 0);
}

void vga_setcolor(uint8_t fg, uint8_t bg) {
	setcolor(bg << 4 | (fg & 0x0F));
}

void putch(char ch) {
	if (!ch)
		return;

	if (ch == '\n') {
		cx = 0;
		cy++;

		goto scroll;
	}

	vga_entry(cy * VGA_WIDTH + cx, ch);
	cx++;
	if (cx > VGA_WIDTH) {
		cx = 0;
		cy++;

		// TODO: scrolling
	scroll:
		if (cy > VGA_HEIGHT)
			vga_cls();
	}
}

void puts(const char* s) {
	while (*s)
		putch(*s++);
}
