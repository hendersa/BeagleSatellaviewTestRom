/**********************************************************
 * Main function, modified from SNES Joypad Test ROM.
 *
 * License: LGPL (Refer to COPYING file)
 * Copyright 2020 Andrew Henderson (hendersa@icculus.org)
 **********************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "snesc.h"
#include "graph.h"
#include "input.h"
#include "screen1.h"
#include "screen2.h"

extern uint8_t tiles1[];
extern uint8_t palette[];

uint16_t blockmap[0x400];
uint16_t pal[0x100];

enum {
  SCREEN_REGISTERS = 1,
  SCREEN_HELP = 2
};

static uint8_t currentScreen;

static uint8_t inputActive = 0;

void capture_inputs(void)
{
  uint16_t joyStatus = (getjoystatus(0) & 0xFFF0);
  clearjoy(0);

  /* No input, so reset our flag */
  if(!joyStatus && inputActive)
  {
    inputActive = 0;
    return;
  }

  /* Input is already active, so ignore new input until clear */
  if (inputActive) return;

  /* New input, so funnel it to the appropriate screen */
  if (joyStatus) {
    uint8_t result = 0;
    inputActive = 1;

    switch(currentScreen) {

      case SCREEN_REGISTERS:
        if(screen1ProcessInput(joyStatus)) {
	  currentScreen = SCREEN_HELP;
	  screen2Redraw();
	}
        break;

      case SCREEN_HELP:
        if (screen2ProcessInput(joyStatus)) {
	  currentScreen = SCREEN_REGISTERS;
	  screen1Redraw();
	}
        break;

      /* We're on an unknown screen! */
      default:
	currentScreen = SCREEN_HELP;
	screen2Redraw();
        break;
    } /* End switch */

  } /* End if */

}

void idle()
{
  resettimer();
  capture_inputs();
  sync(1);
}

int main()
{
  snesc_init();

  settiles(0, tiles1, 0xF00);
  memset(blockmap, 0x00, 0x800);
  memcpy(pal, palette, 0x200);

  screen1ReadAllRegisters();

  currentScreen = SCREEN_HELP;
  screen2Redraw();

  setpalette((uint8_t*)pal);

  enablescreen();

  setmap(0, (uint8_t*)blockmap);

  while(1)
    idle();
}
