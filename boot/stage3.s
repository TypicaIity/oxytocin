[bits 64]
global _start
extern kmain

section .text
_start:
	mov rsp, stackTop
	call kmain
.hang:
	cli
	hlt
	jmp .hang

section .bss
align 16
stackBottom:
	resb 16384
stackTop:
