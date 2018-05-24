/*
 *  STM8 defines for inline assembly.
 *  Names are preceded with underscore.
 */

#define _UNIQUE_ID	0x4865       // 12-byte unique id

#define _PORT_A		0x5000
#define _PORT_B		0x5005
#define _PORT_C		0x500a
#define _PORT_D		0x500f
#define _PORT_E		0x5014
#define _PORT_F		0x5010

#define _PORT_ODR	0
#define _PORT_IDR	1
#define _PORT_DDR	2
#define _PORT_CR1	3
#define _PORT_CR2	4

#define _PA_ODR		0x5000
#define _PA_IDR		0x5001
#define _PA_DDR		0x5002
#define _PA_CR1		0x5003
#define _PA_CR2		0x5004

#define _PB_ODR		0x5005
#define _PB_IDR		0x5006
#define _PB_DDR		0x5007
#define _PB_CR1		0x5008
#define _PB_CR2		0x5009

#define _PC_ODR		0x500a
#define _PC_IDR		0x500b
#define _PC_DDR		0x500c
#define _PC_CR1		0x500d
#define _PC_CR2		0x500e

#define _PD_ODR		0x500f
#define _PD_IDR		0x5010
#define _PD_DDR		0x5011
#define _PD_CR1		0x5012
#define _PD_CR2		0x5013

#define _PE_ODR		0x5014
#define _PE_IDR		0x5015
#define _PE_DDR		0x5016
#define _PE_CR1		0x5017
#define _PE_CR2		0x5018

#define _PF_ODR		0x5019
#define _PF_IDR		0x501a
#define _PF_DDR		0x501b
#define _PF_CR1		0x501c
#define _PF_CR2		0x501d

#define _FLASH_CR1	0x505a     // Flash control 1
#define _FLASH_CR2	0x505b     // Flash control 2
#define _FLASH_NCR2	0x505c     // Flash control 2 complement
#define _FLASH_FPR	0x505d     // Flash protection
#define _FLASH_NFPR	0x505e     // Flash protection complement
#define _FLASH_IAPSR	0x505f     // Flash control 1
#define _FLASH_PUKR	0x5062     // Flash in-application program status
#define _FLASH_DUKR	0x5064     // Data EEPROM unprotect

#define	_EXTI_CR1	0x50a0
#define _EXTI_CR2	0x50a1

#define _TIM2_CR1        0x5300     // TIM2 Control 1
#define _TIM2_IER        0x5303     // TIM2 Interrupt enable
#define _TIM2_SR1        0x5304     // TIM2 Status 1
#define _TIM2_SR2        0x5305     // TIM2 Status 2
#define _TIM2_EGR        0x5306     // TIM2 Event generation
#define _TIM2_CCMR1      0x5307     // TIM2 cap/comp mode 1
#define _TIM2_CCMR2      0x5308     // TIM2 cap/comp mode 2
#define _TIM2_CCMR3      0x5309     // TIM2 cap/comp mode 3
#define _TIM2_CCER1      0x530a     // TIM2 cap/comp enable 1
#define _TIM2_CCER2      0x530b     // TIM2 cap/comp enable 2
#define _TIM2_CNTRH      0x530c     // TIM2 counter high
#define _TIM2_CNTRL      0x530d     // TIM2 counter low
#define _TIM2_PSCR       0x530e     // TIM2 prescaler
#define _TIM2_ARRH       0x530f     // TIM2 auto-reload high
#define _TIM2_ARRL       0x5310     // TIM2 auto-reload low
#define _TIM2_CCR1H      0x5311     // TIM2 cap/comp 1 high
#define _TIM2_CCR1L      0x5312     // TIM2 cap/comp 1 low
#define _TIM2_CCR2H      0x5313     // TIM2 cap/comp 2 high
#define _TIM2_CCR2L      0x5314     // TIM2 cap/comp 2 low
#define _TIM2_CCR3H      0x5315     // TIM2 cap/comp 3 high
#define _TIM2_CCR3L      0x5316     // TIM2 cap/comp 3 low

#define	_ITC_SPR1	0x7f70
#define	_ITC_SPR2	0x7f71
#define	_ITC_SPR3	0x7f72
#define	_ITC_SPR4	0x7f73
#define	_ITC_SPR5	0x7f74
#define	_ITC_SPR6	0x7f75
#define	_ITC_SPR7	0x7f76
#define	_ITC_SPR8	0x7f77

