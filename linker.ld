OUTPUT_FORMAT("elf64-x86-64")
ENTRY(kmain)

SECTIONS {
	. = 0x100000;

	__ld_kernel_begin = .;
	
	.text : {
		*(.text)
	}
	
	.rodata : {
		*(.rodata)
	}
	
	.data : {
		*(.data)
	}
	
	.bss : {
		*(.bss)
	}

	__ld_kernel_end = .;

	/DISCARD/ : {
		*(.comment)
	}
}