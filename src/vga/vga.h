//
// Created by felix on 9/26/2025.
//

// COPYRIGHT Fentanyl LLC 2025
// v0.0.1-beta

// MIT LICENSE

#pragma once

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void vga_cls();
void vga_setcolor(uint8_t fg, uint8_t bg);

void putch(char ch);
void puts(const char* s);
