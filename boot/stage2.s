[bits 16]
[org 0x1000]

stage2:
	mov ah, 0x42
	mov dl, 0x80
	mov si, packet
	int 0x13
	jc .err

	cli
	
	pushfd
	pop eax
	mov ecx, eax
	xor eax, 1 << 21
	push eax
	popfd
	pushfd
	pop eax
	push ecx
	popfd
	xor eax, ecx
	jz .err

	mov eax, 0x80000000
	cpuid
	cmp eax, 0x80000001
	jb .err

	mov eax, 0x80000001
	cpuid
	test edx, 1 << 29
	jz .err
	
	call setupPaging
	
	lgdt [gdtr]
	
	mov eax, cr0 
	or al, 1
	mov cr0, eax

	jmp CODESEG:pm
.err:
	mov si, err
.loop:
	lodsb
	or al, al
	jz .done
	mov ah, 0x0E
	int 0x10
	jmp .loop
.done:
	hlt
	jmp .done

[bits 32]
pm:
	mov ax, DATASEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	mov eax, PML4_TABLE
	mov cr3, eax

	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	jmp CODESEG64:lm

[bits 32]
setupPaging:
	mov edi, PML4_TABLE
	mov ecx, 0x1000
	xor eax, eax
	rep stosd

	mov eax, PDP_TABLE
	or eax, 0x03		; present | writable
	mov [PML4_TABLE], eax
	mov eax, PD_TABLE
	or eax, 0x03		; present | writable
	mov [PDP_TABLE], eax
	mov eax, 0x83		; present | writable | page size
	mov [PD_TABLE], eax
	ret

[bits 64]
lm:
	mov ax, DATASEG64
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	jmp KERNEL

gdtStart:
	dq 0					; null
	dq 0x00CF9A000000FFFF	; cs 32
	dq 0x00CF92000000FFFF	; ds 32
	dq 0x00AF9A000000FFFF	; cs 64
	dq 0x00AF92000000FFFF	; ds 64
gdtEnd:

gdtr:
	dw gdtEnd - gdtStart - 1
	dd gdtStart

packet:
	db 16		; sizeof packet
	db 0		; reserved
	dw 32		; size
	dw KERNEL	; buffer offset
	dw 0		; buffer segment
	dq 2		; starting lba

CODESEG		equ 0x08
DATASEG		equ 0x10
CODESEG64	equ 0x18
DATASEG64	equ 0x20

PML4_TABLE	equ 0x2000
PDP_TABLE	equ 0x3000
PD_TABLE	equ 0x4000

KERNEL		equ 0x9000

err db 'BAD_HARDWARE_OR_DISK', 0
