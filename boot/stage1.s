[bits 16]
[org 0x7C00]

boot:
	mov bp, 0x0500
	mov sp, bp

	mov bx, 0x0002

	mov byte [drive], dl

	mov ah, 0x42
	mov si, packet
	mov dl, [drive]
	int 0x13
	jc .err

	jmp 0x0000:STAGE2

.err:
	mov si, err
	mov ah, 0x0E
.loop:
	lodsb
	test al, al
	jz .done
	int 0x10
	jmp .loop
.done:
	jmp $

packet:
	db 16		; sizeof packet
	db 0		; reserved
	dw 6		; size
	dw STAGE2	; buffer offset
	dw 0		; buffer segment
	dq 1		; starting lba

STAGE2 equ 0x7E00

err db 'ERRDISK', 0
drive: db 0x00

times 510-($-$$) db 0
dw 0xAA55
