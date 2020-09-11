/**********************************************************
 * Modified graphics functions from SNES SDK.
 *
 * License: LGPL (Refer to COPYING file)
 * Copyright 2020 Andrew Henderson (hendersa@icculus.org)
 **********************************************************/

#include <string.h>
#include "graph.h"
#include "snesc.h"
#include "mem.h"

void snesc_vblank(void);

void enablescreen(void)
{
  poke(0x2105, 9); // BG mode 1
  poke(0x2109, 4); // BG3 map location
  poke(0x210b, 1); // BG1/2 tiles location
  poke(0x210c, 2); // BG3/4 tiles location
  poke(0x212c, 0x15); // enable BG1, BG3, OBJ display
  poke(0x2100, 0x80); // force blank
  snesc_vblank();   // flush pending DMA jobs before turning on screen
  poke(0x2100, 0xf); // turn on screen, full brightness
}

void screenmode(const uint8_t m)
{
  poke(0x2105, m);
}

void waitforvsync(void)
{
  snesc_timer_enabled |= 1;
  snesc_timers[0] = 0;

  while(!snesc_timers[0])
  {
  }
}

void delay(const uint16_t d)
{
  snesc_timer_enabled |= 1;
  snesc_timers[0] = 0;

  while(snesc_timers[0] < d)
  {
  }
}

void setpalette(const uint8_t* pal)
{
  memcpy(snesc_palette, pal, 0x200);
  snesc_do_copy |= 0x40;
}

void setsprite(const uint16_t s, const uint8_t x, const uint8_t y, const uint8_t t, const uint8_t p)
{
  struct oam_table1* spr = &snesc_oam_table1[s];
  spr->x = x;
  spr->y = y;
  spr->t = t;
  spr->p = p;
  snesc_do_copy |= 0x80;
}

void sync(const uint16_t d)
{
  while(snesc_timers[0] < d)
  {
  }
}

void resettimer(void)
{
  snesc_timer_enabled |= 1;
  snesc_timers[0] = 0;
}

void settiles(const uint16_t b, uint8_t* p1, const uint16_t size)
{
  uint16_t idx = snesc_do_copy & 0x3f;
  struct dma_transfer* tr = &snesc_dma_transfers[idx];
  /* tile data */
  tr->src.ptr = p1;
  tr->src.c.type = 0;   /* src.ptr and type overlap, so type must be set after */
  tr->dest = (b + 1) << 12;
  tr->size = size;

  /* signal the NMI to copy data to VRAM */
  snesc_do_copy++;
}

void setmap(const uint16_t b, uint8_t* p1)
{
  struct dma_transfer* tr = &snesc_dma_transfers[snesc_do_copy & 0x3f];
  /* tile data */
  tr->src.ptr = p1;
  tr->src.c.type = 0;
  tr->dest = b << 10;
  tr->size = 0x800;

  /* signal the NMI to copy data to VRAM */
  snesc_do_copy++;
}

