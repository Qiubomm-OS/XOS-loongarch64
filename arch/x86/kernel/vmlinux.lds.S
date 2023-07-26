ENTRY(start)
SECTIONS
{
	PROVIDE( _start = . );
	. += 0xC0000000;
	.text : AT(ADDR(.text) - 0xC0000000)
	{
		. += 0x100000;
		*(.init.text)
		. = ALIGN(4096);
		*(.init.data)
		. = ALIGN(8192);
		*(.text)
		. = ALIGN(4096);
	}
	.data : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
		*(.rodata)
		. = ALIGN(8192);
	}

	.bss : AT(ADDR(.bss) - 0xC0000000)
	{
		*(.bss)
		. = ALIGN(4096);
	}
	.stab : AT(ADDR(.stab) - 0xC0000000)
	{
		*(.stab)
		. = ALIGN(8192);
	}
	.data.init_task : AT(ADDR(.data.init_task) - 0xC0000000)
    	{
        	*(.data.init_task)
        	. = ALIGN(8192);
    	}
	init_stack = .;

	PROVIDE( __initcall_start = .);
	.initcall.init : AT(ADDR(.initcall.init) - 0xC0000000)
	{
		*(.initcall.init)
		PROVIDE( __initcall_end = .);
		. = ALIGN(8192);
	}
        . += 0x2000;
	.stabstr : AT(ADDR(.stabstr) - 0xC0000000)
	{
		*(.stabstr)
	 	. = ALIGN(4096);
	}
	PROVIDE( _end = . );

	/DISCARD/ : { *(.comment) *(.eh_frame) }
}
