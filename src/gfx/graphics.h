#pragma once

//					  ..RRGGBB
#define GFX_BLACK	0x00000000
#define GFX_WHITE	0x00FFFFFF
#define GFX_RED		0x00FF0000
#define GFX_GREEN	0x0000FF00
#define GFX_BLUE	0x000000FF
#define GFX_YELLOW	0x00FFFF00
#define GFX_CYAN	0x0000FFFF
#define GFX_MAGENTA	0x00FF00FF
#define GFX_ORANGE	0x00FFA500
#define GFX_PURPLE	0x00800080
#define GFX_GRAY	0x00808080
#define GFX_BROWN	0x00A52A2A
#define GFX_PINK	0x00FFC0CB
#define GFX_LIME	0x00BFFF00
#define GFX_NAVY	0x00000080
#define GFX_LGRAY	0x00D3D3D3

void gfx_clear(uint32_t color);
void gfx_putpixel(uint32_t x, uint32_t y, uint32_t color);
void gfx_putchar(uint32_t x, uint32_t y, char ch, uint32_t color);
void gfx_puttext(uint32_t x, uint32_t y, const char* text, uint32_t color);
void gfx_putcircle(uint32_t cx, uint32_t cy, uint32_t radius, uint32_t color);
void gfx_putrect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
void gfx_putfilledcircle(uint32_t cx, uint32_t cy, uint32_t radius, uint32_t color);
void gfx_putline(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color);
