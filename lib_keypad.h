/*
 *  File name:  lib_keypad.h
 *  Date first: 10/11/2018
 *  Date last:  10/14/2018
 *
 *  Description: STM8 Library for simple keypad
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Size of keypad buffer
 */
#define KP_BUFSIZE	11	/* may be any arbitrary size */

#define KP_DEBOUNCE	40	/* debounce count in milliseconds */
#define KP_KEY_REL		/* notify when key is released (bit-7 set) */
#define KP_POLL_MS	4	/* actual key polling rate, milliseconds */

/*
 *  Keypad configuration
 *  Rows and columns
 */
typedef struct {
    volatile char *reg_base;	/* port ODR register */
    char	 reg_mask;	/* bit for row or column */
} KP_PIN;

/*
 *  Initialize
 *  in: row config, column config
 */
void keypad_init(KP_PIN *, KP_PIN *);

/*
 *  Poll for keypad status
 *  Call every millisecond
 *  (profiling with Timer4 gives 36 uSec per poll)
 */
void keypad_poll(void);

/*
 *  Get key from keypad
 *  out: key or zero (bit-7 set for key release)
 */
char keypad_getc(void);

/*
 *  Install alternate keypad map
 *  in: new keypad map
 */
void keypad_kmap(char *);
