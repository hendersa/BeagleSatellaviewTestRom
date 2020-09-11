/**********************************************************
 * Modified string functions from SNES SDK.
 *
 * License: LGPL (Refer to COPYING file)
 * Copyright 2020 Andrew Henderson (hendersa@icculus.org)
 **********************************************************/

#include "snesc.h"

void writestring(const char* st, uint16_t* map, uint16_t p, uint16_t offset)
{
  uint16_t sp = p; /* start position */
  uint16_t c;

  while((c = *st))
  {
    if(c == '\n')
    {
      sp += 0x20;
      p = sp;
    }
    else
    {
      map[p] = c + offset;
      p++;
    }

    st++;
  }
}

void writenum(uint32_t num, uint8_t len, uint16_t* map, uint16_t p, uint16_t offset)
{
  uint8_t figure;
  p += len - 1;

  if(!num)
  {
    map[p] = offset;
  }
  else
    while(len && num)
    {
      figure = num % 10;

      if(num || figure)
        map[p] = figure + offset;

      num /= 10;
      p--;
      len--;
    }
}

