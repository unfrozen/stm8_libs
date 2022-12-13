/*
 *  File name:  lib_uart.c
 *  Date first: 12/30/2017
 *  Date last:  12/13/2022
 *
 *  Description: STM8 Library for UART1 and UART2
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2017, 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 */
#include "stm8s_header.h"

#include "lib_uart.h"

/*
 *  Buffers and counters
 */

static char uart_rxbuf[UART_BUF_RX];
static char uart_txbuf[UART_BUF_TX];

static volatile char tx_get, tx_put;
static volatile char rx_get, rx_put;

static short	rx_overruns;
static short	buf_overruns;

/******************************************************************************
 *
 *  UART init
 *  in: baud rate
 */
void uart_init(unsigned short baud)
{
    rx_get = 0;
    rx_put = 0;
    tx_get = 0;
    tx_put = 0;
    rx_overruns = 0;
    buf_overruns = 0;

    PD_DDR |= 0x20;		/* D5 is TX */
    PD_DDR &= 0xbf;		/* D6 is RX */

    UART_BRR2 = baud & 0xff;	/* write BRR2 first */
    UART_BRR1 = baud >> 8;
    UART_CR1  = 0;	/* 8 bits, no parity */
    UART_CR2  = 0x2c;	/* enable RX, TX, RX interrupts only */
    UART_CR3  = 0;	/* one stop bit, no synchronous */
    UART_CR4  = 0;	/* not using LIN */
    UART_CR5  = 0;	/* no smartcard, no IRDA */
}

/******************************************************************************
 *
 *  Get number of bytes waiting in RX buf
 */

char uart_rsize(void)
{
    signed char	size;

    size = rx_put - rx_get;
    if (size < 0)
	size += UART_BUF_RX;
    return size;
}

/******************************************************************************
 *
 *  Get next received byte (wait for it)
 */
char uart_get(void)
{
    char	byte;

    while (rx_get == rx_put);

    byte = uart_rxbuf[rx_get];
    rx_get++;
    rx_get &= (UART_BUF_RX - 1);

    return byte;
}

/******************************************************************************
 *
 *  Send byte
 */
void uart_put(char byte)
{
    char	new_ptr;

    new_ptr = (tx_put + 1) & (UART_BUF_TX - 1);
    while (tx_get == new_ptr);
    uart_txbuf[tx_put] = byte;
    tx_put = new_ptr;
    UART_CR2 |= SR_TXE;
}

/******************************************************************************
 *
 *  Send string
 *  in: character string, terminated with binary 00
 */
void uart_puts(const char *str)
{
    while (*str)
	uart_put(*str++);
}

/******************************************************************************
 *
 *  Send CR/LF
 */
void uart_crlf(void)
{
    uart_put(0x0d);
    uart_put(0x0a);
}

/******************************************************************************
 *
 *  Get hardware and buffer RX overruns
 */
short uart_over_hw(void)
{
    return rx_overruns;
}
short uart_over_buf(void)
{
    return buf_overruns;
}

/******************************************************************************
 *
 *  TX interrupt
 */
void uart_tx_isr(void) __interrupt (IRQ_UART_TX)
{
    if (tx_get == tx_put) {	/* empty buffer? */
	UART_CR2 &= ~SR_TXE;
	return;
    }
    UART_DR = uart_txbuf[tx_get];
    tx_get = (tx_get + 1) & (UART_BUF_TX - 1);
    if (tx_get == tx_put)
	UART_CR2 &= ~SR_TXE;
}

/******************************************************************************
 *
 *  RX interrupt
 */
void uart_rx_isr(void) __interrupt (IRQ_UART_RX)
{
    char	rxbyte, new_ptr;

    if (UART_SR & SR_OR)
	rx_overruns++;
    rxbyte = UART_DR;
    uart_rxbuf[rx_put] = rxbyte;

    new_ptr = (rx_put + 1) & (UART_BUF_RX - 1);
    if (new_ptr != rx_get)
	rx_put = new_ptr;
    else
	buf_overruns++;
}
