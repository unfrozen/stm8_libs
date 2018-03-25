/*
 *  File name:  lib_cli.c
 *  Date first: 02/19/2018
 *  Date last:  02/23/2018
 *
 *  Description: STM8 Library for handling commands from input source.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *
 ******************************************************************************
 *
 */

#include "lib_cli.h"

#define CMD_CR		0x0d
#define	CMD_BELL	0x07
#define CMD_BS		0x08
#define CMD_CRLF	"\r\n"

static char	cmpkey(char *, char *);	/* compare keyword */
static char	toupper(char);
static char	tolower(char);

static COMMAND_TAB *command_tab;
static char	    command_len;
static char	*buf_in;
static char	 buf_len;

static char	*cmd_ptr;
static char	 cmd_len;

static char	(*getc)(void);
static void	(*putc)(char);
static char	(*rxct)(void);

/******************************************************************************
 *
 *  Command init
 *  in: COMMAND_TAB, tab size, command buffer, buffer size
 */

void cli_init(COMMAND_CTX *ctx)
{
    command_tab = ctx->tab;
    buf_in  = ctx->buf_in;
    buf_len = ctx->buf_len - 1;

    getc = ctx->getc;
    putc = ctx->putc;
    rxct = ctx->size;

    cmd_ptr = buf_in;
    cmd_len = 0;
    *cmd_ptr = 0;
}

/******************************************************************************
 *
 *  Poll for command characters
 *  out: non-zero = line ready
 */

char cli_poll(void)
{
    char	cmdchr;

    while (rxct()) {
	cmdchr = getc();
	if (cmdchr == CMD_CR)
	    return 1;
	if (cmdchr == CMD_BS) {
	    if (!cmd_len)
		continue;
	    cmd_len--;
	    cmd_ptr--;
	    *cmd_ptr = 0;
	    putc(CMD_BS);
	    putc(' ');
	    putc(CMD_BS);
	    continue;
	}
	if (cmd_len == buf_len) {
	    putc(CMD_BELL);
	    continue;
	}
	putc(cmdchr);
	*cmd_ptr = cmdchr;
	cmd_len++;
	cmd_ptr++;
	*cmd_ptr = 0;
    }
    return 0;
}

/******************************************************************************
 *
 *  Get keyword number
 *  out: command number or zero (invalid)
 */

char cli_keynum(char *str)
{
    COMMAND_TAB *tab;

    tab = command_tab;
    while (tab->cmd_num) {
	if (cmpkey(str, tab->cmd_name) == 0)
	    return tab->cmd_num;
	tab++;
    }
    return 0;
}

/******************************************************************************
 *
 *  Reset command line
 */

void cli_reset(void)
{
    cmd_ptr = buf_in;
    cmd_len = 0;
    *cmd_ptr = 0;
}

/******************************************************************************
 *
 *  Function using inline assembly follow.
 *  Remove compiler warnings that functions "must return value"
 */

#pragma disable_warning 59

/******************************************************************************
 *
 *  Trim whitespace from string
 *  out: final string length
 */

char cli_trim (char *s)
{
    s;
__asm
    ldw		x, (3, sp)
    ldw		y, (3, sp)
    push	#0		; not in quotes
    push	#0		; final length
    push	#' '		; pretend previous space

00001$:
    ld		a, (x)
    jreq        00090$
    incw	x
    tnz		(3, sp)		; inside quotes?
    jrne	00009$		; do not trim inside quote
    cp		a, #' '
    jrne	00009$
    tnz		(1, sp)
    jrne	00001$
    ld		(1, sp),a
    ld          (y), a
    incw	y
    inc		(2, sp)
    jra		00001$

00009$:
    ld		(y), a
    incw	y
    inc		(2, sp)
    clr		(1, sp)
    cp		a, #'"'
    jrne	00001$
    cpl		(3, sp)
    jra		00001$

00090$:
    tnz		(1, sp)		; trailing space?
    jreq	00091$
    tnz		(2, sp)		; only spaces?
    jreq	00091$
    decw	y
00091$:
    clr		(y)
    ld		a, (2, sp)
    addw	sp, #3
__endasm;
}

/******************************************************************************
 *
 *  Tokenize command line
 *  in:  command line, token pointer list, max tokens
 *  out: token count
 */

char cli_tokenize(char *inbuf, char **toklist, char tokmax)
{
    inbuf, toklist, tokmax;
__asm
    ldw		x, (3, sp)
    ldw		y, (5, sp)
    ld		a, (7, sp)

    push	#0		; (2, sp) token count
    push	#0		; (1, sp) quote flag

00001$:
    ld		a, (x)
    jreq	00090$
    cp		a, #'"'		; does token start with quote?
    jrne	00003$
    incw	x
    cpl		(1, sp)
00003$:
    ldw		(y), x
    incw	y
    incw	y
    inc		(2, sp)
    dec		(9, sp)
    jreq	00090$

00005$:
    ld		a, (x)
    jreq	00090$
    incw	x
    cp		a, #'"'
    jreq	00010$
    tnz		(1, sp)		; inside quote?
    jrne	00005$		; leave quoted token alone

    cp		a, #' '		; space ends token
    jrne	00005$
    decw	x
    clr		(x)		; terminate token		      
    incw	x
    jra		00001$

00010$:
    decw	x
    clr		(x)		; quote is now end of token
    incw	x
    cpl		(1, sp)
    jrne	00001$		; starting new token inside quote
    ld		a, (x)
    cp		a, #' '		; end quote followed by space?
    jrne	00001$
    incw	x		; skip space
    jra		00001$

00090$:
    ld		a, (2, sp)
    addw	sp, #2
__endasm;
}

/******************************************************************************
 *
 *  Remove comment from string (comment char inside double quotes ignored)
 *  in:  string, 4 character string with comment chars (eg, "#;;;")
 */

void cli_comment(char *buf, char *comment)
{
    buf, comment;
__asm
    ldw		y, (3, sp)
    ldw		x, (5, sp)
    push	#0		; quotes flag
00001$:
    ld		a, (y)
    jreq	00095$
    incw	y
    cp		a, #'"'
    jreq	00010$
    tnz		(1, sp)		; inside quotes?
    jrne	00001$
    cp		a, (x)
    jreq	00005$
    cp		a, (1, x)
    jreq	00005$
    cp		a, (2, x)
    jreq	00005$
    cp		a, (3, x)
    jrne	00001$
00005$:
    decw	y
    clr		(y)
    jra		00095$
00010$:
    cpl		(1, sp)
    jra		00001$
00095$:
    addw	sp, #1
__endasm;
}

/******************************************************************************
 *
 *  Make uppercase
 */

static char toupper(char c)
{
    c;
__asm
    cp		a, #'a'
    jrc		00001$
    cp		a, #'z' + 1
    jrnc	00001$
    sub		a, #0x20
00001$:
__endasm;
}

static char tolower(char c)
{
    c;
__asm
    cp		a, #'A'
    jrc		00001$
    cp		a, #'Z' + 1
    jrnc	00001$
    add		a, #0x20
00001$:
__endasm;
}

/******************************************************************************
 *
 *  Compare keywords
 *
 *  in: word 1 (upper/lower), word 2 (lowercase)
 * out: zero = match
 */

static char cmpkey(char *in, char *list)
{
    in, list;
__asm
    ldw		x, (3, sp)
    ldw		y, (5, sp)
00001$:
    ld		a, (y)
    jreq	00090$
    incw	y
    call	_tolower
    sub		a, (x)
    jrne	00090$
    incw	x
    jra		00001$
00090$:
__endasm;
}
