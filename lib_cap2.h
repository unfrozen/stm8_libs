/*
 *  File name:  lib_cap2.h
 *  Date first: 01/08/2018
 *  Date last:  01/09/2018
 *
 *  Description: STM8 Library for capture on Timer2
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Initialize timer2 for capture
 *  in: timer prescaler
 */

void cap2_init(char);

#define CAP2_16_MHZ	0	/* capture up to 4 ms */
#define CAP2_1_US	4	/* capture up to 65 ms */
#define CAP2_4_US	6	/* capture up to 1/4 sec */
#define CAP2_1_MS	14	/* capture up to 65 sec */

/*
 *  Get count of captures waiting
 */

char cap2_count(void);

/*
 *  Get capture value (wait if needed)
 */

int cap2_get(void);

/*
 *  Get overflow count
 */

int cap2_overflow(void);

/*
 *  Interrupt prototypes must be included with main()
 */

#include "vectors.h"

void cap2_isr(void) __interrupt (IRQ_TIM2C);
