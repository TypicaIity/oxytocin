#include "vga.h"

#include "std/types.h"

static uint16_t cx = 0;
static uint16_t cy = 0;
static uint8_t col = 0x07;

static uint16_t* buf = (uint16_t*)0xB8000;

#define setcolor(fg, bg) col = (bg) << 4 | ((fg) & 0x0F)
#define vga_entry(i, ch) buf[i] = ((uint8_t)(ch) | col << 8)

void vga_cls() {
	for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		vga_entry(i, ' ');
	cx = 0, cy = 0;
}

void vga_setcolor(uint8_t fg, uint8_t bg) {
	setcolor(fg, bg);
}

void putch(char ch) {
	switch (ch) {
	case 0:
		return;
	case '\n':
		cx = 0;
		cy++;
		goto scroll;
	case '\t':
		vga_entry(cy * VGA_WIDTH + cx++, ' ');
		vga_entry(cy * VGA_WIDTH + cx++, ' ');
		vga_entry(cy * VGA_WIDTH + cx++, ' ');
		vga_entry(cy * VGA_WIDTH + cx++, ' ');
		break;
	case '\b':
		vga_entry(cy * VGA_WIDTH + --cx, ' ');
		break;
	default:
		break;
	}

	vga_entry(cy * VGA_WIDTH + cx, ch);

	cx++;
	if (cx > VGA_WIDTH) {
		cy++;

	// TODO:
	scroll:
		if (cy > VGA_HEIGHT)
			vga_cls();
	}
}

void puts(const char* s) {
	while (*s)
		putch(*s++);
}
