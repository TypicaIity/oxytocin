#include "int/gdt.h"
#include "int/idt.h"
#include "int/isr.h"

#include "mmu/pmm.h"
#include "mmu/kheap.h"

#include "gfx/graphics.h"

#include "drivers/keyboard.h"

BootInfo* gInfo = nullptr;

void kinit(BootInfo* info) {
	gInfo = info;

	gdt_init(), idt_init(), isr_init();
	pmm_init(), kheap_init(0x1000);
	gfx_clear(GFX_BLACK);
}

void kmain(BootInfo* info) {
	kinit(info);

	asm volatile (
		"mov $0, %rax\n"
		"div %al"
	);
}
