/*
 * Header for STM8S 105
 */
#ifndef STM8105
#define STM8105
#endif

#define PTR(addr) *(volatile char *)(addr)

#define EEPROM		PTR(0x4000)
#define EEPROM_SIZE	1024

#define FLASH_BASE	PTR(0x8000)
#define FLASH_SIZE	16834
#define FLASH_BLOCK	128

#define OPT2		PTR(0x4803)	// Alternate function remap
#define NOPT2		PTR(0x4804)

#define UNIQUE_ID	PTR(0x48cd)	// 12-byte unique id

#define PA_ODR		PTR(0x5000)	// Port A output latch
#define PA_IDR		PTR(0x5001)	// Port A input status
#define PA_DDR		PTR(0x5002)	// Port A data direction
#define PA_CR1		PTR(0x5003)	// Port A open drain or pull-ups
#define PA_CR2		PTR(0x5004)	// Port A output speed or interrupt

#define PB_ODR		PTR(0x5005)
#define PB_IDR		PTR(0x5006)
#define PB_DDR		PTR(0x5007)
#define PB_CR1		PTR(0x5008)
#define PB_CR2		PTR(0x5009)

#define PC_ODR		PTR(0x500a)
#define PC_IDR		PTR(0x500b)
#define PC_DDR		PTR(0x500c)
#define PC_CR1		PTR(0x500d)
#define PC_CR2		PTR(0x500e)

#define PD_ODR		PTR(0x500f)
#define PD_IDR		PTR(0x5010)
#define PD_DDR		PTR(0x5011)
#define PD_CR1		PTR(0x5012)
#define PD_CR2		PTR(0x5013)

#define PE_ODR		PTR(0x5014)
#define PE_IDR		PTR(0x5015)
#define PE_DDR		PTR(0x5016)
#define PE_CR1		PTR(0x5017)
#define PE_CR2		PTR(0x5018)

#define PF_ODR		PTR(0x5019)
#define PF_IDR		PTR(0x501a)
#define PF_DDR		PTR(0x501b)
#define PF_CR1		PTR(0x501c)
#define PF_CR2		PTR(0x501d)

#define PG_ODR		PTR(0x501e)
#define PG_IDR		PTR(0x501f)
#define PG_DDR		PTR(0x5020)
#define PG_CR1		PTR(0x5021)
#define PG_CR2		PTR(0x5022)

#define FLASH_CR1	PTR(0x505a)     // Flash control 1
#define FLASH_CR2	PTR(0x505b)     // Flash control 2
#define FLASH_NCR2	PTR(0x505c)     // Flash control 2 complement
#define FLASH_FPR	PTR(0x505d)     // Flash protection
#define FLASH_NFPR	PTR(0x505e)     // Flash protection complement
#define FLASH_IAPSR	PTR(0x505f)     // Flash in-application program status
#define FLASH_PUKR	PTR(0x5062)     // Flash unprotect
#define FLASH_DUKR	PTR(0x5064)     // Data EEPROM unprotect

#define EXTI_CR1	PTR(0x50a0)	// External interrupt control
#define EXTI_CR2	PTR(0x50a1)

#define CLK_ICKR	PTR(0x50c0)	// Internal clock control
#define CLK_ECKR	PTR(0x50c1)	// External clock control
#define CLK_CMSR	PTR(0x50c3)	// Clock master status
#define CLK_SWR		PTR(0x50c4)	// Clock master switch
#define CLK_SWCR	PTR(0x50c5)	// Clock switch control
#define CLK_CKDIVR	PTR(0x50c6)	// Clock divider
#define CLK_PCKENR1	PTR(0x50c7)	// Peripheral clock gating #1
#define CLK_CSSR	PTR(0x50c8)	// Clock security system
#define CLK_CCOR	PTR(0x50c9)	// Configurable clock control
#define CLK_PCKENR2	PTR(0x50ca)	// Peripheral clock gating #2
#define CLK_HSITRIMR	PTR(0x50cc)	// HSI clock calibration trimming
#define CLK_SWIMCCR	PTR(0x50cd)	// SWIM clock control

#define BEEP_CSR	PTR(0x50f3)	// BEEP control

#define UART2_SR	PTR(0x5240)	// UART2 status
#define UART2_DR	PTR(0x5241)	// UART2 data
#define UART2_BRR1	PTR(0x5242)	// UART2 baud rate #1
#define UART2_BRR2	PTR(0x5243)	// UART2 baud rate #2
#define UART2_CR1	PTR(0x5244)	// UART2 control #1
#define UART2_CR2	PTR(0x5245)	// UART2 control #2
#define UART2_CR3	PTR(0x5246)	// UART2 control #3
#define UART2_CR4	PTR(0x5247)	// UART2 control #4
#define UART2_CR5	PTR(0x5248)	// UART2 control #5
#define UART2_CR6	PTR(0x5249)	// UART2 control #6
#define UART2_GTR	PTR(0x524a)	// UART2 guard time
#define UART2_PSCR	PTR(0x524b)	// UART2 prescaler

#define SR_OR		(1 << 3)
#define SR_TXE		(1 << 7)

#define TIM1_CR1	PTR(0x5250)	// TIM1 Control 1
#define TIM1_CR2	PTR(0x5251)	// TIM1 Control 2
#define TIM1_SMCR	PTR(0x5252)	// TIM1 Slave mode control
#define TIM1_ETR	PTR(0x5253)	// TIM1 External trigger
#define TIM1_IER	PTR(0x5254)	// TIM1 Interrupt enable
#define TIM1_SR1	PTR(0x5255)	// TIM1 status 1
#define TIM1_SR2	PTR(0x5256)	// TIM1 status 2
#define TIM1_EGR	PTR(0x5257)	// TIM1 Event generation
#define TIM1_CCMR1	PTR(0x5258)	// TIM1 Capture/compare mode 1
#define TIM1_CCMR2	PTR(0x5259)	// TIM1 Capture/compare mode 2
#define TIM1_CCMR3	PTR(0x525a)	// TIM1 Capture/compare mode 3
#define TIM1_CCMR4	PTR(0x525b)	// TIM1 Capture/compare mode 4
#define TIM1_CCER1	PTR(0x525c)	// TIM1 Capture/compare enable 1
#define TIM1_CCER2	PTR(0x525d)	// TIM1 Capture/compare enable 2
#define TIM1_CNTRH	PTR(0x525e)	// TIM1 Counter high
#define TIM1_CNTRL	PTR(0x525f)	// TIM1 Counter low
#define TIM1_PSCRH	PTR(0x5260)	// TIM1 Prescale high
#define TIM1_PSCRL	PTR(0x5261)	// TIM1 Prescale low
#define TIM1_ARRH	PTR(0x5262)	// TIM1 Auto reload high
#define TIM1_ARRL	PTR(0x5263)	// TIM1 Auto reload low
#define TIM1_RCR	PTR(0x5264)	// TIM1 Repetition counter
#define TIM1_CCR1H	PTR(0x5265)	// TIM1 Capture/compare 1 high
#define TIM1_CCR1L	PTR(0x5266)	// TIM1 Capture/compare 1 low
#define TIM1_CCR2H	PTR(0x5267)	// TIM1 Capture/compare 2 high
#define TIM1_CCR2L	PTR(0x5268)	// TIM1 Capture/compare 2 low
#define TIM1_CCR3H	PTR(0x5269)	// TIM1 Capture/compare 3 high
#define TIM1_CCR3L	PTR(0x526a)	// TIM1 Capture/compare 3 low
#define TIM1_CCR4H	PTR(0x526b)	// TIM1 Capture/compare 4 high
#define TIM1_CCR4L	PTR(0x526c)	// TIM1 Capture/compare 4 low
#define TIM1_BKR	PTR(0x526d)	// TIM1 Break register
#define TIM1_DTR	PTR(0x526e)	// TIM1 Dead time register
#define TIM1_OISR	PTR(0x526f)	// TIM1 Output idle state

#define TIM2_CR1        PTR(0x5300)     // TIM2 Control 1
#define TIM2_IER        PTR(0x5301)     // TIM2 Interrupt enable
#define TIM2_SR1        PTR(0x5302)     // TIM2 Status 1
#define TIM2_SR2        PTR(0x5303)     // TIM2 Status 2
#define TIM2_EGR        PTR(0x5304)     // TIM2 Event generation
#define TIM2_CCMR1      PTR(0x5305)     // TIM2 cap/comp mode 1
#define TIM2_CCMR2      PTR(0x5306)     // TIM2 cap/comp mode 2
#define TIM2_CCMR3      PTR(0x5307)     // TIM2 cap/comp mode 3
#define TIM2_CCER1      PTR(0x5308)     // TIM2 cap/comp enable 1
#define TIM2_CCER2      PTR(0x5309)     // TIM2 cap/comp enable 2
#define TIM2_CNTRH      PTR(0x530a)     // TIM2 counter high
#define TIM2_CNTRL      PTR(0x530b)     // TIM2 counter low
#define TIM2_PSCR       PTR(0x530c)     // TIM2 prescaler
#define TIM2_ARRH       PTR(0x530d)     // TIM2 auto-reload high
#define TIM2_ARRL       PTR(0x530e)     // TIM2 auto-reload low
#define TIM2_CCR1H      PTR(0x530f)     // TIM2 cap/comp 1 high
#define TIM2_CCR1L      PTR(0x5310)     // TIM2 cap/comp 1 low
#define TIM2_CCR2H      PTR(0x5311)     // TIM2 cap/comp 2 high
#define TIM2_CCR2L      PTR(0x5312)     // TIM2 cap/comp 2 low
#define TIM2_CCR3H      PTR(0x5313)     // TIM2 cap/comp 3 high
#define TIM2_CCR3L      PTR(0x5314)     // TIM2 cap/comp 3 low

#define TIMx_CEN	(1 << 0)	// counter enable
#define TIMx_UIE	(1 << 0)	// update interrupt
#define TIMx_CC1IE	(1 << 1)	// capture/compare 1 interrupt
#define TIMx_CC2IE	(1 << 2)	// capture/compare 2 interrupt
#define TIMx_CC3IE	(1 << 3)	// capture/compare 3 interrupt
#define TIMx_CC1E	(1 << 0)	// capture 1 enable
#define TIMx_CC1G	(1 << 1)	// capture/compare 1 generation
#define TIMx_CC1P	(1 << 1)	// capture/compare 1 polarity

#define TIM4_CR1	PTR(0x5340)	// TIM4 control 1
#define TIM4_IER	PTR(0x5341)	// TIM4 interrupt enable
#define TIM4_SR		PTR(0x5342)	// TIM4 status
#define TIM4_EGR	PTR(0x5343)	// TIM4 event generation
#define TIM4_CNTR	PTR(0x5344)	// TIM4 counter
#define TIM4_PSCR	PTR(0x5345)	// TIM4 prescaler
#define TIM4_ARR	PTR(0x5346)	// TIM4 auto reload

#define ADC_DB0RH	PTR(0x53e0)	// ADC data buffer 0 high
#define ADC_DB0RL	PTR(0x53e1)	// ADC data buffer 0 low

#define ADC_CSR		PTR(0x5400)	// ADC control/status
#define ADC_CR1		PTR(0x5401)	// ADC config reg 1
#define ADC_CR2		PTR(0x5402)	// ADC config reg 2
#define ADC_CR3		PTR(0x5403)	// ADC config reg 3
#define ADC_DRH		PTR(0x5404)	// ADC data register high
#define ADC_DRL		PTR(0x5405)	// ADC data register low
#define ADC_TDRH	PTR(0x5406)	// ADC Schmitt trigger disable high
#define ADC_TDRL	PTR(0x5407)	// ADC Schmitt trigger disable low
#define ADC_HTRH	PTR(0x5408)	// ADC high threshold high
#define ADC_HTRL	PTR(0x5409)	// ADC high trheshold low
#define ADC_LTRH	PTR(0x540a)	// ADC low threshold high
#define ADC_LTRL	PTR(0x540b)	// ADC low threshold low
#define ADC_AWSRH	PTR(0x540c)	// ADC watchdog status high 
#define ADC_AWSRL	PTR(0x540d)	// ADC watchdog status low
#define ADC_AWCRH	PTR(0x540e)	// ADC watchdog control high
#define ADC_AWCRL	PTR(0x540f)	// ADC watchdog control low

