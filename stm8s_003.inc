/*
 *  STM8 defines for inline assembly.
 *  Names are preceded with underscore.
 */

#define _OPT2           0x4803       // Alternate function remap
#define _NOPT2          0x4804 

#define _UNIQUE_ID	0x4865		// 12-byte unique id

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

#define _SPI_CR1	0x5200	// SPI control #1
#define _SPI_CR2	0x5201	// SPI control #2
#define _SPI_ICR	0x5202	// SPI interrupt control
#define _SPI_SR		0x5203	// SPI status
#define _SPI_DR		0x5204	// SPI data R/W
#define _SPI_CRCPR	0x5205	// SPI CRC polynomial
#define _SPI_RXCRCR	0x5206	// SPI RX CRC
#define _SPI_TXCRCR	0x5207	// SPI TX CRC

#define _TIM1_CR1	0x5250	// TIM1 Control 1
#define _TIM1_CR2	0x5251	// TIM1 Control 2
#define _TIM1_SMCR	0x5252	// TIM1 Slave mode control
#define _TIM1_ETR	0x5253	// TIM1 External trigger
#define _TIM1_IER	0x5254	// TIM1 Interrupt enable
#define _TIM1_SR1	0x5255	// TIM1 status 1
#define _TIM1_SR2	0x5256	// TIM1 status 2
#define _TIM1_EGR	0x5257	// TIM1 Event generation
#define _TIM1_CCMR1	0x5258	// TIM1 Capture/compare mode 1
#define _TIM1_CCMR2	0x5259	// TIM1 Capture/compare mode 2
#define _TIM1_CCMR3	0x525a	// TIM1 Capture/compare mode 3
#define _TIM1_CCMR4	0x525b	// TIM1 Capture/compare mode 4
#define _TIM1_CCER1	0x525c	// TIM1 Capture/compare enable 1
#define _TIM1_CCER2	0x525d	// TIM1 Capture/compare enable 2
#define _TIM1_CNTRH	0x525e	// TIM1 Counter high
#define _TIM1_CNTRL	0x525f	// TIM1 Counter low
#define _TIM1_PSCRH	0x5260	// TIM1 Prescale high
#define _TIM1_PSCRL	0x5261	// TIM1 Prescale low
#define _TIM1_ARRH	0x5262	// TIM1 Auto reload high
#define _TIM1_ARRL	0x5263	// TIM1 Auto reload low
#define _TIM1_RCR	0x5264	// TIM1 Repetition counter
#define _TIM1_CCR1H	0x5265	// TIM1 Capture/compare 1 high
#define _TIM1_CCR1L	0x5266	// TIM1 Capture/compare 1 low
#define _TIM1_CCR2H	0x5267	// TIM1 Capture/compare 2 high
#define _TIM1_CCR2L	0x5268	// TIM1 Capture/compare 2 low
#define _TIM1_CCR3H	0x5269	// TIM1 Capture/compare 3 high
#define _TIM1_CCR3L	0x526a	// TIM1 Capture/compare 3 low
#define _TIM1_CCR4H	0x526b	// TIM1 Capture/compare 4 high
#define _TIM1_CCR4L	0x526c	// TIM1 Capture/compare 4 low
#define _TIM1_BKR	0x526d	// TIM1 Break register
#define _TIM1_DTR	0x526e	// TIM1 Dead time register
#define _TIM1_OISR	0x526f	// TIM1 Output idle state

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

#define _ADC_DB0RH       0x53e0     // ADC data buffer 0 high
#define _ADC_DB0RL       0x53e1     // ADC data buffer 0 low 

#define _ADC_CSR         0x5400     // ADC control/status
#define _ADC_CR1         0x5401     // ADC config reg 1
#define _ADC_CR2         0x5402     // ADC config reg 2
#define _ADC_CR3         0x5403     // ADC config reg 3
#define _ADC_DRH         0x5404     // ADC data register high
#define _ADC_DRL         0x5405     // ADC data register low
#define _ADC_TDRH        0x5406     // ADC Schmitt trigger disable high
#define _ADC_TDRL        0x5407     // ADC Schmitt trigger disable low
#define _ADC_HTRH        0x5408     // ADC high threshold high
#define _ADC_HTRL        0x5409     // ADC high trheshold low
#define _ADC_LTRH        0x540a     // ADC low threshold high
#define _ADC_LTRL        0x540b     // ADC low threshold low
#define _ADC_AWSRH       0x540c     // ADC watchdog status high
#define _ADC_AWSRL       0x540d     // ADC watchdog status low 
#define _ADC_AWCRH       0x540e     // ADC watchdog control high
#define _ADC_AWCRL       0x540f     // ADC watchdog control low 

#define	_ITC_SPR1	0x7f70
#define	_ITC_SPR2	0x7f71
#define	_ITC_SPR3	0x7f72
#define	_ITC_SPR4	0x7f73
#define	_ITC_SPR5	0x7f74
#define	_ITC_SPR6	0x7f75
#define	_ITC_SPR7	0x7f76
#define	_ITC_SPR8	0x7f77

