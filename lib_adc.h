/*
 *  File name:  lib_adc.h
 *  Date first: 09/14/2018
 *  Date last:  10/11/2018
 *
 *  Description: Library for using STM8S ADC.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Initialize
 *  in:  bits for ADC channel(s) to use
 */
void adc_init(char);

/* Pin numbers are for 103F SO20 and 105K LQFP32 */

#define ADC_CH0		0x01	/*         B0: 16 */
#define ADC_CH1		0x02	/*         B1: 15 */
#define ADC_CH2		0x04	/* C4: 14  B2: 14 */
#define ADC_CH3		0x08	/* D2: 19  B3: 13 */
#define ADC_CH4		0x10	/* D3: 20  B4: 12 */
#define ADC_CH5		0x20	/* D5:  2  B5: 11 */
#define ADC_CH6		0x40	/* D6:  3         */
#define ADC_CH12	0x80	/*         F4:  8 */
#define ADC_MASK	0x7f

/*
 *  Get current raw A/D value
 *  in:  ADC channel (0 to 12)
 *  out: 10-bit A/D conversion value, -1 if timeout
 */
short adc_val(char);

/*
 *  Get average of 4 A/D samples
 *  in:  ADC channel (0 to 12)
 *  out: 10-bit A/D average value, -1 if timeout
 */
short adc_avg(char);
