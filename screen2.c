/**********************************************************
 * Help screen functions (input, update, drawing).
 *
 * License: LGPL (Refer to COPYING file)
 * Copyright 2020 Andrew Henderson (hendersa@icculus.org)
 **********************************************************/

#include <stdint.h>
#include <stdio.h>
#include "snesc.h"
#include "graph.h"
#include "input.h"
#include "screen2.h"

extern uint16_t blockmap[0x400];

#define LINE_BLOCKS 0x20

static const char* LINE_BLANK = "                               ";

uint8_t screen2ProcessInput(const uint16_t input) {

  switch(input) {
    case START_BUTTON:
      return 1;

    default:
      break;
  } /* End switch */

  return 0;
}

void screen2Redraw(void) {
  uint8_t line = 1;

  /* Header label (3 lines) */
  writestring(" SatellaView Register Test ROM ", blockmap, (LINE_BLOCKS * line++), 0x3F6);

  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);

  /* Stream register labels (13 lines) */
  writestring(" CONTROLS:                     ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring("                               ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(" * D-Pad selects register      ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(" * [L/R] selects register bit  ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(" * [B] toggles current bit     ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(" * [Y] writes current register ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(" * [X] reads current register  ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(" * [SELECT] reads all registers", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(" * [START] toggles help screen ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);

  writestring(" ------------------------------", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(" Press [START] to return to the", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring("  register interface screen    ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(" ------------------------------", blockmap, (LINE_BLOCKS * line++), 0x3F6);

  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);

  writestring(" This ROM is OSS (LGPL) & open ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(" content (CC BY-SA 3.0). Get   ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(" the source and content here:  ", blockmap, (LINE_BLOCKS * line++), 0x3F6);

  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(" https://www.beaglesatella.org ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring("     hendersa@icculus.org      ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  setmap(0, (uint8_t*) blockmap);
}

