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
  poke(0x2101, 0); // Sprite register (size)
  poke(0x2102, 0); // Sprite registers (OAM address)
  poke(0x2103, 0); // Sprite registers (OAM address)
  poke(0x2105, 9); // BG mode 1
  poke(0x2106, 0); // No planes, no mosaic
  poke(0x2107, 0); // BG1 map location
  poke(0x2108, 0); // BG2 map location
  poke(0x2109, 4); // BG3 map location
  poke(0x210A, 0); // BG4 map location
  poke(0x210B, 1); // BG1/2 tiles location
  poke(0x210C, 0); // BG3/4 tiles location
  poke(0x210D, 0); // BG1 scroll X (11:3 bits)
  poke(0x210D, 0); // BG1 scroll x (2:0 bits)
  poke(0x210E, 0xFF); // BG1 scroll y (11:3 bits)
  poke(0x210E, 0x07); // BG1 scroll y (2:0 bits)
  poke(0x210F, 0); // BG2 scroll x (11:3 bits)
  poke(0x210F, 0); // BG2 scroll x (2:0 bits)
  poke(0x2110, 0xFF); // BG2 scroll y (11:3 bits)
  poke(0x2110, 0x07); // BG2 scroll y (2:0 bits)
  poke(0x2111, 0); // BG3 scroll x (11:3 bits)
  poke(0x2111, 0); // BG3 scroll x (2:0 bits)
  poke(0x2112, 0xFF); // BG3 scroll y (11:3 bits)
  poke(0x2112, 0x07); // BG3 scroll y (2:0 bits)
  poke(0x2113, 0); // BG4 scroll x (11:3 bits)
  poke(0x2113, 0); // BG4 scroll x (2:0 bits)
  poke(0x2114, 0xFF); // BG4 scroll y (11:3 bits)
  poke(0x2114, 0x07); // BG4 scroll y (2:0 bits)
  poke(0x2115, 0x80); // VRAM address increment register
  poke(0x2116, 0); // VRAM address low
  poke(0x2117, 0); // VRAM address high
  poke(0x212C, 0x01);
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

