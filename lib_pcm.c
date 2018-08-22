/*
 *  File name:  lib_pcm.c
 *  Date first: 08/20/2018
 *  Date last:  08/21/2018
 *
 *  Description: STM8 Library for PCM and servos
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *
 ******************************************************************************
 *
 */

#include "stm8s_header.h"

#include "lib_eeprom.h"
#include "lib_pcm.h"

static void set_duty(char, unsigned int);	/* internal set duty cycle */
static void set_remap(char);	/* set remap channel 3 pin on STM8S105 */

/******************************************************************************
 *
 *  Initialize PCM
 *  in: mode, channels to use
 */

void pcm_init(char mode, char chans)
{
#ifdef STM8103
    TIM2_PSCR = 3;      /* prescaler = 8 for 16mhz */
#endif
#ifdef STM8105
    char	remap = 0;
    TIM2_PSCR = 2;      /* prescaler = 4 for 8mhz */
#endif
    switch (mode) {
    case PCM_DUTY :
	TIM2_ARRH = 0;
	TIM2_ARRL = 199;	/* 100 uS = 10khz */
	break;
    case PCM_SERVO :
	TIM2_ARRH = 39999 >> 8;	/* 20000 uS = 50 hz */
	TIM2_ARRL = 39999 & 255;
	break;
    }
    if (chans & PCM_C1) {
	TIM2_CCR1H = 0;
	TIM2_CCR1L = 0;
	TIM2_CCMR1  = 0x68;	/* PCM mode 1, use preload register */
	TIM2_CCER1 |= 0x01;	/* output enable, normal polarity */
    }
    if (chans & PCM_C2) {
	TIM2_CCR2H = 0;
	TIM2_CCR2L = 0;
	TIM2_CCMR2  = 0x68;
	TIM2_CCER1 |= 0x10;
    }
    if (chans & PCM_C3) {
#ifdef STM8105
	remap = 2;
#endif
	TIM2_CCR3H = 0;
	TIM2_CCR3L = 0;
	TIM2_CCMR3  = 0x68;
	TIM2_CCER2 |= 0x01;
    }
#ifdef STM8105
    set_remap(remap);
#endif
    TIM2_CR1 = 0x81;		/* use TIM2_ARR preload register, enable */
}

/******************************************************************************
 *
 *  Set new PCM duty cycle
 *  in: channel(s), percentage/2: 0 to 200
 */

void pcm_duty(char chan, char duty)
{
    set_duty(chan, duty);
}

/******************************************************************************
 *
 *  Set new servo position
 *  in: channel(s), position: 0 to 250
 */

void pcm_servo(char chan, char pos)
{
    int		duty;

    duty  = pos * 16;		/* get 0 to 2000 milliseconds */
    duty += 1000;		/* now 500 to 2500 mS */

    set_duty(chan, duty);
}

/******************************************************************************
 *
 *  Set duty count
 *  in: channel(s), new duty count (times 1/2 microsecond)
 */

static void set_duty(char chan, unsigned int duty)
{
    char	ch, cl;

    ch = duty >> 8;
    cl = duty & 0xff;

    if (chan & PCM_C1) {
	TIM2_CCR1H = ch;
	TIM2_CCR1L = cl;
    }
    if (chan & PCM_C2) {
	TIM2_CCR2H = ch;
	TIM2_CCR2L = cl;
    }
    if (chan & PCM_C3) {
	TIM2_CCR3H = ch;
	TIM2_CCR3L = cl;
    }
}

/******************************************************************************
 *
 *  Set remapping for output channel 3 on STM8S105K
 *  in: 0 = no, 2 = yes
 */
#ifdef STM8105
static void set_remap(char val)
{
    char	cfg;

    eeprom_unlock();
    cfg = OPT2;
    cfg &= 0xfd;
    cfg |= val;
    OPT2 = cfg;
    NOPT2 = ~cfg;
    eeprom_lock();
}
#endif
