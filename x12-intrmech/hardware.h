/* common/hardware.h */
/* Copyright (c) 2018-20 J. M. Spivey */

#define UBIT 1
#define UBIT_V1 1

/* Hardware register definitions for nRF51822 */

#define BIT(i) (1 << (i))
#define GET_BIT(reg, n) (((reg) >> (n)) & 0x1)
#define SET_BIT(reg, n) reg |= BIT(n)
#define CLR_BIT(reg, n) reg &= ~BIT(n)

#define GET_BYTE(reg, n) (((reg) >> (8*(n))) & 0xff)
#define SET_BYTE(reg, n, v) \
    reg = (reg & ~(0xff << 8*n)) | ((v & 0xff) << 8*n)

/* The macros SET_FIELD, etc., are defined in an indirect way that
permits (because of the timing of CPP macro expansion) the 'field'
argument to be a macro that expands the a 'position, width' pair. */

#define SET_FIELD(reg, field, val) _SET_FIELD(reg, field, val)
#define _SET_FIELD(reg, pos, wd, val) \
    reg = (reg & ~_MASK(pos, wd)) | _FIELD(pos, wd, val)

#define GET_FIELD(reg, field) _GET_FIELD(reg, field)
#define _GET_FIELD(reg, pos, wd)  ((reg >> pos) & _MASK0(wd))

#define FIELD(field, val) _FIELD(field, val)
#define _FIELD(pos, wd, val)  (((val) & _MASK0(wd)) << pos)

#define MASK(field) _MASK(field)
#define _MASK(pos, wd)  (_MASK0(wd) << pos)

#define _MASK0(wd)  (~((-2) << (wd-1)))

#define __BIT(pos) pos
#define __FIELD(pos, wd) pos, wd


/* Device pins */
#define PAD19 0
#define   I2C_SCL PAD19
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
#define   BUTTON_A PAD5
#define PAD8 18
#define PAD12 20
#define PAD15 21
#define   SPI_MOSI PAD15
#define PAD14 22
#define   SPI_MISO PAD14
#define PAD13 23
#define   SPI_SCK PAD13
#define USB_TX 24
#define USB_RX 25
#define PAD11 26
#define   BUTTON_B PAD11
#define PAD20 30
#define   I2C_SDA PAD20


/* Interrupts */
#define SVC_IRQ    -5
#define PENDSV_IRQ -2
#define RADIO_IRQ   1
#define UART_IRQ    2
#define I2C_IRQ     3
#define SPI_IRQ     4
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


/* Device registers */
#define _BASE(addr) ((unsigned volatile *) addr)
#define _REG(ty, addr) (* (ty volatile *) addr)
#define _ARR(ty, addr) ((ty volatile *) addr)


/* System contol block */

#define SCB_BASE                        _BASE(0xe000ed00)
#define SCB_CPUID                       _REG(unsigned, 0xe000ed00)
#define SCB_ICSR                        _REG(unsigned, 0xe000ed04)
#define   SCB_ICSR_PENDSVSET __BIT(28)
#define   SCB_ICSR_VECTACTIVE __FIELD(0, 8)
#define SCB_SCR                         _REG(unsigned, 0xe000ed10)
#define   SCB_SCR_SLEEPONEXIT __BIT(1)
#define   SCB_SCR_SLEEPDEEP __BIT(2)
#define   SCB_SCR_SEVONPEND __BIT(4)
#define SCB_SHPR                        _ARR(unsigned, 0xe000ed18)


/* Nested vectored interupt controller */

#define NVIC_BASE                       _BASE(0xe000e000)
#define NVIC_ISER                       _ARR(unsigned, 0xe000e100)
#define NVIC_ICER                       _ARR(unsigned, 0xe000e180)
#define NVIC_ISPR                       _ARR(unsigned, 0xe000e200)
#define NVIC_ICPR                       _ARR(unsigned, 0xe000e280)
#define NVIC_IPR                        _ARR(unsigned, 0xe000e400)


/* Clock control */

#define CLOCK_BASE                      _BASE(0x40000000)
#define CLOCK_HFCLKSTART                _REG(unsigned, 0x40000000)
#define CLOCK_LFCLKSTART                _REG(unsigned, 0x40000008)
#define CLOCK_HFCLKSTARTED              _REG(unsigned, 0x40000100)
#define CLOCK_LFCLKSTARTED              _REG(unsigned, 0x40000104)
#define CLOCK_LFCLKSRC                  _REG(unsigned, 0x40000518)
#define   CLOCK_LFCLKSRC_RC 0
#define CLOCK_XTALFREQ                  _REG(unsigned, 0x40000550)
#define   CLOCK_XTALFREQ_16MHz 0xFF


/* Memory protection unit */

#define MPU_BASE                        _BASE(0x40000000)
#define MPU_DISABLEINDEBUG              _REG(unsigned, 0x40000608)


/* Factory information */

#define FICR_BASE                       _BASE(0x10000000)
#define FICR_DEVICEID                   _ARR(unsigned, 0x10000060)
#define FICR_DEVICEADDR                 _ARR(unsigned, 0x100000a4)
#define FICR_OVERRIDEEN                 _REG(unsigned, 0x100000a0)
#define   FICR_OVERRIDEEN_NRF __BIT(0)
#define FICR_NRF_1MBIT                  _ARR(unsigned, 0x100000b0)


/* Power management */

/* Interrupts */
#define POWER_INT_POFWARN 2

#define POWER_BASE                      _BASE(0x40000000)
/* Tasks */
#define POWER_CONSTLAT                  _REG(unsigned, 0x40000078)
#define POWER_LOWPWR                    _REG(unsigned, 0x4000007c)
/* Events */
#define POWER_POFWARN                   _REG(unsigned, 0x40000108)
/* Registers */
#define POWER_INTENSET                  _REG(unsigned, 0x40000304)
#define POWER_INTENCLR                  _REG(unsigned, 0x40000308)
#define POWER_RESETREAS                 _REG(unsigned, 0x40000400)
#define   POWER_RESETREAS_RESETPIN __BIT(0)
#define   POWER_RESETREAS_DOG __BIT(1)
#define   POWER_RESETREAS_SREQ __BIT(2)
#define   POWER_RESETREAS_LOCKUP __BIT(3)
#define   POWER_RESETREAS_OFF __BIT(16)
#define   POWER_RESETREAS_LPCOMP __BIT(17)
#define   POWER_RESETREAS_DIF _BIT(18)
#define   POWER_RESETREAS_ALL 0x0007000f
#define POWER_RAMSTATUS                 _REG(unsigned, 0x40000428)
#define POWER_SYSTEMOFF                 _REG(unsigned, 0x40000500)
#define POWER_POFCON                    _REG(unsigned, 0x40000510)
#define   POWER_POFCON_POF __BIT(1)
#define   POWER_POFCON_TRESHOLD _FIELD(1, 2)
#define     POWER_THRESHOLD_V21 0
#define     POWER_THRESHOLD_V23 1
#define     POWER_THRESHOLD_V25 2
#define     POWER_THRESHOLD_V27 3
#define POWER_GPREGRET                  _REG(unsigned, 0x4000051c)
#define POWER_RAMON                     _REG(unsigned, 0x40000524)
#define POWER_RESET                     _REG(unsigned, 0x40000544)
#define POWER_RAMONB                    _REG(unsigned, 0x40000554)
#define POWER_DCDCEN                    _REG(unsigned, 0x40000578)


/* Watchdog timer */

/* Interrupts */
#define WDT_INT_TIMEOUT 0

#define WDT_BASE                        _BASE(0x40010000)
/* Tasks */
#define WDT_START                       _REG(unsigned, 0x40010000)
/* Events */
#define WDT_TIMEOUT                     _REG(unsigned, 0x40010100)
/* Registers */
#define WDT_INTENSET                    _REG(unsigned, 0x40010304)
#define WDT_INTENCLR                    _REG(unsigned, 0x40010308)
#define WDT_RUNSTATUS                   _REG(unsigned, 0x40010400)
#define WDT_REQSTATUS                   _REG(unsigned, 0x40010404)
#define WDT_CRV                         _REG(unsigned, 0x40010504)
#define   WDT_HERTZ 32768
#define WDT_RREN                        _REG(unsigned, 0x40010508)
#define WDT_CONFIG                      _REG(unsigned, 0x4001050c)
#define   WDT_CONFIG_SLEEP __BIT(0)
#define   WDT_CONFIG_HALT __BIT(3)
#define WDT_RR                          _ARR(unsigned, 0x40010600)
#define   WDT_MAGIC 0x6e524635


/* Non-Volatile Memory Controller */

#define NVMC_BASE                       _BASE(0x4001e000)
#define NVMC_READY                      _REG(unsigned, 0x4001e400)
#define NVMC_CONFIG                     _REG(unsigned, 0x4001e504)
#define   NVMC_CONFIG_WEN __BIT(0)
#define   NVMC_CONFIG_EEN __BIT(1)
#define NVMC_ERASEPAGE                  _REG(void*, 0x4001e508)


/* GPIO */

#define GPIO_BASE                       _BASE(0x50000500)
/* Registers */
#define GPIO_OUT                        _REG(unsigned, 0x50000504)
#define GPIO_OUTSET                     _REG(unsigned, 0x50000508)
#define GPIO_OUTCLR                     _REG(unsigned, 0x5000050c)
#define GPIO_IN                         _REG(unsigned, 0x50000510)
#define GPIO_DIR                        _REG(unsigned, 0x50000514)
#define GPIO_DIRSET                     _REG(unsigned, 0x50000518)
#define GPIO_DIRCLR                     _REG(unsigned, 0x5000051c)
#define GPIO_PINCNF                     _ARR(unsigned, 0x50000700)
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


/* GPIOTE */

/* Interrupts */
#define GPIOTE_INT_IN0 0
#define GPIOTE_INT_IN1 1
#define GPIOTE_INT_IN2 2
#define GPIOTE_INT_IN3 3
#define GPIOTE_INT_PORT 31

#define GPIOTE_BASE                     _BASE(0x40006000)
/* Tasks */
#define GPIOTE_OUT                      _ARR(unsigned, 0x40006000)
#define GPIOTE_SET                      _ARR(unsigned, 0x40006030)
#define GPIOTE_CLR                      _ARR(unsigned, 0x40006060)
/* Events */
#define GPIOTE_IN                       _ARR(unsigned, 0x40006100)
#define GPIOTE_PORT                     _REG(unsigned, 0x4000617c)
/* Registers */
#define GPIOTE_INTENSET                 _REG(unsigned, 0x40006304)
#define GPIOTE_INTENCLR                 _REG(unsigned, 0x40006308)
#define GPIOTE_CONFIG                   _ARR(unsigned, 0x40006510)
#define   GPIOTE_CONFIG_MODE __FIELD(0, 2)
#define     GPIOTE_MODE_Event 1
#define     GPIOTE_MODE_Task 3
#define   GPIOTE_CONFIG_PSEL __FIELD(8, 5)
#define   GPIOTE_CONFIG_POLARITY __FIELD(16, 2)
#define     GPIOTE_POLARITY_LoToHi 1
#define     GPIOTE_POLARITY_HiToLo 2
#define     GPIOTE_POLARITY_Toggle 3
#define   GPIOTE_CONFIG_OUTINIT __FIELD(20, 1)


/* PPI */
typedef struct { unsigned EN, DIS; } ppi_chg;

typedef struct { unsigned volatile *EEP, *TEP; } ppi_chan;


#define PPI_BASE                        _BASE(0x4001f000)
/* Tasks */
#define PPI_CHG                         _ARR(ppi_chg, 0x4001f000)
/* Registers */
#define PPI_CHEN                        _REG(unsigned, 0x4001f500)
#define PPI_CHENSET                     _REG(unsigned, 0x4001f504)
#define PPI_CHENCLR                     _REG(unsigned, 0x4001f508)
#define PPI_CH                          _ARR(ppi_chan, 0x4001f510)
#define PPI_CHGRP                       _ARR(unsigned, 0x4001f800)


/* Radio */

/* Interrupts */
#define RADIO_INT_READY 0
#define RADIO_INT_END 3
#define RADIO_INT_DISABLED 4

#define RADIO_BASE                      _BASE(0x40001000)
/* Tasks */
#define RADIO_TXEN                      _REG(unsigned, 0x40001000)
#define RADIO_RXEN                      _REG(unsigned, 0x40001004)
#define RADIO_START                     _REG(unsigned, 0x40001008)
#define RADIO_STOP                      _REG(unsigned, 0x4000100c)
#define RADIO_DISABLE                   _REG(unsigned, 0x40001010)
#define RADIO_RSSISTART                 _REG(unsigned, 0x40001014)
#define RADIO_RSSISTOP                  _REG(unsigned, 0x40001018)
#define RADIO_BCSTART                   _REG(unsigned, 0x4000101c)
#define RADIO_BCSTOP                    _REG(unsigned, 0x40001020)
/* Events */
#define RADIO_READY                     _REG(unsigned, 0x40001100)
#define RADIO_ADDRESS                   _REG(unsigned, 0x40001104)
#define RADIO_PAYLOAD                   _REG(unsigned, 0x40001108)
#define RADIO_END                       _REG(unsigned, 0x4000110c)
#define RADIO_DISABLED                  _REG(unsigned, 0x40001110)
#define RADIO_DEVMATCH                  _REG(unsigned, 0x40001114)
#define RADIO_DEVMISS                   _REG(unsigned, 0x40001118)
#define RADIO_RSSIEND                   _REG(unsigned, 0x4000111c)
#define RADIO_BCMATCH                   _REG(unsigned, 0x40001128)
/* Registers */
#define RADIO_SHORTS                    _REG(unsigned, 0x40001200)
#define RADIO_INTENSET                  _REG(unsigned, 0x40001304)
#define RADIO_INTENCLR                  _REG(unsigned, 0x40001308)
#define RADIO_CRCSTATUS                 _REG(unsigned, 0x40001400)
#define RADIO_RXMATCH                   _REG(unsigned, 0x40001408)
#define RADIO_RXCRC                     _REG(unsigned, 0x4000140c)
#define RADIO_DAI                       _REG(unsigned, 0x40001410)
#define RADIO_PACKETPTR                 _REG(void*, 0x40001504)
#define RADIO_FREQUENCY                 _REG(unsigned, 0x40001508)
#define RADIO_TXPOWER                   _REG(unsigned, 0x4000150c)
#define RADIO_MODE                      _REG(unsigned, 0x40001510)
#define   RADIO_MODE_NRF_1Mbit 0
#define RADIO_PCNF0                     _REG(unsigned, 0x40001514)
#define   RADIO_PCNF0_LFLEN __FIELD(0, 4)
#define   RADIO_PCNF0_S0LEN __FIELD(8, 1)
#define   RADIO_PCNF0_S1LEN __FIELD(16, 4)
#define RADIO_PCNF1                     _REG(unsigned, 0x40001518)
#define   RADIO_PCNF1_MAXLEN __FIELD(0, 8)
#define   RADIO_PCNF1_STATLEN __FIELD(8, 8)
#define   RADIO_PCNF1_BALEN __FIELD(16, 3)
#define   RADIO_PCNF1_ENDIAN __FIELD(24, 1)
#define     RADIO_ENDIAN_Little 0
#define     RADIO_ENDIAN_Big 1
#define   RADIO_PCNF1_WHITEEN __BIT(25)
#define RADIO_BASE0                     _REG(unsigned, 0x4000151c)
#define RADIO_BASE1                     _REG(unsigned, 0x40001520)
#define RADIO_PREFIX0                   _REG(unsigned, 0x40001524)
#define RADIO_PREFIX1                   _REG(unsigned, 0x40001528)
#define RADIO_TXADDRESS                 _REG(unsigned, 0x4000152c)
#define RADIO_RXADDRESSES               _REG(unsigned, 0x40001530)
#define RADIO_CRCCNF                    _REG(unsigned, 0x40001534)
#define RADIO_CRCPOLY                   _REG(unsigned, 0x40001538)
#define RADIO_CRCINIT                   _REG(unsigned, 0x4000153c)
#define RADIO_TEST                      _REG(unsigned, 0x40001540)
#define RADIO_TIFS                      _REG(unsigned, 0x40001544)
#define RADIO_RSSISAMPLE                _REG(unsigned, 0x40001548)
#define RADIO_STATE                     _REG(unsigned, 0x40001550)
#define RADIO_DATAWHITEIV               _REG(unsigned, 0x40001554)
#define RADIO_BCC                       _REG(unsigned, 0x40001560)
#define RADIO_DAB                       _ARR(unsigned, 0x40001600)
#define RADIO_DAP                       _ARR(unsigned, 0x40001620)
#define RADIO_DACNF                     _REG(unsigned, 0x40001640)
#define RADIO_OVERRIDE                  _ARR(unsigned, 0x40001724)
#define RADIO_POWER                     _REG(unsigned, 0x40001ffc)


/* TIMERS: Timer 0 is 8/16/24/32 bit, Timers 1 and 2 are 8/16 bit. */

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

/* Timer 0: 32 bit */
#define TIMER0_BASE                     _BASE(0x40008000)
/* Tasks */
#define TIMER0_START                    _REG(unsigned, 0x40008000)
#define TIMER0_STOP                     _REG(unsigned, 0x40008004)
#define TIMER0_COUNT                    _REG(unsigned, 0x40008008)
#define TIMER0_CLEAR                    _REG(unsigned, 0x4000800c)
#define TIMER0_SHUTDOWN                 _REG(unsigned, 0x40008010)
#define TIMER0_CAPTURE                  _ARR(unsigned, 0x40008040)
/* Events */
#define TIMER0_COMPARE                  _ARR(unsigned, 0x40008140)
/* Registers */
#define TIMER0_SHORTS                   _REG(unsigned, 0x40008200)
#define TIMER0_INTENSET                 _REG(unsigned, 0x40008304)
#define TIMER0_INTENCLR                 _REG(unsigned, 0x40008308)
#define TIMER0_MODE                     _REG(unsigned, 0x40008504)
#define   TIMER_MODE_Timer 0
#define   TIMER_MODE_Counter 1
#define TIMER0_BITMODE                  _REG(unsigned, 0x40008508)
#define   TIMER_BITMODE_16Bit 0
#define   TIMER_BITMODE_8Bit 1
#define   TIMER_BITMODE_24Bit 2
#define   TIMER_BITMODE_32Bit 3
#define TIMER0_PRESCALER                _REG(unsigned, 0x40008510)
#define TIMER0_CC                       _ARR(unsigned, 0x40008540)

/* Timer 1: 16 bit */
#define TIMER1_BASE                     _BASE(0x40009000)
#define TIMER1_START                    _REG(unsigned, 0x40009000)
#define TIMER1_STOP                     _REG(unsigned, 0x40009004)
#define TIMER1_COUNT                    _REG(unsigned, 0x40009008)
#define TIMER1_CLEAR                    _REG(unsigned, 0x4000900c)
#define TIMER1_SHUTDOWN                 _REG(unsigned, 0x40009010)
#define TIMER1_CAPTURE                  _ARR(unsigned, 0x40009040)
#define TIMER1_COMPARE                  _ARR(unsigned, 0x40009140)
#define TIMER1_SHORTS                   _REG(unsigned, 0x40009200)
#define TIMER1_INTENSET                 _REG(unsigned, 0x40009304)
#define TIMER1_INTENCLR                 _REG(unsigned, 0x40009308)
#define TIMER1_MODE                     _REG(unsigned, 0x40009504)
#define TIMER1_BITMODE                  _REG(unsigned, 0x40009508)
#define TIMER1_PRESCALER                _REG(unsigned, 0x40009510)
#define TIMER1_CC                       _ARR(unsigned, 0x40009540)

/* Timer 2: 16 bit */
#define TIMER2_BASE                     _BASE(0x4000a000)
#define TIMER2_START                    _REG(unsigned, 0x4000a000)
#define TIMER2_STOP                     _REG(unsigned, 0x4000a004)
#define TIMER2_COUNT                    _REG(unsigned, 0x4000a008)
#define TIMER2_CLEAR                    _REG(unsigned, 0x4000a00c)
#define TIMER2_SHUTDOWN                 _REG(unsigned, 0x4000a010)
#define TIMER2_CAPTURE                  _ARR(unsigned, 0x4000a040)
#define TIMER2_COMPARE                  _ARR(unsigned, 0x4000a140)
#define TIMER2_SHORTS                   _REG(unsigned, 0x4000a200)
#define TIMER2_INTENSET                 _REG(unsigned, 0x4000a304)
#define TIMER2_INTENCLR                 _REG(unsigned, 0x4000a308)
#define TIMER2_MODE                     _REG(unsigned, 0x4000a504)
#define TIMER2_BITMODE                  _REG(unsigned, 0x4000a508)
#define TIMER2_PRESCALER                _REG(unsigned, 0x4000a510)
#define TIMER2_CC                       _ARR(unsigned, 0x4000a540)


/* Random Number Generator */

/* Interrupts */
#define RNG_INT_VALRDY 0

#define RNG_BASE                        _BASE(0x4000d000)
/* Tasks */
#define RNG_START                       _REG(unsigned, 0x4000d000)
#define RNG_STOP                        _REG(unsigned, 0x4000d004)
/* Events */
#define RNG_VALRDY                      _REG(unsigned, 0x4000d100)
/* Registers */
#define RNG_SHORTS                      _REG(unsigned, 0x4000d200)
#define RNG_INTEN                       _REG(unsigned, 0x4000d300)
#define RNG_INTENSET                    _REG(unsigned, 0x4000d304)
#define RNG_INTENCLR                    _REG(unsigned, 0x4000d308)
#define RNG_CONFIG                      _REG(unsigned, 0x4000d504)
#define RNG_CONFIG_DERCEN __BIT(0)
#define RNG_VALUE                       _REG(unsigned, 0x4000d508)


/* Temperature sensor */

/* Interrupts */
#define TEMP_INT_DATARDY 0

#define TEMP_BASE                       _BASE(0x4000c000)
/* Tasks */
#define TEMP_START                      _REG(unsigned, 0x4000c000)
#define TEMP_STOP                       _REG(unsigned, 0x4000c004)
/* Events */
#define TEMP_DATARDY                    _REG(unsigned, 0x4000c100)
/* Registers */
#define TEMP_INTEN                      _REG(unsigned, 0x4000c300)
#define TEMP_INTENSET                   _REG(unsigned, 0x4000c304)
#define TEMP_INTENCLR                   _REG(unsigned, 0x4000c308)
#define TEMP_VALUE                      _REG(unsigned, 0x4000c508)


/* I2C */
#define N_I2C 1
#define I2C_INTERNAL 0
#define I2C_EXTERNAL 0


/* Interrupts */
#define I2C_INT_STOPPED 1
#define I2C_INT_RXDREADY 2
#define I2C_INT_TXDSENT 7
#define I2C_INT_ERROR 9
#define I2C_INT_BB 14
/* Shortcuts */
#define I2C_BB_SUSPEND 0
#define I2C_BB_STOP 1

#define I2C_BASE                        _BASE(0x40003000)
/* Tasks */
#define I2C_STARTRX                     _REG(unsigned, 0x40003000)
#define I2C_STARTTX                     _REG(unsigned, 0x40003008)
#define I2C_STOP                        _REG(unsigned, 0x40003014)
#define I2C_SUSPEND                     _REG(unsigned, 0x4000301c)
#define I2C_RESUME                      _REG(unsigned, 0x40003020)
/* Events */
#define I2C_STOPPED                     _REG(unsigned, 0x40003104)
#define I2C_RXDREADY                    _REG(unsigned, 0x40003108)
#define I2C_TXDSENT                     _REG(unsigned, 0x4000311c)
#define I2C_ERROR                       _REG(unsigned, 0x40003124)
#define I2C_BB                          _REG(unsigned, 0x40003138)
#define I2C_SUSPENDED                   _REG(unsigned, 0x40003148)
/* Registers */
#define I2C_SHORTS                      _REG(unsigned, 0x40003200)
#define I2C_INTEN                       _REG(unsigned, 0x40003300)
#define I2C_INTENSET                    _REG(unsigned, 0x40003304)
#define I2C_INTENCLR                    _REG(unsigned, 0x40003308)
#define I2C_ERRORSRC                    _REG(unsigned, 0x400034c4)
#define   I2C_ERRORSRC_OVERRUN __BIT(0)
#define   I2C_ERRORSRC_ANACK __BIT(1)
#define   I2C_ERRORSRC_DNACK __BIT(2)
#define   I2C_ERRORSRC_All 0x7
#define I2C_ENABLE                      _REG(unsigned, 0x40003500)
#define   I2C_ENABLE_Disabled 0
#define   I2C_ENABLE_Enabled 5
#define I2C_PSELSCL                     _REG(unsigned, 0x40003508)
#define I2C_PSELSDA                     _REG(unsigned, 0x4000350c)
#define I2C_RXD                         _REG(unsigned, 0x40003518)
#define I2C_TXD                         _REG(unsigned, 0x4000351c)
#define I2C_FREQUENCY                   _REG(unsigned, 0x40003524)
#define   I2C_FREQUENCY_100kHz 0x01980000
#define I2C_ADDRESS                     _REG(unsigned, 0x40003588)
#define I2C_POWER                       _REG(unsigned, 0x40003ffc)


/* UART */

/* Interrupts */
#define UART_INT_RXDRDY 2
#define UART_INT_TXDRDY 7

#define UART_BASE                       _BASE(0x40002000)
/* Tasks */
#define UART_STARTRX                    _REG(unsigned, 0x40002000)
#define UART_STARTTX                    _REG(unsigned, 0x40002008)
/* Events */
#define UART_RXDRDY                     _REG(unsigned, 0x40002108)
#define UART_TXDRDY                     _REG(unsigned, 0x4000211c)
/* Registers */
#define UART_INTENSET                   _REG(unsigned, 0x40002304)
#define UART_INTENCLR                   _REG(unsigned, 0x40002308)
#define UART_ENABLE                     _REG(unsigned, 0x40002500)
#define   UART_ENABLE_Disabled 0
#define   UART_ENABLE_Enabled 4
#define UART_PSELTXD                    _REG(unsigned, 0x4000250c)
#define UART_PSELRXD                    _REG(unsigned, 0x40002514)
#define UART_RXD                        _REG(unsigned, 0x40002518)
#define UART_TXD                        _REG(unsigned, 0x4000251c)
#define UART_BAUDRATE                   _REG(unsigned, 0x40002524)
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
#define UART_CONFIG                     _REG(unsigned, 0x4000256c)
#define   UART_CONFIG_HWFC __BIT(0)
#define   UART_CONFIG_PARITY __FIELD(1, 3)
#define     UART_PARITY_None 0
#define     UART_PARITY_Even 7


/* ADC */

/* Interrupts */
#define ADC_INT_END 0

#define ADC_BASE                        _BASE(0x40007000)
/* Tasks */
#define ADC_START                       _REG(unsigned, 0x40007000)
#define ADC_STOP                        _REG(unsigned, 0x40007004)
/* Events */
#define ADC_END                         _REG(unsigned, 0x40007100)
/* Registers */
#define ADC_INTEN                       _REG(unsigned, 0x40007300)
#define ADC_INTENSET                    _REG(unsigned, 0x40007304)
#define ADC_INTENCLR                    _REG(unsigned, 0x40007308)
#define ADC_BUSY                        _REG(unsigned, 0x40007400)
#define ADC_ENABLE                      _REG(unsigned, 0x40007500)
#define ADC_CONFIG                      _REG(unsigned, 0x40007504)
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
#define ADC_RESULT                      _REG(unsigned, 0x40007508)


/* NVIC stuff */

/* irq_priority -- set priority of an IRQ from 0 (highest) to 255 */
void irq_priority(int irq, unsigned priority);

/* enable_irq -- enable interrupts from an IRQ */
#define enable_irq(irq)  NVIC_ISER[0] = BIT(irq)

/* disable_irq -- disable interrupts from a specific IRQ */
#define disable_irq(irq)  NVIC_ICER[0] = BIT(irq)

/* clear_pending -- clear pending interrupt from an IRQ */
#define clear_pending(irq)  NVIC_ICPR[0] = BIT(irq)

/* reschedule -- request PendSV interrupt */
#define reschedule()  SCB_ICSR = BIT(SCB_ICSR_PENDSVSET)

/* active_irq -- find active interrupt: returns -16 to 31 */
#define active_irq()  (GET_FIELD(SCB_ICSR, SCB_ICSR_VECTACTIVE) - 16)

/* delay_loop -- timed delay */
void delay_loop(unsigned usec);


/* GPIO convenience */

#ifndef INLINE
#define INLINE inline
#endif

/* gpio_dir -- set GPIO direction */
INLINE void gpio_dir(unsigned pin, unsigned dir) {
    if (dir)
        GPIO_DIRSET = BIT(pin);
    else
        GPIO_DIRCLR = BIT(pin);
}

/* gpio_connect -- connect pin for input */
INLINE void gpio_connect(unsigned pin) {
    SET_FIELD(GPIO_PINCNF[pin], GPIO_PINCNF_INPUT, GPIO_INPUT_Connect);
}

/* gpio_drive -- set GPIO drive strength */
INLINE void gpio_drive(unsigned pin, unsigned mode) {
    SET_FIELD(GPIO_PINCNF[pin], GPIO_PINCNF_DRIVE, mode);
}

/* gpio_out -- set GPIO output value */
INLINE void gpio_out(unsigned pin, unsigned value) {
    if (value)
        GPIO_OUTSET = BIT(pin);
    else
        GPIO_OUTCLR = BIT(pin);
}

/* gpio_in -- get GPIO input bit */
INLINE unsigned gpio_in(unsigned pin) {
    return GET_BIT(GPIO_IN, pin);
}


/* Image constants */

#define NIMG 3

typedef unsigned image[NIMG];

#define _ROW(r, c1, c2, c3, c4, c5, c6, c7, c8, c9)                 \
    (BIT(r) | !c1<<4 | !c2<<5 | !c3<<6 | !c4<<7 | !c5<<8             \
     | !c6<<9 | !c7<<10 | !c8<<11 | !c9<<12)

#define IMAGE(x11, x24, x12, x25, x13,                               \
              x34, x35, x36, x37, x38,                               \
              x22, x19, x23, x39, x21,                               \
              x18, x17, x16, x15, x14,                               \
              x33, x27, x31, x26, x32)                               \
    { _ROW(ROW1, x11, x12, x13, x14, x15, x16, x17, x18, x19),      \
      _ROW(ROW2, x21, x22, x23, x24, x25, x26, x27, 0, 0),          \
      _ROW(ROW3, x31, x32, x33, x34, x35, x36, x37, x38, x39) }

#define LED_MASK 0xfff0

#define led_init()  GPIO_DIRSET = LED_MASK
#define led_dot()   GPIO_OUTSET = 0x5fbf
#define led_off()   GPIO_OUTCLR = LED_MASK


/* A few assembler macros for single instructions. */
#define pause()         asm volatile ("wfe")
#define intr_disable()  asm volatile ("cpsid i")
#define intr_enable()   asm volatile ("cpsie i")
#define get_primask()   ({ unsigned x;                                   \
                           asm volatile ("mrs %0, primask" : "=r" (x)); x; })
#define set_primask(x)  asm volatile ("msr primask, %0" : : "r" (x))
#define nop()           asm volatile ("nop")
#define syscall(op)     asm volatile ("svc %0" : : "i"(op))
