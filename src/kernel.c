#include "vga/vga.h"

#include "interrupts/isr.h"
#include "interrupts/idt.h"

void kinit() {
	vga_cls();
	
	idt_init();
}

void kmain() {
	kinit();
	
	asm volatile("mov $0, %eax\n div %al");
}