/*
 *  File name:  lib_lcd.c
 *  Date first: 12/19/2017
 *  Date last:  04/13/2018
 *
 *  Description: Library for Hitachi HD44780 LCDs on STM8 architecture.
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
 *  C4..C7	LCD D4..D7 (pins 11-14, fast)
 *  C3		LCD Enable (pin 6, fast)
 *  A3		LCD RS (pin 4) on STM8S_103 (fast pin)
 *  F4		LCD RS (pin 4) on STM8S_105 (slow pin)
 *  Vss		LCD ground (pin 1)
 *  Vdd		LCD +5V (pin 2)
 *  Vo		LCD contrast (pin 3)
 *  W*		LCD R/W wired low (pin 5)
 */

#include "stm8s_header.h"
#include "lib_lcd.h"

static void delay_500ns(void);
static void delay_usecs(char);
static void delay_50us(void);
static void delay_ms(unsigned char);

static void lcd_comd(char);
static void lcd_data(char);
static void lcd_nybble(char);

#ifdef STM8103
#define BUSY_COUNT	100	/* RC clock at 16mhz */
#define RS_PORT		_PA_ODR
#define RS_PIN		3
#endif
#ifdef STM8105
#define RS_PORT		_PF_ODR
#define RS_PIN		4
#warning "Pins C1 & C2 clobbered. Use as inputs."
#define BUSY_COUNT	50	/* crystal oscillator at 8 mhz */
#endif

char	busy_count = BUSY_COUNT;	/* 100 uSecs */

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
    val;
__asm
    bset	RS_PORT, #RS_PIN
    push	_busy_count

    ld		a, (4, sp)
    and		a, #0xf0
    ld		_PC_ODR, a
    bset	_PC_ODR, #3
    call	_delay_500ns
    bres	_PC_ODR, #3

    ld		a, (4, sp)
    swap	a
    and		a, #0xf0
    ld		_PC_ODR, a
    bset	_PC_ODR, #3
    call	_delay_500ns
    bres	_PC_ODR, #3

    call	_delay_usecs
    pop		a
__endasm;
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
    val;
__asm
    bres	RS_PORT, #RS_PIN
    ld		a, (3, sp)
    and		a, #0xf0
    ld		_PC_ODR, a
    call	_delay_500ns
    bset	_PC_ODR, #3
    call	_delay_500ns
    bres	_PC_ODR, #3
__endasm;
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
#ifdef STM8103
    PA_DDR |= 0x08;		/* A3 output */
    PA_CR1 |= 0x08;
    PA_CR2 |= 0x08;
#endif
#ifdef STM8105
    PF_DDR = 0x10;
    PF_CR1 = 0x10;
#endif
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
        ld	a, (3, sp)
	dec	a
	clrw	x
	ld	xl,a
	sllw	x
	sllw	x
00001$:
        decw	x		; (2)
	jrne	00001$		; (2)
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

