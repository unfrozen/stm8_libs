/*
 *  File name:  lib_ping.h
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
 *  IMPORTANT: This library is not compatible with lib_pwm.
 *  
 ******************************************************************************
 *
 *  Bring in IO pin structures
 */
#include "lib_pins.h"

/*
 *  LIBRARY CONFIGURATION:
 *
 *  Save code space by enabling only the PING channels you use.
 *  Comment out (or delete) the channels you do not use.
 */
#define PING_USE_C1
#define PING_USE_C2
#define PING_USE_C3

/*
 *  Initialize device
 *  in: Timer2 channels, output pin(s), callback function(s)
 */

void ping_init(char, IO_CALL_INT *);

/* Channels and their pins on STM8S103 20SSOP */

#define PING_CHAN1	1	/* D4 pin 1 */
#define PING_CHAN2	2	/* D3 pin 20 */
#define PING_CHAN3	4	/* A3 pin 10 */

/* Callback function gets round trip time in microseconds, or -1 (0xffff)
 * if there is no echo response. This callback is in interrupt context,
 * so don't make expensive function calls. Be alert for possible races.
 */

/* Divisors to convert return value into CM, inches, or microseconds */

#define PING_CM		58	/* get distance in centimeters */
#define PING_INCH	148	/* get distance in inches */
#define PING_USEC	2	/* get distance in microseconds */

/*
 *  Start ping process (ping may take up to 20ms to complete)
 *  in:  Channel: PING_CHAN1/2/3
 */

void ping_send(char);

void ping_isr_clk(void) __interrupt (IRQ_TIM2);
void ping_isr_cap(void) __interrupt (IRQ_TIM2C);
