/*
 * Header for STM8S 103
 */
#define STM8103

#define PTR(addr) *(volatile char *)(addr)

#define EEPROM		PTR(0x4000)
#define EEPROM_SIZE	640

#define UNIQUE_ID	PTR(0x4865)	// 12-byte unique id

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

#define FLASH_CR1	PTR(0x505a)	// Flash control 1
#define FLASH_CR2	PTR(0x505b)	// Flash control 2
#define FLASH_NCR2	PTR(0x505c)	// Flash control 2 complement
#define FLASH_FPR	PTR(0x505d)	// Flash protection
#define FLASH_NFPR	PTR(0x505e)	// Flash protection complement
#define FLASH_IAPSR	PTR(0x505f)	// Flash control 1
#define FLASH_PUKR	PTR(0x5062)	// Flash in-application program status
#define FLASH_DUKR	PTR(0x5064)	// Data EEPROM unprotect

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

#define UART1_SR	PTR(0x5230)	// UART2 status
#define UART1_DR	PTR(0x5231)	// UART2 data
#define UART1_BRR1	PTR(0x5232)	// UART2 baud rate #1
#define UART1_BRR2	PTR(0x5233)	// UART2 baud rate #2
#define UART1_CR1	PTR(0x5234)	// UART2 control #1
#define UART1_CR2	PTR(0x5235)	// UART2 control #2
#define UART1_CR3	PTR(0x5236)	// UART2 control #3
#define UART1_CR4	PTR(0x5237)	// UART2 control #4
#define UART1_CR5	PTR(0x5238)	// UART2 control #5
#define UART1_GTR	PTR(0x5239)	// UART2 guard time
#define UART1_PSCR	PTR(0x523a)	// UART2 prescaler

#define TIM2_CR1	PTR(0x5300)	// TIM2 Control 1
#define TIM2_IER	PTR(0x5303)	// TIM2 Interrupt enable
#define TIM2_SR1	PTR(0x5304)	// TIM2 Status 1
#define TIM2_SR2	PTR(0x5305)	// TIM2 Status 2
#define TIM2_EGR	PTR(0x5306)	// TIM2 Event generation
#define TIM2_CCMR1	PTR(0x5307)	// TIM2 cap/comp mode 1
#define TIM2_CCMR2	PTR(0x5308)	// TIM2 cap/comp mode 2
#define TIM2_CCMR3	PTR(0x5309)	// TIM2 cap/comp mode 3
#define TIM2_CCER1	PTR(0x530a)	// TIM2 cap/comp enable 1
#define TIM2_CCER2	PTR(0x530b)	// TIM2 cap/comp enable 2
#define TIM2_CNTRH	PTR(0x530c)	// TIM2 counter high
#define TIM2_CNTRL	PTR(0x530d)	// TIM2 counter low
#define TIM2_PSCR	PTR(0x530e)	// TIM2 prescaler
#define TIM2_ARRH	PTR(0x530f)	// TIM2 auto-reload high
#define TIM2_ARRL	PTR(0x5310)	// TIM2 auto-reload low
#define TIM2_CCR1H	PTR(0x5311)	// TIM2 cap/comp 1 high
#define TIM2_CCR1L	PTR(0x5312)	// TIM2 cap/comp 1 low
#define TIM2_CCR2H	PTR(0x5313)	// TIM2 cap/comp 2 high
#define TIM2_CCR2L	PTR(0x5314)	// TIM2 cap/comp 2 low
#define TIM2_CCR3H	PTR(0x5315)	// TIM2 cap/comp 3 high
#define TIM2_CCR3L	PTR(0x5316)	// TIM2 cap/comp 3 low

#define TIM4_CR1	PTR(0x5340)	// TIM4 control 1
#define TIM4_IER	PTR(0x5343)	// TIM4 interrupt enable
#define TIM4_SR		PTR(0x5344)	// TIM4 status
#define TIM4_EGR	PTR(0x5345)	// TIM4 event generation
#define TIM4_CNTR	PTR(0x5346)	// TIM4 counter
#define TIM4_PSCR	PTR(0x5347)	// TIM4 prescaler
#define TIM4_ARR	PTR(0x5348)	// TIM4 auto reload


