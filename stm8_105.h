/*
 * Header for STM8S 105
 */
#define STM8105

#define PTR(addr) *(volatile char *)(addr)

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

#define PG_ODR		PTR(0x501e)
#define PG_IDR		PTR(0x501f)
#define PG_DDR		PTR(0x5020)
#define PG_CR1		PTR(0x5021)
#define PG_CR2		PTR(0x5022)

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

#define BEEP_CSR	PTR(0x50f3)	// Beeper control

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


#define TIM4_CR1	PTR(0x5340)	// TIM4 control 1
#define TIM4_IER	PTR(0x5341)	// TIM4 interrupt enable
#define TIM4_SR		PTR(0x5342)	// TIM4 status
#define TIM4_EGR	PTR(0x5343)	// TIM4 event generation
#define TIM4_CNTR	PTR(0x5344)	// TIM4 counter
#define TIM4_PSCR	PTR(0x5345)	// TIM4 prescaler
#define TIM4_ARR	PTR(0x5346)	// TIM4 auto reload


