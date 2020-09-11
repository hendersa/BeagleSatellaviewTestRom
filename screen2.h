/**********************************************************
 * Help screen functions (input, update, drawing).
 *
 * License: LGPL (Refer to COPYING file)
 * Copyright 2020 Andrew Henderson (hendersa@icculus.org)
 **********************************************************/

#ifndef __SCREEN2_H__
#define __SCREEN2_H__

#include <stdint.h>

extern uint8_t screen2ProcessInput(const uint16_t input);
extern void screen2Redraw(void);

#endif /* __SCREEN2_H__ */

