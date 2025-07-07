[BITS 16]
[ORG 0x7C00]

boot:
	cli
	mov ax, 0x0000
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7C00
	sti

	mov ah, 0x41
	mov bx, 0x55AA
	int 0x13
	jc .err
	
	cmp bx, 0xAA55
	jne .err

	mov ah, 0x42
	mov si, packet
	int 0x13
	jc .err
	jmp 0x0000:STAGE2

.err:
	mov si, err
	call puts
	jmp $

puts:
	pusha
	mov ah, 0x0E
.loop:
	lodsb
	test al, al
	jz .done
	int 0x10
	jmp .loop
.done:
	popa
	ret

packet:
	db 16		; sizeof packet
	db 0		; reserved
	dw 6		; size
	dw STAGE2	; buffer offset
	dw 0		; buffer segment
	dq 1		; starting lba

STAGE2 equ 0x1000

err db 'ERRDISK', 13, 10, 0

times 510-($-$$) db 0
dw 0xAA55
