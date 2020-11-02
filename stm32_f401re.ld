MEMORY
{
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
	SRAM (wrx) : ORIGIN = 0x20000000, LENGTH = 96K
}

ENTRY(_start)

SECTIONS
{
	/* Set location pointer */
	. = 0x0;

	/* 
	 * Create final .text section from vector table and .text sections
	 * available from input files
	 */
	.text : ALIGN(4)
	{
		*(vectors)
		*(.text)
		*(.text*)
		KEEP(*(vectors))
	} >FLASH

	/* Used by the startup to initialize data */
        _sidata = LOADADDR(.data);
        .data : ALIGN(4)
        {
                . = ALIGN(4);
                _sdata = .;     /* create global symbol at data start */

                *(.data)
                *(.data*)

                . = ALIGN(4);
                _edata = .;     /* define a global symbol at data end */
        } >SRAM AT>FLASH

        /* Uninitialized data section */
        .bss : ALIGN(4)
	{
                /* This is used by the startup in order to initialize the .bss section */
                _sbss = .;
                *(.bss .bss*)
                *(COMMON)

                . = ALIGN(4);
                _ebss = .;
        } >SRAM AT>SRAM

        /* Constant data goes into flash */
        .rodata : ALIGN(4)
        {
                *(.rodata)      /* .rodata sections (constants) */
                *(.rodata*)     /* .rodata* sections (strings, etc.) */
        } >FLASH
}
