#define FLASH_BASE	0x08000000
#define SRAM_BASE	0x20000000
#define PERIPH_BASE	0x40000000 

#define SRAM_SIZE	96*1024
#define SRAM_END	(PERIPH_BASE + SRAM_SIZE)

#define RCC_BASE	(PERIPH_BASE + 0x23800)
#define RCC_AHB1ENR	((unsigned *)(RCC_BASE + 0x30))

#define GPIOA_BASE	(PERIPH_BASE + 0x20000)
#define GPIOB_BASE	(PERIPH_BASE + 0x20400)
#define GPIOC_BASE	(PERIPH_BASE + 0x20800)
#define GPIOD_BASE	(PERIPH_BASE + 0x20C00)


typedef struct {
	volatile unsigned moder;
	volatile unsigned otyper;
	volatile unsigned ospeedr;
	volatile unsigned pupdr;
	volatile unsigned idr;
	volatile unsigned odr;
	volatile unsigned bsrr;
	volatile unsigned lckr;
	volatile unsigned afrl;
	volatile unsigned afrh;
} gpiox_regs_t;


typedef struct {
	gpiox_regs_t *reg;
	unsigned pin;
} reg_pin_pair_t;


static gpiox_regs_t *GPIOA_Reg_Ptr = (gpiox_regs_t *)(GPIOA_BASE);
static gpiox_regs_t *GPIOB_Reg_Ptr = (gpiox_regs_t *)(GPIOB_BASE);
static gpiox_regs_t *GPIOC_Reg_Ptr = (gpiox_regs_t *)(GPIOC_BASE);
static gpiox_regs_t *GPIOD_Reg_Ptr = (gpiox_regs_t *)(GPIOD_BASE);


static reg_pin_pair_t output_pins[] = {
	{(gpiox_regs_t *)GPIOC_BASE, 8},
	{(gpiox_regs_t *)GPIOC_BASE, 6},
	{(gpiox_regs_t *)GPIOC_BASE, 5},
	{(gpiox_regs_t *)GPIOD_BASE, 8},
	{(gpiox_regs_t *)GPIOA_BASE, 12},
	{(gpiox_regs_t *)GPIOA_BASE, 11},
	{(gpiox_regs_t *)GPIOB_BASE, 12},
	{(gpiox_regs_t *)GPIOB_BASE, 2},
	{(gpiox_regs_t *)GPIOB_BASE, 1}
};
static const unsigned output_pins_sz = sizeof output_pins / sizeof *output_pins;


static void init_output_pins(void)
{
	unsigned i;
	reg_pin_pair_t *e;
	for (i = 0; i < output_pins_sz; i++) {
		e = &output_pins[i];
		/* Set as output pins */
		e->reg->moder &= ~((1 << (e->pin * 2)) + 1);
		e->reg->moder &= ~(1 << (e->pin * 2));
		e->reg->moder |= (1 << (e->pin * 2));
	}
}


static void delay(unsigned count)
{
	while (count--);
}


static void init_gpios(void)
{
	/* Enable clock for GPIOA */
	do {
		*RCC_AHB1ENR = 0x1;
	} while (!(*RCC_AHB1ENR & 0x1));
	/* Enable clock for GPIOB */
	do {
		*RCC_AHB1ENR |= 0x2;
	} while (!(*RCC_AHB1ENR & 0x2));
	/* Enable clock for GPIOC */
	do {
		*RCC_AHB1ENR |= 0x4;
	} while (!(*RCC_AHB1ENR & 0x4));
	/* Enable clock for GPIOD */
	do {
		*RCC_AHB1ENR |= 0x8;
	} while (!(*RCC_AHB1ENR & 0x8));

	/* Configure PC13 (Button) as input no pull-up or pull-down */
	GPIOC_Reg_Ptr->moder &= ~(1 << 26 | 1 << 27);
	GPIOC_Reg_Ptr->pupdr &= ~(1 << 26 | 1 << 27);

	/* Configure PA5 (LD2) as output */
	GPIOA_Reg_Ptr->moder &= ~((1 << 10));
	GPIOA_Reg_Ptr->moder &= ~((1 << 11));
	GPIOA_Reg_Ptr->moder |= ((1 << 10));

	init_output_pins();
}


static void run_gpio_ladder(unsigned reverse, unsigned d)
{
	int i;
	reg_pin_pair_t *e;

	if (reverse) {
		for (i = output_pins_sz - 1; i >= 0; i--) {
			e = &output_pins[i];
			e->reg->odr |= (1 << e->pin);
			delay(d);
			e->reg->odr &= ~(1 << e->pin);
		}
	} else {
		for (i = 0; i < output_pins_sz; i++) {
			e = &output_pins[i];
			e->reg->odr |= (1 << e->pin);
			delay(d);
			e->reg->odr &= ~(1 << e->pin);
		}
	}
}

int main(void)
{
	unsigned reverse = 0;
	unsigned d = 200000;

	init_gpios();

	while (1) {
		/* TODO Replace with button interrupt */
		if (!(GPIOC_Reg_Ptr->idr & (1 << 13))) {
			reverse = !reverse;
			/* To notify direction of ladder has changed */
			GPIOA_Reg_Ptr->odr |= 0x20;
			delay(2000000);
			GPIOA_Reg_Ptr->odr &= ~0x20;
		}
		run_gpio_ladder(reverse, d);
	}

	return 0;
}

