/*
 *  File name:  lib_pcm.c
 *  Date first: 08/20/2018
 *  Date last:  08/20/2018
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
#include "lib_pcm.h"

/******************************************************************************
 *
 *  Initialize PCM
 *  in: Channels to use
 */

void pcm_init(char mode, char chans)
{
    char	i;

    i, mode, chans;

}

/******************************************************************************
 *
 *  Set new PCM duty cycle
 *  in: channel, percentage: 0 to 100
 */

void pcm_duty(char chan, char duty)
{
    chan, duty;
}

/******************************************************************************
 *
 *  Set new servo position
 *  in: channel, position: 0 to 255
 */

void pcm_servo(char chan, char pos)
{
    chan, pos;
}

