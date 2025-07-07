#pragma once

#include "common.h"

#include "std/types.h"

typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
    uint64_t int_no, err_code;
    uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed)) context_t;

typedef void (*isr_t)(context_t* ctx);

n_func void isr_init();
n_func void isr_register(uint8_t n, isr_t handler);
