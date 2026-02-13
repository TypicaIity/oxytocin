#pragma once

typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
    uint64_t int_no, err_code;
    uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed)) ISRContext;

typedef void (*isr_t)(ISRContext* ctx);

void isr_init();
void isr_register(uint8_t n, isr_t handler);
