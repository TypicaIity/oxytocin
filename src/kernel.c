#include "std/mem.h"
#include "vga/vga.h"
#include "interrupts/isr.h"
#include "interrupts/idt.h"

void kmain() {
	vga_cls();
	idt_init();

	//
}
