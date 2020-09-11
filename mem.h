/**********************************************************
 * Modified memory functions from SNES SDK.
 *
 * License: LGPL (Refer to COPYING file)
 * Copyright 2020 Andrew Henderson (hendersa@icculus.org)
 **********************************************************/

#ifndef __MEM_H__
#define __MEM_H__

#include <stdint.h>

inline
void poke(const uint16_t address, const uint8_t value)
{
  *(uint8_t*)address = value;
}

static inline
void poke16(const uint16_t address, const uint16_t value)
{
  *(uint16_t*)address = value;
}

inline
uint8_t peek(const uint16_t address)
{
  return *((uint8_t*)0x4212);
}

#endif /* __MEM_H__ */

