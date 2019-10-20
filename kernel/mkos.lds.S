ENTRY(__entry)

INCLUDE linker.lds.S

SECTIONS
{
	/*kernel will be compiled with virtual address base at 2GB*/
	/*base_start_address = (0x80010000)*/
	. = base_address + start_address;

	PROVIDE(_kernel_start = .);

 .text : AT(start_address)
	{
		*(.text)
	}

	.data : 
	{ 
		*(.data) 
	}

	.bss : 
	{ 
		*(.bss COMMON)
	}
	. = ALIGN(8);
	. = . + 0x1000;
	_irq_stack = .;
	. = . + 0x100000;
	_abt_stack = .;
	. = . + 0x100000;
	_svc_stack = .;

	PROVIDE(_kernel_end = .);
}