/*

   Created by felix on 9/26/2025.
  

   COPYRIGHT Fentanyl LLC 2025
   v0.0.1-beta

   MIT LICENSE
*/

OUTPUT_FORMAT("elf64-x86-64")
ENTRY(_start)

SECTIONS {
	. = 0x8200;

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