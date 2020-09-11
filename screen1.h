/**********************************************************
 * Main register screen functions (input, update, drawing).
 *
 * License: LGPL (Refer to COPYING file)
 * Copyright 2020 Andrew Henderson (hendersa@icculus.org)
 **********************************************************/

#ifndef __SCREEN1_H__
#define __SCREEN1_H__

#include <stdint.h>

extern uint8_t screen1ProcessInput(const uint16_t input);
extern void screen1Redraw(void);
extern void screen1PartialRedraw(void);
extern void screen1ReadAllRegisters(void);

#endif /* __SCREEN1_H__ */

