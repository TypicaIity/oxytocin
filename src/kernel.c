#include "int/gdt.h"
#include "int/idt.h"
#include "int/isr.h"

#include "mmu/pmm.h"
#include "mmu/kheap.h"

#include "gfx/graphics.h"

#include "drivers/timer.h"
#include "drivers/keyboard.h"

#include "proc/process.h"

BootInfo* gInfo = nullptr;

void kinit(BootInfo* info) {
	gInfo = info;

	gfx_clear(GFX_BLACK);
	gdt_init(), idt_init(), isr_init();
	timer_init(100);
	pmm_init(), kheap_init(0x1000);
	process_init();
}

void testproc() {
	gfx_puttext(0, 0, "123", GFX_LGRAY);
	process_exit(0);
}

void kmain(BootInfo* info) {
	kinit(info);

	process_create("testproc", testproc);
	//gfx_puttext(0, 0, "456", GFX_LGRAY);
}
