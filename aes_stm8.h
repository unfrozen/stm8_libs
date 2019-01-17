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
 *  Permission is hereby granted for any use.
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

