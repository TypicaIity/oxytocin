#include "std/mem.h"

void kmain() {
	uint16_t* p = (uint16_t*)0xB8000;
	for (int i = 0; i < 2000; i++) p[i] = 1824;

	*(char*)p++ = 'O';
	*(char*)p++ = 'X';
	*(char*)p++ = 'Y';
}
