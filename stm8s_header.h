/*
 * Choose stm8s_003, stm8s_103, stm8s_105, or stm8s_107 header,
 * depending on compile directive.
 */

#ifdef STM8003
#include "stm8s_003.h"
#include "stm8s_003.inc"
#endif

#ifdef STM8103
#include "stm8_103.h"
#include "stm8_103.inc"
#endif

#ifdef STM8105
#include "stm8_105.h"
#include "stm8_105.inc"
#endif

#ifdef STM8S207
#include "stm8s_207.h"
#endif

#include "vectors.h"

/*
 *  Choose syntax for inline assembly.
 */
#ifdef __SDCC
#define __ASM __asm
#define __ENDASM __endasm;
#else
#define __ASM #asm		/* Cosmic syntax */
#define __ENDASM #endasm
#endif

/*
 *  Make sure __SDCCCALL is defined so inline assembly can handle
 *  all versions of the calling convention version.
 */
#ifndef __SDCCCALL
#define __SDCCCALL 0
#endif

