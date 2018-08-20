/*
 *  File name:  lib_pcm.h
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
 *  One, two, or three channels are supported.
 *  This library uses Timer2
 *
 *  Initialize PCM
 *  in: mode, channels to use
 */

void pcm_init(char, char);

#define PCM_DUTY  1		/* duty cycle 0 to 100 percent */
#define PCM_SERVO 2		/* send servo signal every 20ms */

#define PCM_C1	1
#define PCM_C2	2
#define	PCM_C3	4

/*
 *  Set new PCM duty cycle
 *  in: channel, percentage: 0 to 100
 */

void pcm_duty(char, char);

/*
 *  Set new servo position
 *  in: channel, position: 0 to 255
 */

void pcm_servo(char, char);
