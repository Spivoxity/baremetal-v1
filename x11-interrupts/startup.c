/* ubit-v1/startup.c */
/* Copyright (c) 2018 J. M. Spivey */

#include "hardware.h"

/* init -- main program, creates application processes */
void init(void);

void default_start(void)
{
    init();                    /* Call the main program. */
    while (1) pause();         /* Halt if init() returns */
}

void __start(void) __attribute((weak, alias("default_start")));

/* The next four routines can be used in C compiler output, even if
not mentioned in the source. */

/* memcpy -- copy n bytes from src to dest (non-overlapping) */
void *memcpy(void *dest, const void *src, unsigned n)
{
    unsigned char *p = dest;
    const unsigned char *q = src;
    while (n-- > 0) *p++ = *q++;
    return dest;
}

/* memmove -- copy n bytes from src to dest, allowing overlaps */
void *memmove(void *dest, const void *src, unsigned n)
{
    unsigned char *p = dest;
    const unsigned char *q = src;
    if (dest <= src)
        while (n-- > 0) *p++ = *q++;
    else {
        p += n; q += n;
        while (n-- > 0) *--p = *--q;
    }
    return dest;
}
    
/* memset -- set n bytes of dest to byte x */
void *memset(void *dest, unsigned x, unsigned n)
{
    unsigned char *p = dest;
    while (n-- > 0) *p++ = x;
    return dest;
}

/* memcmp -- compare n bytes */
int memcmp(const void *pp, const void *qq, int n)
{
    const unsigned char *p = pp, *q = qq;
    while (n-- > 0) {
        if (*p++ != *q++)
            return (p[-1] < q[-1] ? -1 : 1);
    }
    return 0;
}

/* Addresses set by the linker */
extern unsigned char __data_start[], __data_end[],
    __bss_start[], __bss_end[], __etext[], __stack[];

/* __reset -- the system starts here */
void __reset(void)
{
    /* Activate the crystal clock */
    CLOCK.HFCLKSTARTED = 0;
    CLOCK.HFCLKSTART = 1;
    while (! CLOCK.HFCLKSTARTED) { }

    int data_size = __data_end - __data_start;
    int bss_size = __bss_end - __bss_start;
    memcpy(__data_start, __etext, data_size);
    memset(__bss_start, 0, bss_size);

    __start();
}


/* NVIC SETUP FUNCTIONS */

/* On Cortex-M0, only the top two bits of each interrupt priority are
implemented, but for portability priorities should be specified with
integers in the range [0..255].  On Cortex-M4, the top three bits are
implemented.*/

/* irq_priority -- set priority for an IRQ to a value [0..255] */
void irq_priority(int irq, unsigned prio)
{
    if (irq < 0)
        SET_BYTE(SCB.SHPR[(irq+12) >> 2], irq & 0x3, prio);
    else
        SET_BYTE(NVIC.IPR[irq >> 2], irq & 0x3, prio);
}
     
/* enable_irq -- enable interrupts from an IRQ */
void enable_irq(int irq)
{
    NVIC.ISER[0] = BIT(irq);
}

/* disable_irq -- disable interrupts from a specific IRQ */
void disable_irq(int irq)
{
    NVIC.ICER[0] = BIT(irq);
}

/* clear_pending -- clear pending interrupt from an IRQ */
void clear_pending(int irq)
{
    NVIC.ICPR[0] = BIT(irq);
}


/* DEVICE TABLES */

volatile struct _timer * const TIMER[] = {
    &TIMER0, &TIMER1, &TIMER2
};

volatile struct _i2c * const I2C[] = {
    &I2C0, &I2C1
};

volatile struct _spi * const SPI[] = {
    &SPI0, &SPI1
};


/* GPIO CONVENIENCE */

/* gpio_dir -- set GPIO direction */
void gpio_dir(unsigned pin, unsigned dir) {
    if (dir)
        GPIO.DIRSET = BIT(pin);
    else
        GPIO.DIRCLR = BIT(pin);
}

/* gpio_connect -- connect pin for input */
void gpio_connect(unsigned pin) {
    SET_FIELD(GPIO.PINCNF[pin], GPIO_PINCNF_INPUT, GPIO_INPUT_Connect);
}

/* gpio_drive -- set GPIO drive strength */
void gpio_drive(unsigned pin, unsigned mode) {
    SET_FIELD(GPIO.PINCNF[pin], GPIO_PINCNF_DRIVE, mode);
}

/* gpio_out -- set GPIO output value */
void gpio_out(unsigned pin, unsigned value) {
    if (value)
        GPIO.OUTSET = BIT(pin);
    else
        GPIO.OUTCLR = BIT(pin);
}

/* gpio_in -- get GPIO input bit */
unsigned gpio_in(unsigned pin) {
    return GET_BIT(GPIO.IN, pin);
}


/*  INTERRUPT VECTORS */

/* We use the linker script to define each handler name as an alias
for default_handler if it is not defined elsewhere.  Applications can
subsitute their own definitions for individual handler names like
uart_handler(). */

/* delay_loop -- timed delay */
void delay_loop(unsigned usecs)
{
    unsigned t = usecs << 1;
    while (t > 0) {
        /* 500nsec per iteration at 16MHz */
        nop(); nop(); nop();
        t--;
    }
}

/* spin -- show Seven Stars of Death */
void spin(void)
{
    intr_disable();

    GPIO.DIR = 0xfff0;
    while (1) {
        GPIO.OUT = 0x4000;
        delay_loop(500000);
        GPIO.OUT = 0;
        delay_loop(100000);
    }          
}

void default_handler(void) __attribute((weak, alias("spin")));

/* The linker script makes all these handlers into weak aliases for */
/* default_handler. */

void nmi_handler(void);
void hardfault_handler(void);
void svc_handler(void);
void pendsv_handler(void);
void systick_handler(void);
void uart_handler(void);
void timer0_handler(void);
void timer1_handler(void);
void timer2_handler(void);
void power_clock_handler(void);
void radio_handler(void);
void i2c0_spi0_handler(void);
void i2c1_spi1_handler(void);
void gpiote_handler(void);
void adc_handler(void);
void rtc0_handler(void);
void temp_handler(void);
void rng_handler(void);
void ecb_handler(void);
void ccm_aar_handler(void);
void wdt_handler(void);
void rtc1_handler(void);
void qdec_handler(void);
void lpcomp_handler(void);
void swi0_handler(void);
void swi1_handler(void);
void swi2_handler(void);
void swi3_handler(void);
void swi4_handler(void);
void swi5_handler(void);

/* This vector table is placed at address 0 in the flash by directives
in the linker script. */

void *__vectors[] __attribute((section(".vectors"))) = {
    __stack,                    /* -16 */
    __reset,
    nmi_handler,
    hardfault_handler,
    0,                          /* -12 */
    0,
    0,
    0,
    0,                          /*  -8 */
    0,
    0,
    svc_handler,
    0,                          /* -4 */
    0,
    pendsv_handler,
    systick_handler,
    
    /* external interrupts */
    power_clock_handler,        /*  0 */
    radio_handler,
    uart_handler,
    i2c0_spi0_handler,
    i2c1_spi1_handler,          /*  4 */
    0,
    gpiote_handler,
    adc_handler,
    timer0_handler,             /*  8 */
    timer1_handler,
    timer2_handler,
    rtc0_handler,
    temp_handler,               /* 12 */
    rng_handler,
    ecb_handler,
    ccm_aar_handler,
    wdt_handler,                /* 16 */
    rtc1_handler,
    qdec_handler,
    lpcomp_handler,
    swi0_handler,               /* 20 */
    swi1_handler,
    swi2_handler,
    swi3_handler,
    swi4_handler,               /* 24 */
    swi5_handler,
    0,
    0,
    0,                          /* 28 */
    0,
    0,
    0
};
