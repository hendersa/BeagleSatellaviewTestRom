/**********************************************************
 * Modified SNESC functions from SNES SDK.
 *
 * License: LGPL (Refer to COPYING file)
 * Copyright 2020 Andrew Henderson (hendersa@icculus.org)
 **********************************************************/

#ifndef __SNESC_H__
#define __SNESC_H__

#include <stdint.h>

/* init */
void snesc_init(void);

/* string */
void writenum(uint32_t num, uint8_t len, uint16_t* map, uint16_t p, uint16_t offset);
void writestring(const char* st, uint16_t* map, uint16_t p, uint16_t offset);

extern uint8_t snesc_timer_enabled;
extern uint16_t snesc_timers[16];
extern uint8_t snesc_do_copy;

struct oam_table1
{
  uint8_t x;
  uint8_t y;
  uint8_t t;
  uint8_t p;
};

extern struct oam_table1 snesc_oam_table1[128];
extern uint16_t snesc_oam_table2[32];
extern uint8_t snesc_palette[512];

struct dma_transfer
{
  union
  {
    struct
    {
      uint16_t addr; /* 0 + 1 */
      uint8_t bank; /* 2 */
      uint8_t type;
    } c;
    uint8_t* ptr;
  } src;
  uint16_t dest; /* 3 + 4 */
  uint16_t size; /* 5 + 6 */
};

extern struct dma_transfer snesc_dma_transfers[64];

#endif /* __SNESC_H__ */

