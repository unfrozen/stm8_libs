/*
 *  File name:  aes_tables.c
 *  Date first: 12/13/2017
 *  Date last:  12/13/2017
 *
 *  Description: Output AES mix tables.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2017, Richard Hodges.  All rights reserved.
 *  Permission is granted to use or copy for any purpose.
 *
 ******************************************************************************
 *
 *  Includes
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 *
 *  Types
 */

#ifndef _BYTE
#define _BYTE
typedef unsigned char BYTE;
#endif


static BYTE mix_mul2(BYTE);
static void print_table(char *, BYTE *);

/******************************************************************************
 *
 */

int main(int argc, char *argv[])
{
    BYTE	x_2[256];
    BYTE       	x_3[256];
    BYTE	x_9[256];
    BYTE	x_11[256];
    BYTE	x_13[256];
    BYTE	x_14[256];
    int		i, val;

    for (i = 0; i < 256; i++) {
	val = i;
	x_3[i]  = val;		/* x 1 */
	x_9[i]  = val;
	x_11[i] = val;
	x_13[i] = val;
	val = mix_mul2(val);	/* x 2 */
	x_2[i]   = val;
	x_3[i]  ^= val;
	x_11[i] ^= val;
	x_14[i]  = val;
	val = mix_mul2(val);	/* x 4 */
	x_13[i] ^= val;
	x_14[i] ^= val;
	val = mix_mul2(val);	/* x 8 */
	x_9[i]  ^= val;
	x_11[i] ^= val;
	x_13[i] ^= val;
	x_14[i] ^= val;
    }
    print_table("mix_2", x_2);
    print_table("mix_3", x_3);
    print_table("mix_9", x_9);
    print_table("mix_11", x_11);
    print_table("mix_13", x_13);
    print_table("mix_14", x_14);

    return 0;
}

/******************************************************************************
 *
 *  Print mix table
 */

static void print_table(char *tabname, BYTE *table)
{
    int		 i, j;

    printf("const BYTE %s[256] = {", tabname);
    for (i = 0; i < 32; i++) {
	printf("\n    ");
	for (j = 0; j < 8; j++)
	    printf("0x%02x, ", *table++);
    }
    printf("\n};\n");
}

/******************************************************************************
 *
 *  Multiply by two for mix operation
 */

static BYTE mix_mul2(BYTE val)
{
    BYTE	 retval;

    retval = val << 1;
    if (val & 0x80)
	retval ^= 0x1b;

    return (retval);
}
