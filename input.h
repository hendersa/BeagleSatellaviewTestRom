/**********************************************************
 * Modified input functions from SNES SDK.
 *
 * License: LGPL (Refer to COPYING file)
 * Copyright 2020 Andrew Henderson (hendersa@icculus.org)
 **********************************************************/

#ifndef __INPUT_H__
#define __INPUT_H__

#include <stdint.h>

#define TR_BUTTON 0x0010
#define TL_BUTTON 0x0020
#define X_BUTTON 0x0040
#define A_BUTTON 0x0080
#define RIGHT_BUTTON 0x0100
#define LEFT_BUTTON 0x0200
#define DOWN_BUTTON 0x0400
#define UP_BUTTON 0x0800
#define START_BUTTON 0x1000
#define SELECT_BUTTON 0x2000
#define Y_BUTTON 0x4000
#define B_BUTTON 0x8000

uint16_t getjoystatus(const uint16_t j);
void clearjoy(const uint16_t j);

#endif /* __INPUT_H__ */

