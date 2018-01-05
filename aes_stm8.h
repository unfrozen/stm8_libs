/*
 *  Program:  aes_stm8.h
 *
 *  Date first: 12/17/2017
 *  Date last:  12/17/2017
 *
 *  Author:  Richard Hodges
 * 
 *  This is a library for encoding and decoding AES 128 blocks on STM8.
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
 *  Types
 */

#ifndef _BYTE
#define _BYTE
typedef unsigned char BYTE;
#endif

typedef struct {
    BYTE	 *block;
    BYTE	 key[11][16];
    unsigned char round;
} AES_CTX;

void aes_new_key(AES_CTX *, BYTE *);
void aes_encrypt(AES_CTX *, BYTE *);
void aes_decrypt(AES_CTX *, BYTE *);

