#include "isr.h"

#include <string.h>
#include "gfx/graphics.h"
#include "drivers/serial.h"

extern BootInfo* gInfo;

static isr_t handlers[256];

static const char* exceptionMessages[32] = {
	"EXCEPTION_DIVIDE_BY_ZERO",
	"EXCEPTION_DEBUG",
	"EXCEPTION_NON_MASKABLE_INTERRUPT",
	"EXCEPTION_BREAKPOINT",
	"EXCEPTION_OVERFLOW",
	"EXCEPTION_BOUND_RANGE_EXCEEDED",
	"EXCEPTION_INVALID_OPCODE",
	"EXCEPTION_NO_COPROCESSOR",
	"EXCEPTION_DOUBLE_FAULT",
	"EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN",
	"EXCEPTION_INVALID_TSS",
	"EXCEPTION_SEGMENT_NOT_PRESENT",
	"EXCEPTION_STACK_FAULT",
	"EXCEPTION_GENERAL_PROTECTION_FAULT",
	"EXCEPTION_PAGE_FAULT",
	"EXCEPTION_UNKNOWN_INTERRUPT",
	"EXCEPTION_COPROCESSOR_ERROR",
	"EXCEPTION_ALIGNMENT_CHECK",
	"EXCEPTION_MACHINE_CHECK",
	"EXCEPTION_RESERVED_19",
	"EXCEPTION_RESERVED_20",
	"EXCEPTION_RESERVED_21",
	"EXCEPTION_RESERVED_22",
	"EXCEPTION_RESERVED_23",
	"EXCEPTION_RESERVED_24",
	"EXCEPTION_RESERVED_25",
	"EXCEPTION_RESERVED_26",
	"EXCEPTION_RESERVED_27",
	"EXCEPTION_RESERVED_28",
	"EXCEPTION_RESERVED_29",
	"EXCEPTION_RESERVED_30",
	"EXCEPTION_RESERVED_31"
};

void isr_init(void) {
	memset(handlers, 0, sizeof(handlers));
	
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x00);
	outb(0xA1, 0x00);
}

void isr_register(uint8_t n, isr_t f) {
	handlers[n] = f;
}

static const char hexDigits[] = "0123456789ABCDEF";

void isr_handler(context_t* ctx) {
	isr_t f = handlers[ctx->int_no];
	if (f)
		f(ctx);
	else {
		gfx_clear(GFX_BLUE);

		uint32_t x = 0, y = 200;

		gfx_puttext(x, y, "                                     ;C", GFX_WHITE);
		y += 64;
		
		gfx_puttext(x, y,
			"     Your PC ran into a problem and needs to restart.\n"
			"     We're just collecting some error info, and then we'll restart for you.\n"
			"     If you'd like to know more, you can search online later for this error:", 
			GFX_WHITE
		);
		y += 384;
		
		x += 80;
		const char* msg = exceptionMessages[ctx->int_no];
		gfx_puttext(x, y, msg, GFX_WHITE);
		x += (strlen(msg) + 1) * 16;

		int idx = 0;
		char hexStr[16];
		hexStr[idx++] = '(',hexStr[idx++] = '0', hexStr[idx++] = 'x';
		for (int i = 28; i >= 0; i -= 4) hexStr[idx++] = hexDigits[(ctx->int_no >> i) & 0xF];
		hexStr[idx++] = ')', hexStr[idx] = '\0';
		gfx_puttext(x, y, hexStr, GFX_WHITE);

		asm volatile("hlt");
	}
}

void irq_handler(context_t* ctx) {
	if (ctx->int_no >= 40)
		outb(0xA0, 0x20);
	outb(0x20, 0x20);
	
	isr_t f = handlers[ctx->int_no];
	if (f) f(ctx);
}
