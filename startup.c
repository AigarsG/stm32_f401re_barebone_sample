/* Defined in linker script */
/* Begin address for the initialisation values of the .data section */
extern unsigned _sidata;
/* Begin address for the .data section */
extern unsigned _sdata;
/* End address for the .data section */
extern unsigned _edata;
/* Start address for the .bss section */
extern unsigned _sbss;
/* End address for the .bss section */
extern unsigned _ebss;


#define STACK_TOP 0x20018000
void _start();
extern int main();
extern void button_press_handler(void);

/* Define vector table */
void (*stm32_f401re_vectors[])() __attribute__ ((section("vectors"))) = {
	(void (*)(void))STACK_TOP, /* stack pointer */
	(void (*)(void))_start, /* entry point */
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
	(void (*)(void))button_press_handler
};


static void __initialize_data(unsigned *flash_begin, unsigned *data_begin,
			      unsigned *data_end)
{
	unsigned *p = data_begin;
	while (p < data_end) {
		*p = *flash_begin;
		p++;
		flash_begin++;
	}
}

static void __initialize_bss(unsigned *bss_begin, unsigned *bss_end)
{
	unsigned *p = bss_begin;
	while (p < bss_end) {
		*p = 0;
		p++;
	}
}

void _start()
{
	/* 
	 * Copy data belonging to the '.data' section from its load time
	 * position on flash (ROM) to its run time position in SRAM
	 */
	__initialize_data(&_sidata, &_sdata, &_edata);

	/* Initialize data in the '.bss' section to zeros */
	__initialize_bss(&_sbss, &_ebss);

	/* Call the 'main()' function */
	main();
}
