/**********************************************************
 * Modified graphics functions from SNES SDK.
 *
 * License: LGPL (Refer to COPYING file)
 * Copyright 2020 Andrew Henderson (hendersa@icculus.org)
 **********************************************************/

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdint.h>

/* graphics */

void settiles(const uint16_t b, uint8_t* p1, const uint16_t size);
void setmap(const uint16_t b, uint8_t* p1);
void setpalette(const uint8_t* pal);
void enablescreen(void);
void screenmode(const uint8_t m);
void setsprite(const uint16_t s, const uint8_t x, const uint8_t y, const uint8_t t, const uint8_t p);
void waitforvsync(void);
void delay(const uint16_t d);
void resettimer(void);
void sync(const uint16_t d);

#endif /* __GRAPH_H__ */

