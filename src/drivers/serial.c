#include "serial.h"

#define COM1 0x3F8

void serial_init() {
	outb(COM1 + 1, 0x00);
	outb(COM1 + 3, 0x80);
	outb(COM1 + 0, 0x03);
	outb(COM1 + 1, 0x00);
	outb(COM1 + 3, 0x03);
	outb(COM1 + 2, 0xC7);
	outb(COM1 + 4, 0x0B);
}

void serial_write(const char* s) {
	for (const char* p = s; *p; p++) {
		char ch = *p;
		while (!(inb(COM1 + 5) & 0x20));
		outb(COM1, ch);
	}
}