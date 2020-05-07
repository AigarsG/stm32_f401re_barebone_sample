SECTIONS
{
	/* Set location pointer */
	. = 0x0;

	/* 
	 * Create final .text section from vector table and .text sections
	 * available from input files
	 */
	.text : 
	{
		*(vectors)
		*(.text)
	}

	/* Put .rodata section in read-only memory region after .text section */
	.rodata :
	{
		*(.rodata)
	}
	_DATA_ROM_START = .; /* Points to .rodata */

	/* Move location counter to the start of the SRAM and save it */
	. = 0x20000000;
	_DATA_RAM_START = .;
	/* Put .data section in both read-only and read-write memory regions */
	.data : AT(_DATA_ROM_START)
	{
		*(.data)
	}
	_DATA_RAM_END = .;

	/* .bss section */
	_BSS_START = .; /* In RAM */
	.bss : {
		*(.bss)
	}
	_BSS_END = .;

}
