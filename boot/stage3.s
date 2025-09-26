;
; Created by felix on 9/26/2025.
;

; COPYRIGHT Fentanyl LLC 2025
; v0.0.1-beta

; MIT LICENSE

[bits 64]
[extern kmain]

section .text
global _start
_start:
    call kmain
    jmp $
