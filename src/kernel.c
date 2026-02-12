#include "gfx/graphics.h"
#include "drivers/keyboard.h"

BootInfo* gInfo = nullptr;

void kinit(BootInfo* info) {
	gInfo = info;

	gfx_clear(GFX_BLACK);
}

void kmain(BootInfo* info) {
	kinit(info);

	uint32_t colors[] = {
		GFX_BLACK, GFX_BROWN, GFX_RED, GFX_ORANGE, GFX_YELLOW,
		GFX_GREEN, GFX_CYAN, GFX_BLUE, GFX_PURPLE, GFX_MAGENTA,
		GFX_PINK, GFX_GRAY, GFX_LGRAY, GFX_WHITE
	};

	for (int i = 0, y = 128; i < 14; i++, y += 32)
		gfx_puttext(512, y, "Hello, World!", colors[i]);

	uint32_t cx = 0, cy = 0;
	while (true) {
		int ch = getchar();
		if (ch == '\n') {
			cy += 32;
			cx = 0;
		} else if (ch == '\b') {
			if (cx >= 16) {
				cx -= 16;
				gfx_putrect(cx, cy, 16, 16, GFX_BLACK);
			}
		} else {
			gfx_putchar(cx, cy, ch, GFX_LGRAY);
			cx += 16;
		}
	}
}
