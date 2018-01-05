/*
 *  File name:  aes_stm8.c
 *  Date first: 12/05/2017
 *  Date last:  12/17/2017
 *
 *  Description: AES 128-bit code for STM8 processor.
 *
 *  Author:     Richard Hodges
 *
 ******************************************************************************
 *
 *  Copyright (C) 2017 Richard Hodges.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * If you wish to license this code under different terms, please
 * contact the author: richard@hodges.org
 *
 ******************************************************************************
 *
 *  Includes
 */
#include <string.h>
#include "aes_stm8.h"

/******************************************************************************
 *
 *  Differences between SDCC and COSMIC
 *
 *  Note that SDCC pushes all pointers on the stack before calling, and
 *  Cosmic puts the rightmost (or only) pointer in X. There is only one
 *  assembly function that takes two pointers, and it has to deal with
 *  the Cosmic compiler making all calls far. The others can just use
 *  the pointer given in X.
 */

//#define COSMIC		/* if not defined, assume SDCC */

/******************************************************************************
 *
 *  Locals
 */

static void round_enc(AES_CTX *);
static void round_dec(AES_CTX *);

static void mix_col_dec(BYTE *);
static void mix_col_enc(BYTE *);
static BYTE mix_mul2(BYTE);

static BYTE sbox_enc(BYTE);
static void sbox_enc_block(BYTE *);
static void sbox_dec_block(BYTE *);

static void mix_key(BYTE *, BYTE *);

static void shift_dec(BYTE *);
static void shift_enc(BYTE *);

/******************************************************************************
 *
 *  Encrypt a block using this library
 *
 *  in: key, block
 */

void aes_encrypt(AES_CTX *ctx, BYTE *block)
{
    int		 i;

    ctx->block = block;

    mix_key(block, ctx->key[0]);

    ctx->round = 1;
    for (i = 0; i < 10; i++) {
	round_enc(ctx);

    }
}

/******************************************************************************
 *
 *  Decrypt a block using this library
 *
 *  in: key, block
 */

void aes_decrypt(AES_CTX *ctx, BYTE *block)
{
    int		 i;

    ctx->block = block;

    ctx->round = 11;
    for (i = 0; i < 10; i++) {
	round_dec(ctx);
    }
    mix_key(block, ctx->key[0]);
}

/******************************************************************************
 *
 *  Do decryption round
 *
 *  in: AES_CTX
 */

static void round_dec(AES_CTX *ctx)
{
    int		 r;

    ctx->round--;
    r = ctx->round;

    mix_key(ctx->block, ctx->key[r]);

    if (ctx->round != 10) {
	mix_col_dec(ctx->block +  0);
	mix_col_dec(ctx->block +  4);
	mix_col_dec(ctx->block +  8);
	mix_col_dec(ctx->block + 12);
    }
    shift_dec(ctx->block);

    sbox_dec_block(ctx->block);
}

/******************************************************************************
 *
 *  Do encryption round
 *
 *  in: AES_CTX
 */

static void round_enc(AES_CTX *ctx)
{
    int		 r;

    sbox_enc_block(ctx->block);

    shift_enc(ctx->block);

    if (ctx->round != 10) {
	mix_col_enc(ctx->block +  0);
	mix_col_enc(ctx->block +  4);
	mix_col_enc(ctx->block +  8);
	mix_col_enc(ctx->block + 12);
    }
    r = ctx->round;

    mix_key(ctx->block, ctx->key[r]);
    ctx->round++;
}

/******************************************************************************
 *
 *  Shift block rows, decrypt
 *
 *  in: block
 */

static void shift_dec(BYTE *block)
{
#ifdef ORIG_C
    BYTE	 temp;

    temp = block[1];
    block[ 1] = block[13];
    block[13] = block[ 9];
    block[ 9] = block[ 5];
    block[ 5] = temp;

    temp = block[ 2];
    block[ 2] = block[10];
    block[10] = temp;
    temp = block[ 6];
    block[ 6] = block[14];
    block[14] = temp;

    temp = block[ 3];
    block[ 3] = block[ 7];
    block[ 7] = block[11];
    block[11] = block[15];
    block[15] = temp;
#else
    block;
#ifdef COSMIC
#asm
#else
__asm
    ldw		x, (3, sp)
#endif
    ld		a, (1, x)
    ld		yl, a
    ld		a, (13, x)
    ld		(1, x), a
    ld		a, (9, x)
    ld		(13, x), a
    ld		a, (5, x)
    ld		(9, x), a
    ld		a, yl
    ld		(5, x), a

    ld		a, (2, x)
    ld		yl, a
    ld		a, (10, x)
    ld		(2, x), a
    ld		a, yl
    ld		(10, x), a
    ld		a, (6, x)
    ld		yl, a
    ld		a, (14, x)
    ld		(6, x), a
    ld		a, yl
    ld		(14, x), a

    ld		a, (3, x)
    ld		yl, a
    ld		a, (7, x)
    ld		(3, x), a
    ld		a, (11, x)
    ld		(7, x), a
    ld		a, (15, x)
    ld		(11, x), a
    ld		a, yl
    ld		(15, x), a
#ifdef COSMIC
#endasm
#else
__endasm;
#endif
#endif
}

/******************************************************************************
 *
 *  Shift block rows, encrypt
 *
 *  in: block
 */

static void shift_enc(BYTE *block)
{
#ifdef ORIG_C
    BYTE	 temp;

    temp = block[1];
    block[ 1] = block[ 5];
    block[ 5] = block[ 9];
    block[ 9] = block[13];
    block[13] = temp;

    temp = block[ 2];
    block[ 2] = block[10];
    block[10] = temp;
    temp = block[ 6];
    block[ 6] = block[14];
    block[14] = temp;

    temp = block[15];
    block[15] = block[11];
    block[11] = block[ 7];
    block[ 7] = block[ 3];
    block[ 3] = temp;
#else
    block;
#ifdef COSMIC
#asm
#else
__asm
    ldw		x, (3, sp)
#endif
    ld		a, (1, x)
    ld		yl, a
    ld		a, (5, x)
    ld		(1, x), a
    ld		a, (9, x)
    ld		(5, x), a
    ld		a, (13, x)
    ld		(9, x), a
    ld		a, yl
    ld		(13, x), a

    ld		a, (2, x)
    ld		yl, a
    ld		a, (10, x)
    ld		(2, x), a
    ld		a, yl
    ld		(10, x), a
    ld		a, (6, x)
    ld		yl ,a
    ld		a, (14, x)
    ld		(6, x), a
    ld		a, yl
    ld		(14, x), a

    ld		a, (15, x)
    ld		yl, a
    ld		a, (11, x)
    ld		(15, x), a
    ld		a, (7, x)
    ld		(11, x), a
    ld		a, (3, x)
    ld		(7, x), a
    ld		a, yl
    ld		(3, x), a
#ifdef COSMIC
#endasm
#else
__endasm;
#endif
#endif
}

/******************************************************************************
 *
 *  Multiply by two for mix operation
 */

#ifdef ORIG_C
static BYTE mix_mul2(BYTE val)
{
    BYTE	 retval;

    retval = val << 1;
    if (val & 0x80)
	retval ^= 0x1b;

    return (retval);
}
#endif

/******************************************************************************
 *
 *  Mix column, encryption
 *
 *  in: column
 */

static void mix_col_enc(BYTE *c)
{
#ifdef ORIG_C
    BYTE	 x1[4];
    BYTE	 x2[4];
    BYTE	 x3[4];
    int		 i;

    for (i = 0; i < 4; i++) {
	x1[i] = c[i];
	x2[i] = mix_mul2(x1[i]);
	x3[i] = x2[i] ^ x1[i];
    }
    c[0] = x2[0] ^ x3[1] ^ x1[2] ^ x1[3];
    c[1] = x1[0] ^ x2[1] ^ x3[2] ^ x1[3];
    c[2] = x1[0] ^ x1[1] ^ x2[2] ^ x3[3];
    c[3] = x3[0] ^ x1[1] ^ x1[2] ^ x2[3];
#else
    c;

/* SP + 13, 14 return pointer
 * SP + 10..12 col 3 x1, x2, x3
 * SP +  7..9  col 2 x1, x2, x3
 * SP +  4..6  col 1 x1, x2, x3
 * SP +  1..3  col 0 x1, x2, x3
 */
#ifdef COSMIC
#asm
    exgw	x, y		; y is column pointer
#else
__asm
    ldw		y, (3, sp)	; y is column pointer
#endif
    clrw	x
    sub		sp, #12

    ld		a, (0, y)
    ld		xl, a
    ld		(1, sp), a
    ld		a, (_mix_2, x)
    ld		(2, sp), a
    ld		a, (_mix_3, x)
    ld		(3, sp), a

    ld		a, (1, y)
    ld		xl, a
    ld		(4, sp), a
    ld		a, (_mix_2, x)
    ld		(5, sp), a
    ld		a, (_mix_3, x)
    ld		(6, sp), a

    ld		a, (2, y)
    ld		xl, a
    ld		(7, sp), a
    ld		a, (_mix_2, x)
    ld		(8, sp), a
    ld		a, (_mix_3, x)
    ld		(9, sp), a

    ld		a, (3, y)
    ld		xl, a
    ld		(10, sp), a
    ld		a, (_mix_2, x)
    ld		(11, sp), a
    ld		a, (_mix_3, x)
    ld		(12, sp), a

    exgw	x, y

    ld		a, (2, sp)	; col0 x2
    xor		a, (6, sp)	; col1 x3
    xor		a, (7, sp)	; col2 x1
    xor		a, (10, sp)	; col3 x1
    ld		(0, x), a

    ld		a, (1, sp)	; col0 x1
    xor		a, (5, sp)	; col1 x2
    xor		a, (9, sp)	; col2 x3
    xor		a, (10, sp)	; col4 x1
    ld		(1, x), a

    ld		a, (1, sp)	; col0 x1
    xor		a, (4, sp)	; col1 x1
    xor		a, (8, sp)	; col2 x2
    xor		a, (12, sp)	; col3 x3
    ld		(2, x), a

    ld		a, (3, sp)	; col0 x3
    xor		a, (4, sp)	; col1 x1
    xor		a, (7, sp)	; col2 x1
    xor		a, (11, sp)	; col3 x2
    ld		(3, x), a

    add		sp, #12
#ifdef COSMIC
#endasm
#else
__endasm;
#endif
#endif
}

/******************************************************************************
 *
 *  Mix column, decryption
 *
 *  in: column
 */

static void mix_col_dec(BYTE *c)
{
#ifdef ORIG_C
    BYTE	 x9[4];
    BYTE	 xb[4];
    BYTE	 xd[4];
    BYTE	 xe[4];
    int		 i, val;

    for (i = 0; i < 4; i++) {
	val = c[i];
	x9[i] = val;
	xb[i] = val;
	xd[i] = val;
	val = mix_mul2(val);	/* *2 */
	xb[i] ^= val;
	xe[i]  = val;
	val = mix_mul2(val);	/* *4 */
	xd[i] ^= val;
	xe[i] ^= val;
	val = mix_mul2(val);	/* *8 */
	x9[i] ^= val;
	xb[i] ^= val;
	xd[i] ^= val;
	xe[i] ^= val;
    }
    c[0] = xe[0] ^ xb[1] ^ xd[2] ^ x9[3];
    c[1] = x9[0] ^ xe[1] ^ xb[2] ^ xd[3];
    c[2] = xd[0] ^ x9[1] ^ xe[2] ^ xb[3];
    c[3] = xb[0] ^ xd[1] ^ x9[2] ^ xe[3];
#else
    c;
/* SP +  19, 20 column pointer
 * SP +  17, 18 return pointer
 * SP +  13..16 col 3 x9, x11, x13, x14 values
 * SP +   9..12 col 2 x9, x11, x13, x14 values
 * SP +   5.. 8 col 1 x9, x11, x13, x14 values
 * SP +   1.. 4 col 0 x9, x11, x13, x14 values
 */
#ifdef COSMIC
#asm
    exgw	x, y		; y is column pointer
#else
__asm
    ldw		y, (3, sp)	; y is column pointer
#endif
    clrw	x
    sub		sp, #16

    ld		a, (y)
    ld		xl, a
    ld		a, (_mix_9, x)
    ld		(1, sp), a
    ld		a, (_mix_11, x)
    ld		(2, sp), a
    ld		a, (_mix_13, x)
    ld	       	(3, sp), a
    ld		a, (_mix_14, x)
    ld		(4, sp), a

    ld		a, (1, y)
    ld		xl, a
    ld		a, (_mix_9, x)
    ld		(5, sp), a
    ld		a, (_mix_11, x)
    ld		(6, sp), a
    ld		a, (_mix_13, x)
    ld	       	(7, sp), a
    ld		a, (_mix_14, x)
    ld		(8, sp), a

    ld		a, (2, y)
    ld		xl, a
    ld		a, (_mix_9, x)
    ld		(9, sp), a
    ld		a, (_mix_11, x)
    ld		(10, sp), a
    ld		a, (_mix_13, x)
    ld	       	(11, sp), a
    ld		a, (_mix_14, x)
    ld		(12, sp), a

    ld		a, (3, y)
    ld		xl, a
    ld		a, (_mix_9, x)
    ld		(13, sp), a
    ld		a, (_mix_11, x)
    ld		(14, sp), a
    ld		a, (_mix_13, x)
    ld	       	(15, sp), a
    ld		a, (_mix_14, x)
    ld		(16, sp), a

    ld	    	a, (4, sp)	; col 0 x14
    xor		a, (6, sp)	; col 1 x11
    xor		a, (11, sp)	; col 2 x13
    xor		a, (13, sp)	; col 3 x9 
    ld		(y), a

    ld		a, (1, sp)	; col 0 x9 
    xor		a, (8, sp)	; col 1 x14
    xor		a, (10, sp)	; col 2 x11
    xor		a, (15, sp)	; col 3 x13
    ld		(1, y), a

    ld		a, (3, sp)	; col 0 x13
    xor		a, (5, sp)	; col 1 x9 
    xor		a, (12, sp)	; col 2 x14
    xor		a, (14, sp)	; col 3 x11
    ld		(2, y), a

    ld		a, (2, sp)	; col 0 x11
    xor		a, (7, sp)	; col 1 x13
    xor		a, (9, sp)	; col 2 x9 
    xor		a, (16, sp)	; col 3 x14
    ld		(3, y), a

    add		sp, #16
#ifdef COSMIC
#endasm
#else
__endasm;
#endif
#endif
}

/******************************************************************************
 *
 *  SBOX values, encryption
 */

const BYTE sbox_tab_enc[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

/******************************************************************************
 *
 *  SBOX lookup, encryption
 *
 *  in: index value
 * out: new value
 */

static BYTE sbox_enc(BYTE val)
{
    return (sbox_tab_enc[val]);	/* Don't optimize, only used for key gen */
}

/******************************************************************************
 *
 *  SBOX block substitution, encoding
 *
 *  in: block
 */

void sbox_enc_block(BYTE *block)
{
    block;
#ifdef COSMIC
#asm
    exgw	x, y
#else
__asm
    ldw		y, (3, sp)
#endif
    clrw	x

    ld		a, (y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(y), a

    ld		a, (1, y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(1, y), a

    ld		a, (2, y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(2, y), a

    ld		a, (3, y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(3, y), a

    ld		a, (4, y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(4, y), a

    ld		a, (5, y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(5, y), a

    ld		a, (6, y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(6, y), a

    ld		a, (7, y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(7, y), a

    ld		a, (8, y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(8, y), a

    ld		a, (9, y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(9, y), a

    ld		a, (10, y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(10, y), a

    ld		a, (11, y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(11, y), a

    ld		a, (12, y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(12, y), a

    ld		a, (13, y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(13, y), a

    ld		a, (14, y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(14, y), a

    ld		a, (15, y)
    ld		xl, a
    ld		a, (_sbox_tab_enc, x)
    ld		(15, y), a

#ifdef COSMIC
#endasm
#else
__endasm;
#endif
}
/******************************************************************************
 *
 *  SBOX block substitution, decoding
 *
 *  in: block
 */

void sbox_dec_block(BYTE *block)
{
    block;
#ifdef COSMIC
#asm
    exgw	x, y
#else
__asm
    ldw		y, (3, sp)
#endif
    clrw	x

    ld		a, (y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(y), a

    ld		a, (1, y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(1, y), a

    ld		a, (2, y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(2, y), a

    ld		a, (3, y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(3, y), a

    ld		a, (4, y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(4, y), a

    ld		a, (5, y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(5, y), a

    ld		a, (6, y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(6, y), a

    ld		a, (7, y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(7, y), a

    ld		a, (8, y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(8, y), a

    ld		a, (9, y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(9, y), a

    ld		a, (10, y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(10, y), a

    ld		a, (11, y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(11, y), a

    ld		a, (12, y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(12, y), a

    ld		a, (13, y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(13, y), a

    ld		a, (14, y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(14, y), a

    ld		a, (15, y)
    ld		xl, a
    ld		a, (_sbox_tab_dec, x)
    ld		(15, y), a

#ifdef COSMIC
#endasm
#else
__endasm;
#endif
}

/******************************************************************************
 *
 *  SBOX values, decryption
 */

const BYTE sbox_tab_dec[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
    0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
    0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
    0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
    0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
    0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
    0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
    0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
    0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
    0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
    0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
    0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
    0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
    0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
    0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
    0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

/******************************************************************************
 *
 *  Apply key to block
 */

static void mix_key(BYTE *block, BYTE *key)
{
#ifdef ORIG_C
    int		i;

    for (i = 0; i < 16; i++)
	*block++ ^= *key++;
#else
    block;
    key;
#ifdef COSMIC
#asm
    ldw		x, (1, sp)
    ldw		y, (6, sp)
#else
__asm
    ldw		x, (3, sp)	; block
    ldw		y, (5, sp)	; key 
#endif
    ld		a, (x)
    xor		a, (y)
    ld		(x), a

    ld		a, (1, x)
    xor		a, (1, y)
    ld		(1, x), a

    ld		a, (2, x)
    xor		a, (2, y)
    ld		(2, x), a

    ld		a, (3, x)
    xor		a, (3, y)
    ld		(3, x), a

    ld		a, (4, x)
    xor		a, (4, y)
    ld		(4, x), a

    ld		a, (5, x)
    xor		a, (5, y)
    ld		(5, x), a

    ld		a, (6, x)
    xor		a, (6, y)
    ld		(6, x), a

    ld		a, (7, x)
    xor		a, (7, y)
    ld		(7, x), a

    ld		a, (8, x)
    xor		a, (8, y)
    ld		(8, x), a

    ld		a, (9, x)
    xor		a, (9, y)
    ld		(9, x), a

    ld		a, (10, x)
    xor		a, (10, y)
    ld		(10, x), a

    ld		a, (11, x)
    xor		a, (11, y)
    ld		(11, x), a

    ld		a, (12, x)
    xor		a, (12, y)
    ld		(12, x), a

    ld		a, (13, x)
    xor		a, (13, y)
    ld		(13, x), a

    ld		a, (14, x)
    xor		a, (14, y)
    ld		(14, x), a

    ld		a, (15, x)
    xor		a, (15, y)
    ld		(15, x), a

#ifdef COSMIC
#endasm
#else
__endasm;
#endif
#endif
}


/******************************************************************************
 *
 *  Setup AES key
 *
 *  in: AES_CTX, new key
 *
 *  Please note: The author expects that the AES key will rarely change on
 *  a device, and this function is not optimized. It uses 3982 cycles on the
 *  COSMIC simulator. If your application changes keys frequently and you wish
 *  for an optimized function, please contact me: richard@hodges.org
 */

void aes_new_key(AES_CTX *ctx, BYTE *key)
{
    BYTE	*kptr;
    int		 rcon;
    int		 i, j;
    BYTE	 t1, t2, t3, t4, tt;

    memcpy(ctx->key[0], key, 16);

    rcon = 1;
    kptr = ctx->key[1];
    t1 = kptr[-4];
    t2 = kptr[-3];
    t3 = kptr[-2];
    t4 = kptr[-1];

    for (i = 1; i < 11; i++) {
	for (j = 0; j < 4; j++) {
	    if (j == 0) {
		tt = sbox_enc(t1);
		t1 = sbox_enc(t2) ^ rcon;
		t2 = sbox_enc(t3);
		t3 = sbox_enc(t4);
		t4 = tt;
	    }
	    t1 ^= kptr[-16];
	    t2 ^= kptr[-15];
	    t3 ^= kptr[-14];
	    t4 ^= kptr[-13];
	    kptr[0] = t1;
	    kptr[1] = t2;
	    kptr[2] = t3;
	    kptr[3] = t4;
	    kptr += 4;
	}
	rcon <<= 1;
	if (rcon & 0x100)
	    rcon ^= 0x1b;
    }
}

#include "aes_tables.h"		/* column mix lookup tables */
