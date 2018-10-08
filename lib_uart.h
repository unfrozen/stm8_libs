/*
 *  File name:  lib_uart.h
 *  Date first: 12/30/2017
 *  Date last:  10/08/2018
 *
 *  Description: STM8 Library for UART1
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2017, 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Set RX and TX buffer sizes here
 */

#define UART_BUF_RX	16	/* must be power of 2 */
#define UART_BUF_TX	16	/* must be power of 2 */

/*
 *  UART init
 *  in: baud rate (see defines below)
 */
void uart_init(unsigned short);

/*
 *  Get number of bytes waiting in RX buf
 */
char uart_rsize(void);

/*
 *  Get next received byte (wait for it)
 */
char uart_get(void);

/*
 *  Send byte
 */
void uart_put(char);

/*
 *  Send string (ending with binary 00)
 */
void uart_puts(char *);

/*
 *  Send CR/LF (0x0d 0x0a)
 */
void uart_crlf(void);

/*
 *  Get hardware and buffer RX overruns
 */
short uart_over_hw(void);
short uart_over_buf(void);

/*
 *  Interrupt prototypes must be included with main()
 */

#include "vectors.h"

#ifdef STM8103		/* clock is 16mhz */

#define BAUD_1200	(0x4135)
#define BAUD_2400	(0xa01b)
#define BAUD_4800	(0xd005)
#define BAUD_9600	(0x6803)
#define BAUD_19200	(0x3401)
#define BAUD_38400	(0x1a01)
#define BAUD_57600	(0x1106)
#define BAUD_115200	(0x080b)

void uart_tx_isr(void) __interrupt (IRQ_UART_TX);
void uart_rx_isr(void) __interrupt (IRQ_UART_RX);

#define UART_SR		UART1_SR
#define UART_DR		UART1_DR
#define UART_BRR1	UART1_BRR1
#define UART_BRR2	UART1_BRR2
#define UART_CR1	UART1_CR1
#define UART_CR2	UART1_CR2
#define UART_CR3	UART1_CR3
#define UART_CR4	UART1_CR4
#define UART_CR5	UART1_CR5

#endif /* STM8103 */

#ifdef STM8105		/* clock is 8mhz crystal */

#define BAUD_1200	(0xa01b)
#define BAUD_2400	(0xd005)
#define BAUD_4800	(0x6803)
#define BAUD_9600	(0x3401)
#define BAUD_19200	(0x1a01)
#define BAUD_38400	(0x0d00)
#define BAUD_57600	(0x080b)
#define BAUD_115200	(0x0405)

void uart_tx_isr(void) __interrupt (IRQ_UART2_TX);
void uart_rx_isr(void) __interrupt (IRQ_UART2_RX);

#define UART_SR		UART2_SR
#define UART_DR		UART2_DR
#define UART_BRR1	UART2_BRR1
#define UART_BRR2	UART2_BRR2
#define UART_CR1	UART2_CR1
#define UART_CR2	UART2_CR2
#define UART_CR3	UART2_CR3
#define UART_CR4	UART2_CR4
#define UART_CR5	UART2_CR5

#endif /* STM8105 */
