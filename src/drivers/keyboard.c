#include "keyboard.h"

#include "drivers/serial.h"

static const char scancodeTable[] = {
	0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
	'\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
	0,'a','s','d','f','g','h','j','k','l',';','\'','`',
	0,'\\','z','x','c','v','b','n','m',',','.','/',0,'*',
	0,' '
};

int getchar() {
	while (true) {
		while (!(inb(0x64) & 1)) {}

		uint8_t sc = inb(0x60);
		if (sc > 57)
			continue;

		char c = scancodeTable[sc];
		if (!c)
			continue;

		return c;
	}
}

int getkey() {
	while (!(inb(0x64) & 1)) {}
	return inb(0x60);
}

size_t gets(char* buffer, size_t size) {
	if (!buffer || !size)
		return 0;
	size_t len = 0;

	while (true) {
		char c = getchar();

		if (c == '\n') {
			buffer[len] = 0;
			return len;
		}

		if (c == '\b') {
			if (len > 0) {
				len--;
				buffer[len] = 0;
			}
			continue;
		}

		buffer[len++] = c;
		if (len >= size)
			return len;
	}
}