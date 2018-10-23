/*
 *  File name:  lib_cli.h
 *  Date first: 02/19/2018
 *  Date last:  03/23/2018
 *
 *  Description: STM8 Library for handling commands from input source.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Command structure
 */

typedef struct {
    char	 cmd_num;	/* up to 255 commands */
    char	*cmd_name;	/* case insensitive */
    char	*cmd_info;	/* command help or NULL */
} COMMAND_TAB;

typedef struct {
    COMMAND_TAB *tab;		/* command table */
    char	*buf_in;	/* input buffer */
    char	 buf_len;
    char (*getc)(void);		/* char get function */
    void (*putc)(char);		/* char put function */
    char (*size)(void);		/* rx buf count */
} COMMAND_CTX;

/*
 *  Command init
 *  in: COMMAND_TAB, tab size, command buffer, buffer size
 */

void cli_init(COMMAND_CTX *);

/*
 *  Poll UART for command characters
 *  out: non-zero = line ready
 */

char cli_poll(void);

/*
 *  Get command number
 *  out: command number or zero (invalid)
 */

char cli_keynum(char *);

/*
 *  Reset command line
 */

void cli_reset(void);

/*
 *  Remove comment from string (comment char inside double quotes ignored)
 *  in:  string, 4 character string with comment chars (eg, "#;;;")
 */

void cli_comment(char *, char *);

/*
 *  Trim whitespace from buffer
 *  Spaces inside double quotes are left intact.
 *
 *  out: final string length
 */

char cli_trim(char *);

/*
 *  Tokenize string
 *
 *  Extra whitespace must be trimmed first. Suggest cli_trim() above.
 *  Tokens are separated by spaces or double quotes.
 *  Any string inside double quotes is a single token and the quotes are removed.
 *  An empty string returns a token count of zero.
 *  Any excess tokens above max # are not processed and show in the last token.
 *
 *  in: string, token pointers, max token count
 * out: token count
 */

char cli_tokenize(char *, char **, char);

/*
 *  Print available commands
 */

void cli_help(void);
