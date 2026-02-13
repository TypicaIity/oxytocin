#include "gdt.h"

struct GDTEntry gdt[3];

void gdt_setentry(int idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	gdt[idx].limitLow = limit & 0xFFFF;
	gdt[idx].baseLow = base & 0xFFFF;
	gdt[idx].baseMid = (base >> 16) & 0xFF;
	gdt[idx].access = access;
	gdt[idx].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
	gdt[idx].baseHigh = (base >> 24) & 0xFF;
}

void gdt_init() {
	gdt_setentry(0, 0, 0, 0, 0);
	gdt_setentry(1, 0, 0xFFFFF, 0x9A, 0xA0);
	gdt_setentry(2, 0, 0xFFFFF, 0x92, 0xA0);

	struct {
		uint16_t limit;
		uint64_t base;
	} __attribute__((packed)) gdtr;
	gdtr.limit = sizeof(gdt) - 1;
	gdtr.base  = (uint64_t)&gdt;
	asm volatile("lgdt %0" : : "m"(gdtr));

	uint16_t ds = 0x10;
	asm volatile(
		"movw %0, %%ds\n"
		"movw %0, %%es\n"
		"movw %0, %%ss\n"
		"movw %0, %%fs\n"
		"movw %0, %%gs\n"
		:: "r"(ds) :
	);
	asm volatile(
		"pushq $8\n"
		"leaq 1f(%rip), %rax\n"
		"pushq %rax\n"
		"lretq\n"
		"1:\n"
	);
}
