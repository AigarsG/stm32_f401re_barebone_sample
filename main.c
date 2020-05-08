#define FLASH_BASE	0x08000000
#define SRAM_BASE	0x20000000
#define PERIPH_BASE	0x40000000 

#define SRAM_SIZE	96*1024
#define SRAM_END	(PERIPH_BASE + SRAM_SIZE)

#define RCC_BASE	(PERIPH_BASE + 0x23800)
#define RCC_AHB1ENR	((unsigned *)(RCC_BASE + 0x30))
#define RCC_APB2ENR	((unsigned *)(RCC_BASE + 0x44))

#define SYSCFG_BASE	(PERIPH_BASE + 0x13800)
#define SYSCFG_EXTICR4	((unsigned *)(SYSCFG_BASE + 0x14))

#define EXTI_BASE	(PERIPH_BASE + 0x13C00)
#define EXTI_IMR	((unsigned *)(EXTI_BASE + 0x0))
#define EXTI_RTSR	((unsigned *)(EXTI_BASE + 0x8))
#define EXTI_FTSR	((unsigned *)(EXTI_BASE + 0xC))
#define EXTI_PR		((unsigned *)(EXTI_BASE + 0x14))

#define GPIOA_BASE	(PERIPH_BASE + 0x20000)
#define GPIOB_BASE	(PERIPH_BASE + 0x20400)
#define GPIOC_BASE	(PERIPH_BASE + 0x20800)
#define GPIOD_BASE	(PERIPH_BASE + 0x20C00)

#define NVIC_BASE	0xE000E000
#define NVIC_ISER1	((unsigned *)(NVIC_BASE + 0x100 + 0x4))
#define NVIC_ICPR1	((unsigned *)(NVIC_BASE + 0x280 + 0x4))


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


static void init_irq(void)
{
	/* Enable clock for SYSCFGEN in (RCC_APB2ENR) reg */
	do {
		*RCC_APB2ENR |= (1 << 14);
	} while (!(*RCC_APB2ENR & (1 << 14)));
	/* Set PC13 as source for EXTI15_10 */
	*SYSCFG_EXTICR4 = 1 << 5;
	/* Configure EXTI15_10 */
	/* Unmask line 13 of EXTI */
	*EXTI_IMR = 1 << 13;
	/* Select falling edge trigger (button press-down) */
	*EXTI_FTSR = 1 << 13;
	/* Enable EXTI10_15 (40) in NVIC_ISER1 register */
	*NVIC_ISER1 |= (1 << 8);
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


static void run_led_row(unsigned reverse, unsigned nleds, unsigned d)
{
	int i;
	int tail;
	reg_pin_pair_t *e;

	if (reverse) {
		for (i = output_pins_sz - 1; i >= 0; i--) {
			e = &output_pins[i];
			e->reg->odr |= (1 << e->pin);
			if (i + nleds < output_pins_sz) {
				e = &output_pins[i+nleds];
				e->reg->odr &= ~(1 << e->pin);
			}
			delay(d);
		}
		/* Turn off remaining LEDS */
		for (i = nleds - 1; i >= 0; i--) {
			e = &output_pins[i];
			e->reg->odr &= ~(1 << e->pin);
			delay(d);
		}
	} else {
		for (i = 0; i < output_pins_sz; i++) {
			e = &output_pins[i];
			e->reg->odr |= (1 << e->pin);
			if (i >= nleds) {
				e = &output_pins[i-nleds];
				e->reg->odr &= ~(1 << e->pin);
			}
			delay(d);
		}
		/* Turn off remaining LEDS */
		for (i = i - nleds - 1; i < output_pins_sz; i++) {
			e = &output_pins[i];
			e->reg->odr &= ~(1 << e->pin);
			delay(d);
		}
	}
}

static volatile unsigned reverse;
void button_press_handler(void)
{
	/* Clear pending bit for line 13 of EXTI and IRQ40 */
	*EXTI_PR |= (1 << 13);
	*NVIC_ICPR1 |= (1 << 8);
	reverse = ~reverse;
}

int main(void)
{
	unsigned d = 200000;
	unsigned nleds = 3;

	init_irq();
	init_gpios();

	while (1) {
		run_led_row(reverse, nleds, d);
	}

	return 0;
}

