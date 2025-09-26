//
// Created by felix on 9/26/2025.
//

// COPYRIGHT Fentanyl LLC 2025
// v0.0.1-beta

// MIT LICENSE

#include "std/mem.h"
#include "vga/vga.h"
#include "interrupts/isr.h"
#include "interrupts/idt.h"

#define PAGE_PRESENT 1
#define PAGE_READWRITE 2
#define PAGE_ACCESSED 32

// so kernel.bin has credits
const char __credits[] __attribute__((section(".credits"))) = "//\n// Created by felix on 9/26/2025.\n//\n\n// COPYRIGHT Fentanyl LLC 2025\n// v0.0.1-beta\n\n// MIT LICENSE";

void kmain() {
	vga_cls();
	idt_init();
	vmm_init();

	puts("puts");
}
