/* common/hardware.h */
/* Copyright (c) 2018-20 J. M. Spivey */

#define UBIT 1
#define UBIT_V1 1

/* Hardware register definitions for nRF51822 */

#define BIT(i) (1 << (i))
#define SET_BIT(r, n) r |= BIT(n)
#define GET_BIT(r, n) (((r) >> (n)) & 0x1)
#define CLR_BIT(r, n) r &= ~BIT(n)
#define GET_BYTE(r, n) (((r) >> (8*(n))) & 0xff)
#define SET_BYTE(r, n, v) \
    r = (r & ~(0xff << 8*n)) | ((v & 0xff) << 8*n)

/* The macros SET_FIELD, etc., are defined in an indirect way that
permits (because of the timing of CPP macro expansion) the 'field'
argument to be a macro that expands the a 'position, width' pair. */

#define SET_FIELD(r, field, val) _SET_FIELD(r, field, val)
#define _SET_FIELD(r, pos, wd, val) \
    r = (r & ~_MASK(pos, wd)) | _FIELD(pos, wd, val)

#define GET_FIELD(r, field) _GET_FIELD(r, field)
#define _GET_FIELD(r, pos, wd)  ((r >> pos) & _MASK0(wd))

#define FIELD(field, val) _FIELD(field, val)
#define _FIELD(pos, wd, val)  (((val) & _MASK0(wd)) << pos)

#define MASK(field) _MASK(field)
#define _MASK(pos, wd)  (_MASK0(wd) << pos)

#define _MASK0(wd)  (~((-2) << (wd-1)))

#define __BIT(pos) pos
#define __FIELD(pos, wd) pos, wd


/* Device pins */
#define PAD19 0
#define PAD2 1
#define PAD1 2
#define PAD0 3
/* LED columns are GPIO 4-12 */
#define PAD3 4
#define PAD4 5
#define PAD10 6
#define PAD9 10
#define PAD7 11
#define PAD6 12
#define ROW1 13
#define ROW2 14
#define ROW3 15
#define PAD16 16
#define PAD5 17
#define PAD8 18
#define PAD12 20
#define PAD15 21
#define PAD14 22
#define PAD13 23
#define PAD11 26
#define PAD20 30

#define USB_TX 24
#define USB_RX 25

#define BUTTON_A PAD5
#define BUTTON_B PAD11

#define I2C0_SCL PAD19
#define I2C0_SDA PAD20

#define SPI_SCK PAD13
#define SPI_MISO PAD14
#define SPI_MOSI PAD15

/* One shared I2C bus (I2C_SCL, I2C_SDA); use SPI1 for SPI */
#define N_I2C 1
#define I2C_INTERNAL 0
#define I2C_EXTERNAL 0
#define SPI_CHAN 1

/* Interrupts */
#define SVC_IRQ    -5
#define PENDSV_IRQ -2
#define RADIO_IRQ   1
#define UART_IRQ    2
#define I2C0_IRQ    3
#define SPI0_IRQ    3
#define I2C1_IRQ    4
#define SPI1_IRQ    4
#define GPIOTE_IRQ  6
#define ADC_IRQ     7
#define TIMER0_IRQ  8
#define TIMER1_IRQ  9
#define TIMER2_IRQ 10
#define RTC0_IRQ   11
#define TEMP_IRQ   12
#define RNG_IRQ    13
#define RTC1_IRQ   17

#define N_INTERRUPTS 32

/* Interrupts 3 and 4 are shared between I2C and SPI: we can
define a handler with either name */
#define i2c0_handler i2c0_spi0_handler
#define spi0_handler i2c0_spi0_handler
#define i2c1_handler i2c1_spi1_handler
#define spi1_handler i2c1_spi1_handler


/* Device registers */
#define _PADDING(n) unsigned char _PAD(__LINE__)[n];
#define _PAD(lnum) _JOIN(_pad, lnum)
#define _JOIN(x, y) x##y


/* System contol block */
struct _scb {
    unsigned CPUID;                     // 0x00
    unsigned ICSR;                      // 0x04
#define   SCB_ICSR_PENDSVSET __BIT(28)
#define   SCB_ICSR_VECTACTIVE __FIELD(0, 8)
    _PADDING(8)
    unsigned SCR;                       // 0x10
#define   SCB_SCR_SLEEPONEXIT __BIT(1)
#define   SCB_SCR_SLEEPDEEP __BIT(2)
#define   SCB_SCR_SEVONPEND __BIT(4)
    _PADDING(4)
    unsigned SHPR[3];                   // 0x18
};

extern volatile struct _scb SCB;


/* Nested vectored interupt controller */
struct _nvic {
    _PADDING(256)
    unsigned ISER[8];                   // 0x100
    _PADDING(96)
    unsigned ICER[8];                   // 0x180
    _PADDING(96)
    unsigned ISPR[8];                   // 0x200
    _PADDING(96)
    unsigned ICPR[8];                   // 0x280
    _PADDING(352)
    unsigned IPR[60];                   // 0x400
};

extern volatile struct _nvic NVIC;


/* Clock control */
struct _clock {
    unsigned HFCLKSTART;                // 0x000
    _PADDING(4)
    unsigned LFCLKSTART;                // 0x008
    _PADDING(244)
    unsigned HFCLKSTARTED;              // 0x100
    unsigned LFCLKSTARTED;              // 0x104
    _PADDING(1040)
    unsigned LFCLKSRC;                  // 0x518
#define   CLOCK_LFCLKSRC_RC 0
    _PADDING(52)
    unsigned XTALFREQ;                  // 0x550
#define   CLOCK_XTALFREQ_16MHz 0xFF
};

extern volatile struct _clock CLOCK;


/* Memory protection unit */
struct _mpu {
    _PADDING(1544)
    unsigned DISABLEINDEBUG;            // 0x608
};

extern volatile struct _mpu MPU;


/* Factory information */
struct _ficr {
    _PADDING(96)
    unsigned DEVICEID[2];               // 0x060
    _PADDING(60)
    unsigned DEVICEADDR[2];             // 0x0a4
    unsigned OVERRIDEEN;                // 0x0a0
#define   FICR_OVERRIDEEN_NRF __BIT(0)
    _PADDING(12)
    unsigned NRF_1MBIT[5];              // 0x0b0
};

extern volatile struct _ficr FICR;


/* Power management */
struct _power {
/* Tasks */
    _PADDING(120)
    unsigned CONSTLAT;                  // 0x078
    unsigned LOWPWR;                    // 0x07c
/* Events */
    _PADDING(136)
    unsigned POFWARN;                   // 0x108
/* Registers */
    _PADDING(504)
    unsigned INTENSET;                  // 0x304
    unsigned INTENCLR;                  // 0x308
    _PADDING(244)
    unsigned RESETREAS;                 // 0x400
#define   POWER_RESETREAS_RESETPIN __BIT(0)
#define   POWER_RESETREAS_DOG __BIT(1)
#define   POWER_RESETREAS_SREQ __BIT(2)
#define   POWER_RESETREAS_LOCKUP __BIT(3)
#define   POWER_RESETREAS_OFF __BIT(16)
#define   POWER_RESETREAS_LPCOMP __BIT(17)
#define   POWER_RESETREAS_DIF _BIT(18)
#define   POWER_RESETREAS_ALL 0x0007000f
    _PADDING(36)
    unsigned RAMSTATUS;                 // 0x428
    _PADDING(212)
    unsigned SYSTEMOFF;                 // 0x500
    _PADDING(12)
    unsigned POFCON;                    // 0x510
#define   POWER_POFCON_POF __BIT(1)
#define   POWER_POFCON_TRESHOLD _FIELD(1, 2)
#define     POWER_THRESHOLD_V21 0
#define     POWER_THRESHOLD_V23 1
#define     POWER_THRESHOLD_V25 2
#define     POWER_THRESHOLD_V27 3
    _PADDING(8)
    unsigned GPREGRET;                  // 0x51c
    _PADDING(4)
    unsigned RAMON;                     // 0x524
    _PADDING(28)
    unsigned RESET;                     // 0x544
    _PADDING(12)
    unsigned RAMONB;                    // 0x554
    _PADDING(32)
    unsigned DCDCEN;                    // 0x578
};

/* Interrupts */
#define POWER_INT_POFWARN 2

extern volatile struct _power POWER;


/* Watchdog timer */
struct _wdt {
/* Tasks */
    unsigned START;                     // 0x000
/* Events */
    _PADDING(252)
    unsigned TIMEOUT;                   // 0x100
/* Registers */
    _PADDING(512)
    unsigned INTENSET;                  // 0x304
    unsigned INTENCLR;                  // 0x308
    _PADDING(244)
    unsigned RUNSTATUS;                 // 0x400
    unsigned REQSTATUS;                 // 0x404
    _PADDING(252)
    unsigned CRV;                       // 0x504
#define   WDT_HERTZ 32768
    unsigned RREN;                      // 0x508
    unsigned CONFIG;                    // 0x50c
#define   WDT_CONFIG_SLEEP __BIT(0)
#define   WDT_CONFIG_HALT __BIT(3)
    _PADDING(240)
    unsigned RR[8];                     // 0x600
#define   WDT_MAGIC 0x6e524635
};

/* Interrupts */
#define WDT_INT_TIMEOUT 0

extern volatile struct _wdt WDT;


/* Non-Volatile Memory Controller */
struct _nvmc {
    _PADDING(1024)
    unsigned READY;                     // 0x400
    _PADDING(256)
    unsigned CONFIG;                    // 0x504
#define   NVMC_CONFIG_WEN __BIT(0)
#define   NVMC_CONFIG_EEN __BIT(1)
    void *ERASEPAGE;                    // 0x508
};

extern volatile struct _nvmc NVMC;


/* GPIO */
struct _gpio {
/* Registers */
    _PADDING(4)
    unsigned OUT;                       // 0x004
    unsigned OUTSET;                    // 0x008
    unsigned OUTCLR;                    // 0x00c
    unsigned IN;                        // 0x010
    unsigned DIR;                       // 0x014
    unsigned DIRSET;                    // 0x018
    unsigned DIRCLR;                    // 0x01c
    _PADDING(480)
    unsigned PINCNF[32];                // 0x200
#define   GPIO_PINCNF_DIR __FIELD(0, 1)
#define     GPIO_DIR_Input 0
#define     GPIO_DIR_Output 1
#define   GPIO_PINCNF_INPUT __FIELD(1, 1)
#define     GPIO_INPUT_Connect 0
#define     GPIO_INPUT_Disconnect 1
#define   GPIO_PINCNF_PULL __FIELD(2, 2)
#define     GPIO_PULL_Disabled 0
#define     GPII_PULL_Pulldown 1
#define     GPIO_PULL_Pullup 3
#define   GPIO_PINCNF_DRIVE __FIELD(8, 3)
#define     GPIO_DRIVE_S0S1 0
#define     GPIO_DRIVE_H0S1 1
#define     GPIO_DRIVE_S0H1 2
#define     GPIO_DRIVE_H0H1 3
#define     GPIO_DRIVE_D0S1 4
#define     GPIO_DRIVE_D0H1 5
#define     GPIO_DRIVE_S0D1 6 /* Open drain */
#define     GPIO_DRIVE_H0D1 7
#define   GPIO_PINCNF_SENSE __FIELD(16, 2)
#define     GPIO_SENSE_Disabled 0
#define     GPIO_SENSE_High 2
#define     GPIO_SENSE_Low 3
};

extern volatile struct _gpio GPIO;


/* GPIOTE */
struct _gpiote {
/* Tasks */
    unsigned OUT[4];                    // 0x000
    _PADDING(32)
    unsigned SET[4];                    // 0x030
    _PADDING(32)
    unsigned CLR[4];                    // 0x060
/* Events */
    _PADDING(144)
    unsigned IN[4];                     // 0x100
    _PADDING(108)
    unsigned PORT;                      // 0x17c
/* Registers */
    _PADDING(388)
    unsigned INTENSET;                  // 0x304
    unsigned INTENCLR;                  // 0x308
    _PADDING(516)
    unsigned CONFIG[4];                 // 0x510
#define   GPIOTE_CONFIG_MODE __FIELD(0, 2)
#define     GPIOTE_MODE_Event 1
#define     GPIOTE_MODE_Task 3
#define   GPIOTE_CONFIG_PSEL __FIELD(8, 5)
#define   GPIOTE_CONFIG_POLARITY __FIELD(16, 2)
#define     GPIOTE_POLARITY_LoToHi 1
#define     GPIOTE_POLARITY_HiToLo 2
#define     GPIOTE_POLARITY_Toggle 3
#define   GPIOTE_CONFIG_OUTINIT __FIELD(20, 1)
};

/* Interrupts */
#define GPIOTE_INT_IN0 0
#define GPIOTE_INT_IN1 1
#define GPIOTE_INT_IN2 2
#define GPIOTE_INT_IN3 3
#define GPIOTE_INT_PORT 31

extern volatile struct _gpiote GPIOTE;


/* PPI */
typedef struct { unsigned EN, DIS; } ppi_chg;

typedef struct { unsigned volatile *EEP, *TEP; } ppi_chan;

struct _ppi {
/* Tasks */
    ppi_chg CHG[6];                     // 0x000
/* Registers */
    _PADDING(1232)
    unsigned CHEN;                      // 0x500
    unsigned CHENSET;                   // 0x504
    unsigned CHENCLR;                   // 0x508
    _PADDING(4)
    ppi_chan CH[20];                    // 0x510
    _PADDING(592)
    unsigned CHGRP[6];                  // 0x800
};

extern volatile struct _ppi PPI;


/* Radio */
struct _radio {
/* Tasks */
    unsigned TXEN;                      // 0x000
    unsigned RXEN;                      // 0x004
    unsigned START;                     // 0x008
    unsigned STOP;                      // 0x00c
    unsigned DISABLE;                   // 0x010
    unsigned RSSISTART;                 // 0x014
    unsigned RSSISTOP;                  // 0x018
    unsigned BCSTART;                   // 0x01c
    unsigned BCSTOP;                    // 0x020
/* Events */
    _PADDING(220)
    unsigned READY;                     // 0x100
    unsigned ADDRESS;                   // 0x104
    unsigned PAYLOAD;                   // 0x108
    unsigned END;                       // 0x10c
    unsigned DISABLED;                  // 0x110
    unsigned DEVMATCH;                  // 0x114
    unsigned DEVMISS;                   // 0x118
    unsigned RSSIEND;                   // 0x11c
    _PADDING(8)
    unsigned BCMATCH;                   // 0x128
/* Registers */
    _PADDING(212)
    unsigned SHORTS;                    // 0x200
    _PADDING(256)
    unsigned INTENSET;                  // 0x304
    unsigned INTENCLR;                  // 0x308
    _PADDING(244)
    unsigned CRCSTATUS;                 // 0x400
    _PADDING(4)
    unsigned RXMATCH;                   // 0x408
    unsigned RXCRC;                     // 0x40c
    unsigned DAI;                       // 0x410
    _PADDING(240)
    void *PACKETPTR;                    // 0x504
    unsigned FREQUENCY;                 // 0x508
    unsigned TXPOWER;                   // 0x50c
    unsigned MODE;                      // 0x510
#define   RADIO_MODE_NRF_1Mbit 0
    unsigned PCNF0;                     // 0x514
#define   RADIO_PCNF0_LFLEN __FIELD(0, 4)
#define   RADIO_PCNF0_S0LEN __FIELD(8, 1)
#define   RADIO_PCNF0_S1LEN __FIELD(16, 4)
    unsigned PCNF1;                     // 0x518
#define   RADIO_PCNF1_MAXLEN __FIELD(0, 8)
#define   RADIO_PCNF1_STATLEN __FIELD(8, 8)
#define   RADIO_PCNF1_BALEN __FIELD(16, 3)
#define   RADIO_PCNF1_ENDIAN __FIELD(24, 1)
#define     RADIO_ENDIAN_Little 0
#define     RADIO_ENDIAN_Big 1
#define   RADIO_PCNF1_WHITEEN __BIT(25)
    unsigned BASE0;                     // 0x51c
    unsigned BASE1;                     // 0x520
    unsigned PREFIX0;                   // 0x524
    unsigned PREFIX1;                   // 0x528
    unsigned TXADDRESS;                 // 0x52c
    unsigned RXADDRESSES;               // 0x530
    unsigned CRCCNF;                    // 0x534
    unsigned CRCPOLY;                   // 0x538
    unsigned CRCINIT;                   // 0x53c
    unsigned TEST;                      // 0x540
    unsigned TIFS;                      // 0x544
    unsigned RSSISAMPLE;                // 0x548
    _PADDING(4)
    unsigned STATE;                     // 0x550
    unsigned DATAWHITEIV;               // 0x554
    _PADDING(8)
    unsigned BCC;                       // 0x560
    _PADDING(156)
    unsigned DAB[8];                    // 0x600
    unsigned DAP[8];                    // 0x620
    unsigned DACNF;                     // 0x640
    _PADDING(224)
    unsigned OVERRIDE[5];               // 0x724
    _PADDING(2244)
    unsigned POWER;                     // 0xffc
};

/* Interrupts */
#define RADIO_INT_READY 0
#define RADIO_INT_END 3
#define RADIO_INT_DISABLED 4

extern volatile struct _radio RADIO;


/* TIMERS: Timer 0 is 8/16/24/32 bit, Timers 1 and 2 are 8/16 bit. */
struct _timer {
/* Tasks */
    unsigned START;                     // 0x000
    unsigned STOP;                      // 0x004
    unsigned COUNT;                     // 0x008
    unsigned CLEAR;                     // 0x00c
    unsigned SHUTDOWN;                  // 0x010
    _PADDING(44)
    unsigned CAPTURE[4];                // 0x040
/* Events */
    _PADDING(240)
    unsigned COMPARE[4];                // 0x140
/* Registers */
    _PADDING(176)
    unsigned SHORTS;                    // 0x200
    _PADDING(256)
    unsigned INTENSET;                  // 0x304
    unsigned INTENCLR;                  // 0x308
    _PADDING(504)
    unsigned MODE;                      // 0x504
#define   TIMER_MODE_Timer 0
#define   TIMER_MODE_Counter 1
    unsigned BITMODE;                   // 0x508
#define   TIMER_BITMODE_16Bit 0
#define   TIMER_BITMODE_8Bit 1
#define   TIMER_BITMODE_24Bit 2
#define   TIMER_BITMODE_32Bit 3
    _PADDING(4)
    unsigned PRESCALER;                 // 0x510
    _PADDING(44)
    unsigned CC[4];                     // 0x540
};

/* Interrupts */
#define TIMER_INT_COMPARE0 16
#define TIMER_INT_COMPARE1 17
#define TIMER_INT_COMPARE2 18
#define TIMER_INT_COMPARE3 19
/* Shortcuts */
#define TIMER_COMPARE0_CLEAR 0
#define TIMER_COMPARE1_CLEAR 1
#define TIMER_COMPARE2_CLEAR 2
#define TIMER_COMPARE3_CLEAR 3
#define TIMER_COMPARE0_STOP 8
#define TIMER_COMPARE1_STOP 9
#define TIMER_COMPARE2_STOP 10
#define TIMER_COMPARE3_STOP 11

extern volatile struct _timer TIMER0, TIMER1, TIMER2;
extern volatile struct _timer * const TIMER[];


/* Random Number Generator */
struct _rng {
/* Tasks */
    unsigned START;                     // 0x000
    unsigned STOP;                      // 0x004
/* Events */
    _PADDING(248)
    unsigned VALRDY;                    // 0x100
/* Registers */
    _PADDING(252)
    unsigned SHORTS;                    // 0x200
    _PADDING(252)
    unsigned INTEN;                     // 0x300
    unsigned INTENSET;                  // 0x304
    unsigned INTENCLR;                  // 0x308
    _PADDING(504)
    unsigned CONFIG;                    // 0x504
#define RNG_CONFIG_DERCEN __BIT(0)
    unsigned VALUE;                     // 0x508
};

/* Interrupts */
#define RNG_INT_VALRDY 0

extern volatile struct _rng RNG;


/* Temperature sensor */
struct _temp {
/* Tasks */
    unsigned START;                     // 0x000
    unsigned STOP;                      // 0x004
/* Events */
    _PADDING(248)
    unsigned DATARDY;                   // 0x100
/* Registers */
    _PADDING(508)
    unsigned INTEN;                     // 0x300
    unsigned INTENSET;                  // 0x304
    unsigned INTENCLR;                  // 0x308
    _PADDING(508)
    unsigned VALUE;                     // 0x508
};

/* Interrupts */
#define TEMP_INT_DATARDY 0

extern volatile struct _temp TEMP;


/* I2C */
struct _i2c {
/* Tasks */
    unsigned STARTRX;                   // 0x000
    _PADDING(4)
    unsigned STARTTX;                   // 0x008
    _PADDING(8)
    unsigned STOP;                      // 0x014
    _PADDING(4)
    unsigned SUSPEND;                   // 0x01c
    unsigned RESUME;                    // 0x020
/* Events */
    _PADDING(224)
    unsigned STOPPED;                   // 0x104
    unsigned RXDREADY;                  // 0x108
    _PADDING(16)
    unsigned TXDSENT;                   // 0x11c
    _PADDING(4)
    unsigned ERROR;                     // 0x124
    _PADDING(16)
    unsigned BB;                        // 0x138
    _PADDING(12)
    unsigned SUSPENDED;                 // 0x148
/* Registers */
    _PADDING(180)
    unsigned SHORTS;                    // 0x200
    _PADDING(252)
    unsigned INTEN;                     // 0x300
    unsigned INTENSET;                  // 0x304
    unsigned INTENCLR;                  // 0x308
    _PADDING(440)
    unsigned ERRORSRC;                  // 0x4c4
#define   I2C_ERRORSRC_OVERRUN __BIT(0)
#define   I2C_ERRORSRC_ANACK __BIT(1)
#define   I2C_ERRORSRC_DNACK __BIT(2)
#define   I2C_ERRORSRC_All 0x7
    _PADDING(56)
    unsigned ENABLE;                    // 0x500
#define   I2C_ENABLE_Disabled 0
#define   I2C_ENABLE_Enabled 5
    _PADDING(4)
    unsigned PSELSCL;                   // 0x508
    unsigned PSELSDA;                   // 0x50c
    _PADDING(8)
    unsigned RXD;                       // 0x518
    unsigned TXD;                       // 0x51c
    _PADDING(4)
    unsigned FREQUENCY;                 // 0x524
#define   I2C_FREQUENCY_100kHz 0x01980000
    _PADDING(96)
    unsigned ADDRESS;                   // 0x588
    _PADDING(2672)
    unsigned POWER;                     // 0xffc
};

/* Interrupts */
#define I2C_INT_STOPPED 1
#define I2C_INT_RXDREADY 2
#define I2C_INT_TXDSENT 7
#define I2C_INT_ERROR 9
#define I2C_INT_BB 14
/* Shortcuts */
#define I2C_BB_SUSPEND 0
#define I2C_BB_STOP 1

extern volatile struct _i2c I2C0, I2C1;
extern volatile struct _i2c * const I2C[];


/* SPI */
struct _spi {
    _PADDING(264)
    unsigned READY;                     // 0x108
    _PADDING(500)
    unsigned INTEN;                     // 0x300
    unsigned INTENSET;                  // 0x304
    unsigned INTENCLR;                  // 0x308
    _PADDING(500)
    unsigned ENABLE;                    // 0x500
#define   SPI_ENABLE_Enabled 1
#define   SPI_ENABLE_Disabled 0
    _PADDING(4)
    unsigned PSELSCK;                   // 0x508
    unsigned PSELMOSI;                  // 0x50c
    unsigned PSELMISO;                  // 0x510
    _PADDING(4)
    unsigned RXD;                       // 0x518
    unsigned TXD;                       // 0x51c
    _PADDING(4)
    unsigned FREQUENCY;                 // 0x524
#define   SPI_FREQUENCY_125kHz 0x02000000
#define   SPI_FREQUENCY_250kHz 0x04000000
#define   SPI_FREQUENCY_500kHz 0x08000000
#define   SPI_FREQUENCY_1MHz   0x10000000
#define   SPI_FREQUENCY_2MHz   0x20000000
#define   SPI_FREQUENCY_4MHz   0x40000000
#define   SPI_FREQUENCY_8MHz   0x80000000
    _PADDING(44)
    unsigned CONFIG;                    // 0x554
#define   SPI_CONFIG_ORDER __FIELD(0, 1)
#define     SPI_ORDER_MsbFirst 0
#define     SPI_ORDER_LsbFirst 1
#define   SPI_CONFIG_CPHASE __FIELD(1, 1)
#define     SPI_CPHASE_Leading 0
#define     SPI_CPHASE_Trailing 1
#define   SPI_CONFIG_CPOLARITY __FIELD(2, 1)
#define     SPI_CPOLARITY_ActiveHigh 0
#define     SPI_CPOLARITY_ActiveLow 1
};

#define SPI_INT_READY 2

extern volatile struct _spi SPI0, SPI1;
extern volatile struct _spi * const SPI[];


/* UART */
struct _uart {
/* Tasks */
    unsigned STARTRX;                   // 0x000
    _PADDING(4)
    unsigned STARTTX;                   // 0x008
/* Events */
    _PADDING(252)
    unsigned RXDRDY;                    // 0x108
    _PADDING(16)
    unsigned TXDRDY;                    // 0x11c
/* Registers */
    _PADDING(484)
    unsigned INTENSET;                  // 0x304
    unsigned INTENCLR;                  // 0x308
    _PADDING(500)
    unsigned ENABLE;                    // 0x500
#define   UART_ENABLE_Disabled 0
#define   UART_ENABLE_Enabled 4
    _PADDING(8)
    unsigned PSELTXD;                   // 0x50c
    _PADDING(4)
    unsigned PSELRXD;                   // 0x514
    unsigned RXD;                       // 0x518
    unsigned TXD;                       // 0x51c
    _PADDING(4)
    unsigned BAUDRATE;                  // 0x524
#define   UART_BAUDRATE_1200   0x0004f000
#define   UART_BAUDRATE_2400   0x0009d000
#define   UART_BAUDRATE_4800   0x0013b000
#define   UART_BAUDRATE_9600   0x00275000
#define   UART_BAUDRATE_14400  0x003af000
#define   UART_BAUDRATE_19200  0x004ea000
#define   UART_BAUDRATE_28800  0x0075c000
#define   UART_BAUDRATE_31250  0x00800000
#define   UART_BAUDRATE_38400  0x009d0000
#define   UART_BAUDRATE_56000  0x00e50000
#define   UART_BAUDRATE_57600  0x00eb0000
#define   UART_BAUDRATE_76800  0x013a9000
#define   UART_BAUDRATE_115200 0x01d60000
#define   UART_BAUDRATE_230400 0x03b00000
#define   UART_BAUDRATE_250000 0x04000000
#define   UART_BAUDRATE_460800 0x07400000
#define   UART_BAUDRATE_921600 0x0f000000
#define   UART_BAUDRATE_1M     0x10000000
    _PADDING(68)
    unsigned CONFIG;                    // 0x56c
#define   UART_CONFIG_HWFC __BIT(0)
#define   UART_CONFIG_PARITY __FIELD(1, 3)
#define     UART_PARITY_None 0
#define     UART_PARITY_Even 7
};

/* Interrupts */
#define UART_INT_RXDRDY 2
#define UART_INT_TXDRDY 7

extern volatile struct _uart UART;


/* ADC */
struct _adc {
/* Tasks */
    unsigned START;                     // 0x000
    unsigned STOP;                      // 0x004
/* Events */
    _PADDING(248)
    unsigned END;                       // 0x100
/* Registers */
    _PADDING(508)
    unsigned INTEN;                     // 0x300
    unsigned INTENSET;                  // 0x304
    unsigned INTENCLR;                  // 0x308
    _PADDING(244)
    unsigned BUSY;                      // 0x400
    _PADDING(252)
    unsigned ENABLE;                    // 0x500
    unsigned CONFIG;                    // 0x504
#define   ADC_CONFIG_RES __FIELD(0, 2)
#define     ADC_RES_8Bit 0
#define     ADC_RES_9bit 1
#define     ADC_RES_10bit 2
#define   ADC_CONFIG_INPSEL __FIELD(2, 3)
#define     ADC_INPSEL_AIn_1_1 0
#define     ADC_INPSEL_AIn_2_3 1
#define     ADC_INPSEL_AIn_1_3 2
#define     ADC_INPSEL_Vdd_2_3 5
#define     ADC_INPSEL_Vdd_1_3 6
#define   ADC_CONFIG_REFSEL __FIELD(5, 2)
#define     ADC_REFSEL_BGap 0
#define     ADC_REFSEL_Ext 1
#define     ADC_REFSEL_Vdd_1_2 2
#define     ADC_REFSEL_Vdd_1_3 3
#define   ADC_CONFIG_PSEL __FIELD(8, 8)
#define   ADC_CONFIG_EXTREFSEL __FIELD(16, 2)
#define     ADC_EXTREFSEL_Ref0 1
#define     ADC_EXTREFSEL_Ref1 2
    unsigned RESULT;                    // 0x508
};

/* Interrupts */
#define ADC_INT_END 0

extern volatile struct _adc ADC;


/* NVIC stuff */

/* irq_priority -- set priority of an IRQ from 0 (highest) to 255 */
void irq_priority(int irq, unsigned priority);

/* enable_irq -- enable interrupts from an IRQ */
void enable_irq(int irq);

/* disable_irq -- disable interrupts from a specific IRQ */
void disable_irq(int irq);

/* clear_pending -- clear pending interrupt from an IRQ */
void clear_pending(int irq);

/* reschedule -- request PendSV interrupt */
#define reschedule()  SCB.ICSR = BIT(SCB_ICSR_PENDSVSET)

/* active_irq -- find active interrupt: returns -16 to 31 */
#define active_irq()  (GET_FIELD(SCB.ICSR, SCB_ICSR_VECTACTIVE) - 16)

/* delay_loop -- timed delay */
void delay_loop(unsigned usec);


/* GPIO convenience */

/* gpio_dir -- set GPIO direction */
void gpio_dir(unsigned pin, unsigned dir);

/* gpio_connect -- connect pin for input */
void gpio_connect(unsigned pin);

/* gpio_drive -- set GPIO drive strength */
void gpio_drive(unsigned pin, unsigned mode);

/* gpio_out -- set GPIO output value */
void gpio_out(unsigned pin, unsigned value);

/* gpio_in -- get GPIO input bit */
unsigned gpio_in(unsigned pin);


/* Image constants */

#define NIMG 3

typedef unsigned image[NIMG];

#define _ROW(r, c1, c2, c3, c4, c5, c6, c7, c8, c9)                  \
    (BIT(r) | (!c1<<4) | (!c2<<5) | (!c3<<6) | (!c4<<7) | (!c5<<8)   \
     | (!c6<<9) | (!c7<<10) | (!c8<<11) | (!c9<<12))

#define IMAGE(x11, x24, x12, x25, x13,                               \
              x34, x35, x36, x37, x38,                               \
              x22, x19, x23, x39, x21,                               \
              x18, x17, x16, x15, x14,                               \
              x33, x27, x31, x26, x32)                               \
    { _ROW(ROW1, x11, x12, x13, x14, x15, x16, x17, x18, x19),       \
      _ROW(ROW2, x21, x22, x23, x24, x25, x26, x27, 0, 0),           \
      _ROW(ROW3, x31, x32, x33, x34, x35, x36, x37, x38, x39) }

#define LED_MASK 0xfff0

#define led_init()  GPIO.DIRSET = LED_MASK
#define led_dot()   GPIO.OUTSET = 0x5fbf
#define led_off()   GPIO.OUTCLR = LED_MASK


/* A few assembler macros for single instructions. */
#define pause()         asm volatile ("wfe")
#define intr_disable()  asm volatile ("cpsid i")
#define intr_enable()   asm volatile ("cpsie i")
#define get_primask()   ({ unsigned x;                                   \
                           asm volatile ("mrs %0, primask" : "=r" (x)); x; })
#define set_primask(x)  asm volatile ("msr primask, %0" : : "r" (x))
#define nop()           asm volatile ("nop")
