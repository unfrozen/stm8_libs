/*
 *  File name:  lib_spi.c
 *  Date first: 05/29/2020
 *  Date last:  07/08/2020
 *
 *  Description: STM8 Library for hardware SPI.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2020 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 *  Location:
 *  https://github.com/unfrozen/stm8_libs
 *
 ******************************************************************************
 *
 *  Includes
 */

#include "stm8s_header.h"
#include "lib_spi.h"

/******************************************************************************
 *
 *  Current SPI context, data pointers, and counts
 */

static SPI_CTX *ctx_cur;

static int tx_count;
static int rx_count;
static int tx_dummy;		/* Dummy TX bytes needed for RX clock. */

static char *tx_buf;
static char *rx_buf;

static char rx_debug;		/* Incrementing value to debug RX */

/******************************************************************************
 *
 *  Interrupt handlers
 */

static void irq_idle(void);
static void irq_read(void);
static void irq_write(void);
static void irq_rw(void);

static void (*irq_current)(void);

/******************************************************************************
 *
 *  Initialize SPI
 *  in: SPI context
 */

void spi_init(SPI_CTX *ctx)
{
    PC_DDR |= 0x20;		/* C5 is clock, C6 is output (MOSI) */
    PC_CR1 |= 0x20;		/* Push-pull output */
    PC_CR2 |= 0x20;		/* High speed (10mhz) */

    /* We can't set C6 to output (yet) because bidirectional mode won't work.
     * So we can't set C6 to high speed, either. Measured rise and fall
     * times are around 50 nanoseconds on C6 and 25 on C5.
     */
    spi_config(ctx);
    irq_current = irq_idle;
    ctx->state = SPI_IDLE;
    ctx_cur = ctx;
}

/******************************************************************************
 *
 *  Start SPI transaction
 *  in:  SPI context
 *  out: status code
 */

char spi_start(SPI_CTX *ctx)
{
    /* Wait for final SPI write to finish. This is important when
     * an SPI write-only is followed by SPI read + write transaction.
     */
    spi_wait();
    SPI_DR;		/* If last transfer was TX only, discard RX value. */

    /* check for different SPI config */
    if (ctx_cur != ctx)
	spi_config(ctx);

    tx_count = ctx->tx_count;
    rx_count = ctx->rx_count;
    tx_buf = ctx->tx_buf;
    rx_buf = ctx->rx_buf;
    ctx->flag_done = 0;

    tx_dummy = rx_count - tx_count;	/* Negative is okay, not used. */
    
    if (tx_count && rx_count) {
	ctx->state = SPI_RW;
	irq_current = irq_rw;
	SPI_ICR = SPI_ICR_RXIE | SPI_ICR_TXIE;
	return SPI_OK;
    }
    if (tx_count) {
	ctx->state = SPI_WRITE;
	irq_current = irq_write;
	if (ctx->flag_bidir)
	    SPI_CR2 |= SPI_CR2_BDOE;
	SPI_ICR = SPI_ICR_TXIE;
	return SPI_OK;
    }
    if (rx_count) {
	ctx->state = SPI_READ;
	irq_current = irq_read;
	SPI_ICR = SPI_ICR_RXIE;
	if (ctx->flag_bidir) {	/* SPI clock starts when OE goes low */
	    SPI_CR2 &= ~SPI_CR2_BDOE;
	    tx_dummy = 0;	/* Don't send dummy TX bytes. */
	}
	else {
	    SPI_DR = rx_debug++;	/* Start clock by writing TX. */
	    tx_dummy--;
	}
	SPI_ICR = SPI_ICR_RXIE;
	return SPI_OK;
    }
    return SPI_ERR;
}

/******************************************************************************
 *
 *  SPI interrupt handler
 */

void spi_isr(void) __interrupt (IRQ_SPI) __naked
{
    // irq_current();
__asm
    bset	_PA_ODR, #2
    ldw		x, _irq_current
    call	(x)
    bres	_PA_ODR, #2
    iret
__endasm;
}

/******************************************************************************
 *
 *  Idle interrupt handler
 */

static void irq_idle(void)
{
    SPI_ICR = 0;		/* done, so disable interrupts */
}

/******************************************************************************
 *
 *  Reading interrupt handler
 */

static void irq_read(void)
{
    char	status;

    status = SPI_SR;
    if (status & SPI_SR_RXNE) {
	*rx_buf = SPI_DR;
	rx_buf++;
	rx_count--;
    }
    if ((status & SPI_SR_TXE) &&
	tx_dummy) {		/* Need to feed clock with TX byte? */
	tx_dummy--;
	SPI_DR = rx_debug++;
    }
    if (rx_count)
	return;
    
    SPI_ICR = 0;
    irq_current = irq_idle;
    ctx_cur->state = SPI_IDLE;
    ctx_cur->flag_done = 1;

    /* Turn off the SPI clock. */
    if (ctx_cur->flag_bidir)	/* Go back to pin output mode. */
	SPI_CR2 = SPI_CR2_BDM | SPI_CR2_BDOE;
}

/******************************************************************************
 *
 *  Writing interrupt handler
 */

static void irq_write(void)
{
    if (tx_count) {
	SPI_DR = *tx_buf++;
	tx_count--;
    }
    if (tx_count)
	return;
    SPI_ICR = 0;
    irq_current = irq_idle;
    ctx_cur->state = SPI_IDLE;
    ctx_cur->flag_done = 1;	/* Note that TX is still going now. */
}

/******************************************************************************
 *
 *  Read and Write interrupt handler
 */

static void irq_rw(void)
{
    char	status;

    status = SPI_SR;

    if (status & SPI_SR_TXE) {
	SPI_DR = *tx_buf++;
	tx_count--;
    }
    if (status & SPI_SR_RXNE) {
	*rx_buf = SPI_DR;
	rx_buf++;
	rx_count--;
    }
    if (tx_count && rx_count)
	return;
    if (tx_count) {
	SPI_ICR = SPI_ICR_TXIE;
	irq_current = irq_write;
	ctx_cur->state = SPI_WRITE;
	return;
    }
    if (rx_count) {
	SPI_ICR = SPI_ICR_RXIE | SPI_ICR_TXIE;
	irq_current = irq_read;
	ctx_cur->state = SPI_READ;
	return;
    }
    SPI_ICR = 0;
    ctx_cur->state = SPI_IDLE;
    ctx_cur->flag_done = 1;
}

/******************************************************************************
 *
 *  Configure SPI
 *  in: SPI context
 */

void spi_config(SPI_CTX *ctx)
{
    SPI_CR1 = 0;		/* Disable SPI. */
    SPI_CR1 = ctx->config | 4;	/* Load new config, choose master. */
    
    /* If bidirectional (both TX and RX on same pin), we need to start
     * in TX mode (output enable); in RX mode, the SPI clock will start. */
    if (ctx->flag_bidir) {
	PC_CR2 &= 0xbf;		/* Can't use high speed mode. */
	PC_DDR &= 0xbf;		/* Because we can't set it to output only. */
	SPI_CR2 = SPI_CR2_BDM | SPI_CR2_BDOE;
    }
    else {
	PC_DDR |= 0x40;		/* Set MOSI pin to output mode. */
	PC_CR2 |= 0x40;		/* And to high speed. */
	SPI_CR2 = 0;
    }
    SPI_CR1 |= SPI_CR1_SPE;	/* enable SPI */
    ctx_cur = ctx;
}

/******************************************************************************
 *
 *  Wait for previous SPI write to finish
 */

void spi_wait(void)
{
    while (tx_count | rx_count);

    /* Check and wait for previous data to be sent. Refer to RM0016,
     * 20.3.7 (page 277) and 20.3.8 (page 278) for details.
     */
    while (!(SPI_SR & SPI_SR_TXE)); /* wait for TXE=1 */
    while (SPI_SR & SPI_SR_BSY);    /* wait for BSY=0 */
}
