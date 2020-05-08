/* Defined in linker script */
extern unsigned _DATA_ROM_START;
extern unsigned _DATA_RAM_START;
extern unsigned _DATA_RAM_END;
extern unsigned _BSS_START;
extern unsigned _BSS_END;


#define SRAM_BASE_ADDR 0x20000000
#define STACK_TOP SRAM_BASE_ADDR + 0x5000
void _start();
extern int main();
extern void button_press_handler(void);

/* Define vector table */
unsigned *stm32_f401re_vectors[] __attribute__ ((section("vectors"))) = {
	(unsigned *)STACK_TOP, /* stack pointer */
	(unsigned *)_start, /* entry point */
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0, /* WWDG_IRQHandler */
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	(unsigned *)button_press_handler
};

void _start()
{
	/* 
	 * Copy data belonging to the '.data' section from its load time
	 * position on flash (ROM) to its run time position in SRAM
	 */
	unsigned *data_rom_start_p = &_DATA_ROM_START;
	unsigned *data_ram_start_p = &_DATA_RAM_START;
	unsigned *data_ram_end_p = &_DATA_RAM_END;
	while (data_ram_start_p < data_ram_end_p) {
		*data_ram_start_p = *data_rom_start_p;
		data_ram_start_p++;
		data_rom_start_p++;
	}

	/* Initialize data in the '.bss' section to zeros */
	unsigned *bss_start_p = &_BSS_START;
	unsigned *bss_end_p = &_BSS_END;
	while (bss_start_p < bss_end_p) {
		*bss_start_p = 0;
		bss_start_p++;
	}

	/* Call the 'main()' function defined in 'main.c' */
	main();
}
