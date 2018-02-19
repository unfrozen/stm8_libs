/*
 *  File name:  lib_lcd.c
 *  Date first: 12/19/2017
 *  Date last:  01/19/2018
 *
 *  Description: Library for Hitachi LM016L LCD on STM8 architecture.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2017, 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Pinouts:
 *
 *  C4..C7	LCD D4..D7 (pins 11-14)
 *  C3		LCD RS (pin 4)
 *  A3		LCD Enable (pin 6)
 *  Vss		LCD ground (pin 1)
 *  Vdd		LCD +5V (pin 2)
 *  Vo		LCD contrast (pin 3)
 *  W*		LCD R/W wired low (pin 5)
 */

#include "stm8.h"
#include "lib_lcd.h"

static void delay_500ns(void);
static void delay_usecs(char);
static void delay_50us(void);
static void delay_ms(unsigned char);

static void lcd_comd(char);
static void lcd_data(char);
static void lcd_nybble(char);

/******************************************************************************
 *
 *  Set LCD cursor by line and column
 *
 *  in: line (0-3), column (0-19)
 */

void lcd_curs(char line, char col)
{
    char	val;

    val = col;
    if (line & 1)
	val += 0x40;		/* lines 1 & 3 have 0x40 offset */
    if (line & 2)
	val += 20;		/* lines 2 & 3 have 20 offset */
    lcd_comd(val | 0x80);
}

/******************************************************************************
 *
 *  Write data byte to lcd
 */

void lcd_put(char val)
{
    PC_ODR = val | 8;		/* RS=1 for data */
    delay_500ns();
    PA_ODR |= 0x08;
    delay_500ns();
    PA_ODR &= 0xf7;
    delay_500ns();

    PC_ODR = (val << 4) | 8;
    delay_500ns();
    PA_ODR |= 0x08;
    delay_500ns();
    PA_ODR &= 0xf7;
    delay_usecs(500);
}

void lcd_puts(char *s)
{
    while (1) {
	if (*s == 0)
	    return;
	lcd_put(*s);
	s++;
    }
}

/******************************************************************************
 *
 *  Write command byte to lcd
 */

void lcd_comd(char val)
{
    lcd_nybble(val);
    lcd_nybble(val << 4);
    delay_50us();
}

/******************************************************************************
 *
 *  Write command nybble to lcd
 */

void lcd_nybble(char val)
{
    PC_ODR = val & 0xf0;	/* RS=0 for command */
    delay_500ns();
    PA_ODR |= 8;
    delay_500ns();
    PA_ODR &= 0xf7;
    delay_500ns();
}

/******************************************************************************
 *
 *  Initialize LCD
 */

void lcd_init(void)
{
    PC_DDR |= 0xf8;		/* C3, C4-C7 outputs */
    PC_CR1 |= 0xf8;
    PC_CR2 |= 0xf8;		/* fast outputs */

    PA_DDR |= 0x08;		/* A3 output */
    PA_CR1 |= 0x08;
    PA_CR2 |= 0x08;

    delay_ms(50);
    lcd_nybble(0x30);		/* start with 8-bit mode */
    delay_ms(5);		/* more than 4.1 ms */
    lcd_nybble(0x30);
    delay_usecs(120);		/* more than 100 us */
    lcd_nybble(0x30);
    delay_usecs(50);		/* more than 37 us */
    lcd_nybble(0x20);		/* set to 4-bit mode */
    delay_usecs(50);

    lcd_comd(0x28);		/* 4-bit mode, 2 lines */
    lcd_comd(0x0e);		/* turn on diplay and cursor */
    lcd_comd(0x06);		/* auto increment and move cursor */
}

void delay_500ns(void)
{
    /* call (4) + return (4) == 500 uS */
}
void delay_50us(void)
{
    delay_usecs(50);
}
void delay_usecs(char usecs)
{
    usecs;
__asm
	dec	a
	clrw	x
	ld	xl,a
	sllw	x
	sllw	x
00001$:
	nop
	decw	x
	jrne	00001$
__endasm;
}

void delay_ms(unsigned char wait)
{
    while (wait--) {
	delay_usecs(250);
	delay_usecs(250);
	delay_usecs(250);
	delay_usecs(250);
    }
}

