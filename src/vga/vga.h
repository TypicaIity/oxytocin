#pragma once

#include "std/types.h"

#define VGA_WIDTH	80
#define VGA_HEIGHT	25

void vga_cls();
void vga_setcolor(uint8_t fg, uint8_t bg);

void putch(char ch);
void puts(const char* s);
