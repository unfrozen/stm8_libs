
/*
 *  Interrupt numbers and vector addresses
 */

#define	VECTT_RESET	0	/* 0x8000 Reset vector */
#define VECT_TRAP	0	/* 0x8004 Software interrupt */
#define IRQ_TLI		0	/* 0x8008 External top level interrupt */
#define IRQ_AWU		1	/* 0x800c Auto wake up from halt */
#define IRQ_CLK		2	/* 0x8010 Clock controller */
#define IRQ_EXTI0	3	/* 0x8014 Port A external interrupts */
#define IRQ_EXTI1	4	/* 0x8018 PORT B external interrupts */
#define IRQ_EXTI2	5	/* 0x801c PORT C external interrupts */
#define IRQ_EXTI3	6	/* 0x8020 PORT D external interrupts */
#define IRQ_EXTI4	7	/* 0x8024 PORT E external interrupts */
#define IRQ_res8	8	/* 0x8028 reserved */
#define IRQ_res9	9	/* 0x802c reserved */
#define IRQ_SPI		10	/* 0x8030 End of transfer */
#define IRQ_TIM1	11	/* 0x8034 TIM1 update/over/underflow/trigger */
#define IRQ_TIM1C	12	/* 0x8038 TIM1 capture/compare */
#define IRQ_TIM2	13	/* 0x803c TIM2 update/overflow */
#define IRQ_TIM2C	14	/* 0x8040 TIM2 capture/compare */
#define IRQ_TIM3	15	/* 0x8044 TIM3 update/overflow */
#define IRQ_TIM3C	16	/* 0x8048 TIM3 capture/compare */
#define IRQ_UART_TX	17	/* 0x804c UART TX complete */
#define IRQ_UART_RX	18	/* 0x8050 UART RX data full */
#define IRQ_I2C		19	/* 0x8054 I2C interrupt */
#define IRQ_UART2_TX	20	/* 0x8058 UART2 TX complete */
#define IRQ_UART2_RX	21	/* 0x805c UART2 RX data full */
#define IRQ_ADC1	22	/* 0x8060 ADC1 end of conversion/ analog WD */
#define IRQ_TIM4	23	/* 0x8064 TIM4 update/overflow */
#define IRQ_FLASH	24	/* 0x8068 EOP / WR_PG_DIS */

