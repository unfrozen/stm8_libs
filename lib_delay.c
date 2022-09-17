/*
 *  File name:  lib_delay.h
 *  Date first: 12/19/2017
 *  Date last:  09/16/2022
 *
 *  Description: STM8 Library for short delays.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018, 2022 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 *  Code copied out of lib_lcd.c
 *
 ******************************************************************************
 *
 *  Delays assume 16mhz on stms103 and 8 mhz on stm8s105
 */
#include "lib_delay.h"

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
#if __SDCCCALL == 0
        ld      a, (3, sp)
#endif
        dec     a
        clrw    x
        ld      xl,a
#ifdef STM8103
        sllw    x
#endif
        sllw    x
00001$:
        nop			; (1)
        decw    x               ; (1)
        jrne    00001$          ; (2)
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
