#pragma once

#define VGA_WIDTH	80
#define VGA_HEIGHT	25

void vga_cls();

void putch(char ch);
void puts(const char* s);
