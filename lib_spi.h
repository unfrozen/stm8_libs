/*
 *  File name:  lib_spi.h
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
 *  SPI context
 */

typedef struct {
    int		tx_count;	/* TX bytes to send */
    int		rx_count;	/* RX bytes to get */
    char	*tx_buf;	/* TX buffer, ignored if tx_count is zero  */
    char	*rx_buf;	/* RX buffer, ignored if rx_count is zero  */
    char	config;		/* see config values below */
    char	state;		/* IDLE, READ, WRITE, or RW */
    char	flag_bidir;	/* set if one pin (MOSI) is both RX & TX */
    char	flag_done;	/* set when SPI transaction is almost done */
} SPI_CTX;

/*
 * Note that flag_done is set when the read is complete, or the write buffer
 * is empty. When writing, the SPI will still be sending the last bits.
 * If you need to know that the SPI is inactive, use spi_wait() to be sure.
 */

/*
 *  Initialize SPI
 */
void spi_init(SPI_CTX *);

/*
 *  Start SPI transaction
 *  in:  SPI context
 *  out: status code
 *
 *  NOTE: This function will wait (block) if there is current transaction.
 *  NOTE: If bidirectional I/O, do not combine RX and TX in transaction.
 */
char spi_start(SPI_CTX *);

/*
 *  Wait for previous SPI transaction to finish.
 */
void spi_wait(void);

/*
 *  Reconfigure SPI for changed context.
 *
 *  The spi_start() function will do this if needed, but if you have an
 *  enable pin for the SPI device, you may need to change the configuration
 *  before you assert the enable pin. A glitch in the SPI clock and/or MOSI
 *  pin may upset the device.
 */

void spi_config(SPI_CTX *);

/******************************************************************************
 *
 *  SPI status codes
 */

#define SPI_OK		0
#define SPI_ERR		1
#define SPI_DONE	2

/******************************************************************************
 *
 *  SPI configuration values.
 *  Logical OR them together
 */

/* SPI bit endian */

#define SPI_MSB_FIRST	0x00
#define SPI_LSB_FIRST	0x80
/*
 * SPI CLOCK SPEED
 */
#define SPI_8MHZ	0x00
#define SPI_4MHZ	0x08
#define SPI_2MHZ	0x10
#define SPI_1MHZ	0x18
#define SPI_500K	0x20
#define SPI_250K	0x28
#define SPI_125K	0x30
#define SPI_62K		0x38

/* SPI level when idle */

#define SPI_IDLE_0	0x00	/* Clock is low when idle. */
#define SPI_IDLE_1	0x02	/* Clock is high when idle. */

/* SPI "polarity", data on clock edge 1 or 2? */

#define SPI_EDGE_1	0x00	/* Data is valid on 1st clock edge from idle. */
#define SPI_EDGE_2	0x01	/* Data is valid on 2nd clock edge from idle. */

/*
 *  SPI states
 */
#define SPI_IDLE	0
#define SPI_READ	1
#define SPI_WRITE	2
#define SPI_RW		3

/*
 *  Interrupt service routine declaration
 */
void spi_isr(void) __interrupt (IRQ_SPI);

