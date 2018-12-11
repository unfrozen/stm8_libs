/*
 *  File name:  lib_ping.c
 *  Date first: 11/04/2018
 *  Date last:  12/04/2018
 *
 *  Description: STM8 Library for HC-SR04 ultrasonic range finder.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 */

#include "stm8s_header.h"
#include "lib_ping.h"

typedef struct {
    IO_CALL_INT	*cfg;		/* output pin and callback function */
    char	*count;		/* high latch count */
    char	*pol_addr;	/* register with polarity bit */
    char	 pol_mask;	/* polarity bit */
    int		 start;		/* pulse start */
    char	 ping_hi;	/* high 8 bits of start */
} PING_DATA;

static PING_DATA ping_data[3];

static char active;		/* channel active flags */

static char clock_hi;		/* make 24 bits for timer2 */

static PING_DATA *get_data(char);
static void do_trigger(volatile char *, char);

#pragma disable_warning 59

/******************************************************************************
 *
 *  Initialize
 *  in: Timer2 channel(s), list of pin/callback config
 */

void ping_init(char chans, IO_CALL_INT *cfg)
{
    clock_hi = 0;
    active = 0;

#ifdef PING_USE_C1
    if (chans & PING_CHAN1) {
	ping_data[0].cfg = cfg;
	ping_data[0].count = &TIM2_CCR1H;
	ping_data[0].pol_addr = &TIM2_CCER1;
	ping_data[0].pol_mask = 0x02;
	cfg++;
__asm
    bset	_active, #1
    bset	_TIM2_IER, #1
    bset	_TIM2_EGR, #1
    mov		_TIM2_CCMR1, #0x31
    bset	_TIM2_CCER1, #0
__endasm;
    }
#endif
#ifdef PING_USE_C2
    if (chans & PING_CHAN2) {
	ping_data[1].cfg = cfg;
	ping_data[1].count = &TIM2_CCR2H;
	ping_data[1].pol_addr = &TIM2_CCER1;
	ping_data[1].pol_mask = 0x20;
	cfg++;
__asm
    bset	_active, #2
    bset	_TIM2_IER, #2
    bset	_TIM2_EGR, #2
    mov		_TIM2_CCMR2, #0x31
    bset	_TIM2_CCER1, #4
__endasm;
    }
#endif
#ifdef PING_USE_C3
    if (chans & PING_CHAN3) {
	ping_data[2].cfg = cfg;
	ping_data[2].count = &TIM2_CCR3H;
	ping_data[2].pol_addr = &TIM2_CCER2;
	ping_data[2].pol_mask = 0x02;
__asm
    bset	_active, #3
    bset	_TIM2_IER, #3
    bset	_TIM2_EGR, #3
    mov		_TIM2_CCMR3, #0x31
    bset	_TIM2_CCER2, #0
__endasm;
    }
#endif
__asm
    bset	_TIM2_IER, #0	/* interrupt on clock */
__endasm;
#ifdef STM8103
    TIM2_PSCR = 4;		/* prescale = 16, 1 uSec per count */
#endif
#ifdef STM8105
    TIM2_PSCR = 3;		/* prescale = 8 */
#endif
    TIM2_CR1 = 1;		/* counter enable */
}

/******************************************************************************
 *
 *  Get ping data block from channel
 *  in: Channel
 * out: data block
 */

static PING_DATA *get_data(char chan)
{
    if (chan == PING_CHAN1)
	return (ping_data + 0);
    if (chan == PING_CHAN2)
	return (ping_data + 1);
    return (ping_data + 2);
}

/******************************************************************************
 *
 *  Start ping process (ping may take up to 20ms to complete)
 *  in:  Channel (PING_CHAN1/2/3)
 */

void ping_send(char chan)
{
    PING_DATA *data;
    IO_PIN *pin;

    data = get_data(chan);
    pin  = data->cfg->pin;

    /* set polarity to capture & interrupt on rising edge */
    *(data->pol_addr) &= ~(data->pol_mask);

    do_trigger(pin->reg_base, pin->reg_mask);
}

static void do_trigger(volatile char *base, char mask)
{
    base, mask;
__asm
    ldw		x, (3, sp)
    ld		a, (5, sp)
    or		a, (x)
    ld		(x), a

    push	#10
    call	_delay_usecs
    pop		a

    ldw		x, (3, sp)
    ld		a, (5, sp)
    xor		a, (x)
    ld		(x), a
__endasm;
}

/******************************************************************************
 *
 *  Channel 1/2/3 interrupt
 *
 *  If rising edge, the ping is starting. Save the current time.
 *  If falling edge, the ping has returned. Subtract the start time to
 *  get the round trip time and call the callback function to give it
 *  the information.
 *
 *  in: X has channel data structure (non-standard call)
 */

void isr_chan(PING_DATA *data)
{
    data;
__asm
    ldw		y, x		; channel data structure
    ldw		x, (7, x)	; get pulse start
    pushw	x

    ldw		x, y
    ldw		x, (2, x)	; latch count pointer
    ld		a, (x)		; high byte
    push	a		; pushing out of order
    ld		a, (1, x)
    push	a

    ld		a, (6, y)	; polarity bit
    ldw		x, y
    ldw		x, (4, x)	; register with polarity
    xor		a, (x)		; invert polarity
    ld		(x), a

    popw	x
    swapw	x		; high byte was pushed first

    and		a, (6, y)	; check new polarity
    jrne	00001$		; rising edge, so record start count

    subw	x, (1, sp)	; current - start = length

    ld		a, _clock_hi
    sbc		a, (9, y)	; check for excess time
    jreq	00003$
    ldw		x, #0xffff
00003$:
    pushw	x
    ldw		x, y
    ldw		x, (x)		; output pin and callback
    ldw		x, (2, x)	; callback
    call	(x)
    popw	x
    jra		00002$

00001$:
    ldw		(7, y), x
    ld		a, _clock_hi
    ld		(9, y), a

00002$:
    addw	sp, #2
__endasm;
}

/******************************************************************************
 *
 *  Timer2 capture interrupt
 */

void ping_isr_cap(void) __interrupt (IRQ_TIM2C)
{
__asm
#ifdef PING_USE_C1
    btjf	_active, #1, 00002$
    btjf	_TIM2_SR1, #1, 00002$
    ldw		x, #_ping_data
    call	_isr_chan
00002$:
#endif
#ifdef PING_USE_C2
    btjf	_active, #2, 00003$
    btjf	_TIM2_SR1, #2, 00003$
    ldw		x, #_ping_data+10
    call	_isr_chan
00003$:
#endif
#ifdef PING_USE_C3
    btjf	_active, #3, 00009$
    btjf	_TIM2_SR1, #3, 00009$
    ldw		x, #_ping_data+20
    call	_isr_chan
00009$:
#endif
__endasm;
}

/******************************************************************************
 *
 *  Timer2 clock interrupt
 */

void ping_isr_clk(void) __interrupt (IRQ_TIM2C)
{
__asm
    bres	_TIM2_SR1, #0	/* clear interrupt */
    inc		_clock_hi
__endasm;
}

/******************************************************************************
 *
 *  Notes on the HC-SR04 module:
 *
 *  The host starts a new ping by raising the "trigger" pin for
 *  ten microseconds. The output ("echo") line is low for about
 *  450 microseconds, then goes high to give a pulse that is the
 *  round trip time of the ultrasonic pulse.
 *
 *  So we want to start our count at the "echo" positive edge,
 *  and get the end count at the negative edge. Then it is just
 *  dividing by the speed of sound to get centimeters or inches.
 *
 *  It appears that if the unit does not receive an echo, the
 *  output will stay high for about 190 milliseconds and ignore
 *  a new trigger during this time.
 */
