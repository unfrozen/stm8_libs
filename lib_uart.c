/*
 *  File name:  lib_uart.c
 *  Date first: 12/30/2017
 *  Date last:  03/23/2018
 *
 *  Description: STM8 Library for UART1
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2017 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 */
#include "stm8.h"
#include "vectors.h"

/*
 *  Buffers and counters
 */

static char uart_rxbuf[16];
static char uart_txbuf[16];

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

    UART1_BRR2 = baud & 0xff;	/* write BRR2 first */
    UART1_BRR1 = baud >> 8;
    UART1_CR1  = 0;	/* 8 bits, no parity */
    UART1_CR2  = 0x2c;	/* enable RX, TX, RX interrupts only */
    UART1_CR3  = 0;	/* one stop bit, no synchronous */
    UART1_CR4  = 0;	/* not using LIN */
    UART1_CR5  = 0;	/* no smartcard, no IRDA */
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
	size += 16;
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
    rx_get &= 0x0f;

    return byte;
}

/******************************************************************************
 *
 *  Send byte
 */
void uart_put(char byte)
{
    char	new_ptr;

    new_ptr = (tx_put + 1) & 0x0f;
    while (tx_get == new_ptr);
    uart_txbuf[tx_put] = byte;
    tx_put = new_ptr;
    UART1_CR2 |= SR_TXE;
}

/******************************************************************************
 *
 *  Send string
 *  in: character string, terminated with binary 00
 */
void uart_puts(char *str)
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
	UART1_CR2 &= ~SR_TXE;
	return;
    }
    UART1_DR = uart_txbuf[tx_get];
    tx_get = (tx_get + 1) & 0x0f;
    if (tx_get == tx_put)
	UART1_CR2 &= ~SR_TXE;
}

/******************************************************************************
 *
 *  RX interrupt
 */
void uart_rx_isr(void) __interrupt (IRQ_UART_RX)
{
    char	rxbyte, new_ptr;

    if (UART1_SR & SR_OR)
	rx_overruns++;
    rxbyte = UART1_DR;
    uart_rxbuf[rx_put] = rxbyte;

    new_ptr = (rx_put + 1) & 0x0f;
    if (new_ptr != rx_get)
	rx_put = new_ptr;
    else
	buf_overruns++;
}
