#pragma once

static inline void outb(uint16_t port, uint8_t val) {
	asm volatile("outb %0, %1" :: "a"(val), "Nd"(port) : "memory");
}

static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	asm volatile("in %%dx, %%al" : "=a"(ret) : "d"(port) : "memory");
	return ret;
}

void serial_init();
void serial_write(const char* s);
