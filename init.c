/**********************************************************
 * Modified initialization functions from SNES SDK.
 *
 * License: LGPL (Refer to COPYING file)
 * Copyright 2020 Andrew Henderson (hendersa@icculus.org)
 **********************************************************/

#include <stdint.h>
#include "snesc.h"
#include "mem.h"

extern void* __nmi_handler;

uint8_t snesc_do_copy;  /* bit 7: do sprite copy, bit 6: do palette copy
                           bits 0-5: DMA transfer count */
uint8_t snesc_timer_enabled; /* each bit one timer */
uint16_t snesc_timers[16];

struct oam_table1 snesc_oam_table1[128];    /* OAM low table */
uint16_t snesc_oam_table2[16];  /* OAM high table */

uint8_t snesc_palette[512];   /* palette */

uint16_t snesc_controllers[4];  /* data from 4 controllers */

struct dma_transfer snesc_dma_transfers[64]; /* DMA transfers
                                                0 (src_addr, src_bank) src address (24 bit)
                                                3 (dest) dest address (16 bit)
                                                5 (size) size (16 bit)
                                                7 (type) type (8 bit, 0 == VRAM, 1 == OAM, 2 == CGRAM, >2 == ???) */

void do_dma(const uint8_t do_flags)
{
  if(do_flags & 0x80)      /* copy sprites? */
  {
    *((uint16_t*)0x4300) = 0x400; /* DMA channel 0 Bus B addr $2104 (OAM write) */
    /* the original code loads $8000, turning on sprite priority and
       messing up the gfx (some emulators don't emulate this
       correctly, which is probably why it has slipped through) */
    *(uint16_t*)0x2102 = 0;   /* OAM address 0 */
    *((void**)0x4302) = snesc_oam_table1; /* DMA source address (24 bits); hairy: writes 32 bits... */
    *(uint16_t*)0x4305 = 0x220;   /* DMA size */
    *(uint8_t*)0x420b = 1;    /* enable DMA 0 */
  }

  if(do_flags & 0x40)      /* copy palette? */
  {
    *(uint16_t*)0x4300 = 0x2200;  /* DMA channel 0 Bus B addr $2122 (CGRAM write) */
    *((void**)0x4302) = snesc_palette;  /* DMA source address; see above */
    *(uint16_t*)0x4305 = 0x200;   /* DMA size */
    *(uint8_t*)0x2121 = 0;    /* CGRAM address 0 */
    *(uint8_t*)0x420b = 1;    /* enable DMA 0 */
  }
}

void snesc_vblank(void)
{
  /* stuff pending for DMA? */
  uint8_t do_flags = snesc_do_copy;

  if(do_flags)
  {
    snesc_do_copy = 0;
    do_dma(do_flags);
    do_flags &= 0x3f;   /* mask out palette and sprite flags (used by do_dma) */

    if(do_flags)
    {
      uint16_t x = 0;

      while(do_flags)
      {
        switch(snesc_dma_transfers[x].src.c.type)   /* type of transfer */
        {
        case 0: /* VRAM */
          poke16(0x4300, 0x1801); // 2 regs write once, Bus B addr $2118 (VRAM data write)
          poke16(0x2116, snesc_dma_transfers[x].dest); // VRAM address
          break;
        case 1: /* OAM */
          poke16(0x4300, 0x400); // DMA OAM write
          poke16(0x2102, snesc_dma_transfers[x].dest); // OAM address
          break;
        case 2: /* CGRAM */
          poke16(0x4300, 0x2200); // CGRAM write
          poke(0x2121, snesc_dma_transfers[x].dest); // CGRAM address (trunc'd to 8 bits)
          break;
        }

        *((uint16_t*)0x4302) = snesc_dma_transfers[x].src.c.addr; /* DMA source address (16-bit) */
        *((uint16_t*)0x4305) = snesc_dma_transfers[x].size;   /* DMA size */
        *((uint8_t*)0x4304) = snesc_dma_transfers[x].src.c.bank;  /* DMA source address (bank) */
        *((uint8_t*)0x420b) = 1;  /* enable DMA 0 */
        x++;
        do_flags--;
      }
    }
  }

  while(peek(0x4212) & 1)
  {
  }

  /* update input buffers */
  uint16_t pad;

  for(pad = 0; pad < 4; pad++)
  {
    snesc_controllers[pad] |= ((unsigned short*)0x4218)[pad];
  }

  /* timer ticks */
  uint8_t timers_enabled = snesc_timer_enabled;
  uint16_t tc = 0;

  while(timers_enabled & 1)
  {
    snesc_timers[tc]++;
    tc++;
    timers_enabled >>= 1;
  }
}

void snesc_init(void)
{
  uint16_t i;
  __nmi_handler = snesc_vblank; /* register vblank handler */
  *((uint8_t*)0x4200) = 0x81; /* enable NMI, enable autojoy */
  snesc_timer_enabled = snesc_do_copy = snesc_controllers[0] = 0;

  /* snesc sprite init stuff */
  for(i = 0; i < 128; i++)
  {
    snesc_oam_table1[i].x = 0;
    snesc_oam_table1[i].y = 0xe8;
    snesc_oam_table1[i].t = 0;
    snesc_oam_table1[i].p = 0;
  }

  for(i = 0; i < 32; i += 2)
  {
    snesc_oam_table2[i] = 0;
  }
}

