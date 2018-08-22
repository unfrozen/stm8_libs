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
 *  STM8S103F:
 *    C1 = pin  1 (D4) alt: 15 (C5)
 *    C2 = pin 20 (D3)
 *    C3 = pin 10 (A3) alt: 19 (D2)
 *
 *  STM8S105K: (change pin numbers for SPDIP32)
 *    C1 = pin 29 (D4)
 *    C2 = pin 28 (D3)
 *    C3 = pin 27 (D2) reassigned: note pin A3 is only on the 48 pin part
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
 *  in: channel(s), percentage/2: 0 to 200
 */

void pcm_duty(char, char);

/*
 *  Set new servo position
 *  in: channel(s), position: 0 to 255 (0.5 to 2.5 mS 125 is center: 1.5 mS)
 */

void pcm_servo(char, char);

/* Stated specs:
 * Tower Pro SG90: 0.5 to 2.4 mS (https://servodatabase.com/servo/towerpro/sg90)
 * Futaba S3003:   0.5 to 3.0 mS (https://servodatabase.com/servo/futaba/s3003)
 *    Note: Seems to stop before 2.5 mS
 */
