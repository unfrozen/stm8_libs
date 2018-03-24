/*
 *  File name:  lib_uart.h
 *  Date first: 12/30/2017
 *  Date last:  03/23/2018
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
 *  UART init
 *  in: baud rate
 */
void uart_init(unsigned short);

#define BAUD_1200	(0x4135)
#define BAUD_2400	(0xa01b)
#define BAUD_4800	(0xd005)
#define BAUD_9600	(0x6803)
#define BAUD_19200	(0x3401)
#define BAUD_38400	(0x1a01)
#define BAUD_57600	(0x1106)
#define BAUD_115200	(0x080b)

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

void uart_tx_isr(void) __interrupt (IRQ_UART_TX);
void uart_rx_isr(void) __interrupt (IRQ_UART_RX);
