//
// Created by felix on 9/26/2025.
//

// COPYRIGHT Fentanyl LLC 2025
// v0.0.1-beta

// MIT LICENSE

#include "idt.h"
#include "isr.h"
#include "std/mem.h"

static idtr_t oxy_nobss desc;
static idt_entry_t oxy_nobss idt[IDT_SIZE];

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void idt_init() {
	for (int i = 0; i < IDT_SIZE; i++) {
		idt[i].low = 0;
		idt[i].sel = 0;
		idt[i].ist = 0;
		idt[i].attr = 0;
		idt[i].mid = 0;
		idt[i].high = 0;
		idt[i]._reserved = 0;
	}

	desc.limit = (sizeof(idt_entry_t) * IDT_SIZE) - 1;
	desc.base = (uintptr_t)&idt;
	
	idt_setgate(0, (uintptr_t)isr0, IDT_INTERRUPT_GATE);
	idt_setgate(1, (uintptr_t)isr1, IDT_INTERRUPT_GATE);
	idt_setgate(2, (uintptr_t)isr2, IDT_INTERRUPT_GATE);
	idt_setgate(3, (uintptr_t)isr3, IDT_INTERRUPT_GATE);
	idt_setgate(4, (uintptr_t)isr4, IDT_INTERRUPT_GATE);
	idt_setgate(5, (uintptr_t)isr5, IDT_INTERRUPT_GATE);
	idt_setgate(6, (uintptr_t)isr6, IDT_INTERRUPT_GATE);
	idt_setgate(7, (uintptr_t)isr7, IDT_INTERRUPT_GATE);
	idt_setgate(8, (uintptr_t)isr8, IDT_INTERRUPT_GATE);
	idt_setgate(9, (uintptr_t)isr9, IDT_INTERRUPT_GATE);
	idt_setgate(10, (uintptr_t)isr10, IDT_INTERRUPT_GATE);
	idt_setgate(11, (uintptr_t)isr11, IDT_INTERRUPT_GATE);
	idt_setgate(12, (uintptr_t)isr12, IDT_INTERRUPT_GATE);
	idt_setgate(13, (uintptr_t)isr13, IDT_INTERRUPT_GATE);
	idt_setgate(14, (uintptr_t)isr14, IDT_INTERRUPT_GATE);
	idt_setgate(15, (uintptr_t)isr15, IDT_INTERRUPT_GATE);
	idt_setgate(16, (uintptr_t)isr16, IDT_INTERRUPT_GATE);
	idt_setgate(17, (uintptr_t)isr17, IDT_INTERRUPT_GATE);
	idt_setgate(18, (uintptr_t)isr18, IDT_INTERRUPT_GATE);
	idt_setgate(19, (uintptr_t)isr19, IDT_INTERRUPT_GATE);
	idt_setgate(20, (uintptr_t)isr20, IDT_INTERRUPT_GATE);
	idt_setgate(21, (uintptr_t)isr21, IDT_INTERRUPT_GATE);
	idt_setgate(22, (uintptr_t)isr22, IDT_INTERRUPT_GATE);
	idt_setgate(23, (uintptr_t)isr23, IDT_INTERRUPT_GATE);
	idt_setgate(24, (uintptr_t)isr24, IDT_INTERRUPT_GATE);
	idt_setgate(25, (uintptr_t)isr25, IDT_INTERRUPT_GATE);
	idt_setgate(26, (uintptr_t)isr26, IDT_INTERRUPT_GATE);
	idt_setgate(27, (uintptr_t)isr27, IDT_INTERRUPT_GATE);
	idt_setgate(28, (uintptr_t)isr28, IDT_INTERRUPT_GATE);
	idt_setgate(29, (uintptr_t)isr29, IDT_INTERRUPT_GATE);
	idt_setgate(30, (uintptr_t)isr30, IDT_INTERRUPT_GATE);
	idt_setgate(31, (uintptr_t)isr31, IDT_INTERRUPT_GATE);
	
	idt_setgate(32, (uintptr_t)irq0, IDT_INTERRUPT_GATE);
	idt_setgate(33, (uintptr_t)irq1, IDT_INTERRUPT_GATE);
	idt_setgate(34, (uintptr_t)irq2, IDT_INTERRUPT_GATE);
	idt_setgate(35, (uintptr_t)irq3, IDT_INTERRUPT_GATE);
	idt_setgate(36, (uintptr_t)irq4, IDT_INTERRUPT_GATE);
	idt_setgate(37, (uintptr_t)irq5, IDT_INTERRUPT_GATE);
	idt_setgate(38, (uintptr_t)irq6, IDT_INTERRUPT_GATE);
	idt_setgate(39, (uintptr_t)irq7, IDT_INTERRUPT_GATE);
	idt_setgate(40, (uintptr_t)irq8, IDT_INTERRUPT_GATE);
	idt_setgate(41, (uintptr_t)irq9, IDT_INTERRUPT_GATE);
	idt_setgate(42, (uintptr_t)irq10, IDT_INTERRUPT_GATE);
	idt_setgate(43, (uintptr_t)irq11, IDT_INTERRUPT_GATE);
	idt_setgate(44, (uintptr_t)irq12, IDT_INTERRUPT_GATE);
	idt_setgate(45, (uintptr_t)irq13, IDT_INTERRUPT_GATE);
	idt_setgate(46, (uintptr_t)irq14, IDT_INTERRUPT_GATE);
	idt_setgate(47, (uintptr_t)irq15, IDT_INTERRUPT_GATE);

	asm volatile (
		"lidt (%0)"
		:: "r"((uintptr_t)&desc)
	);
}

void idt_setgate(uint8_t num, uintptr_t handler, uint8_t flags) {
	idt[num].low = handler & 0xFFFF;
	idt[num].mid = (handler >> 16) & 0xFFFF;
	idt[num].high = (handler >> 32) & 0xFFFFFFFF;
	idt[num].sel = KERNEL_CS;
	idt[num].ist = 0;
	idt[num].attr = flags;
	idt[num]._reserved = 0;
}
