#define FLASH_BASE	0x08000000
#define SRAM_BASE	0x20000000
#define PERIPH_BASE	0x40000000 

#define SRAM_SIZE	96*1024
#define SRAM_END	(PERIPH_BASE + SRAM_SIZE)

#define RCC_BASE	(PERIPH_BASE + 0x23800)
#define RCC_AHB1ENR	((unsigned *)(RCC_BASE + 0x30))

#define GPIOA_BASE	(PERIPH_BASE + 0x20000)
#define GPIOA_MODER	((unsigned *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR	((unsigned *)(GPIOA_BASE + 0x14))


void delay(unsigned count)
{
	while (count--);
}


int main(void)
{
	do {
		*RCC_AHB1ENR = 0x1;
	} while (!(*RCC_AHB1ENR & 0x1));

	*GPIOA_MODER |= 0x400;

	while (1) {
		*GPIOA_ODR = 0x20;
		delay(2000000);
		*GPIOA_ODR = 0x0;
		delay(2000000);
	}

	return 0;
}

