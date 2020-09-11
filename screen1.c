/**********************************************************
 * Main register screen functions (input, update, drawing).
 *
 * License: LGPL (Refer to COPYING file)
 * Copyright 2020 Andrew Henderson (hendersa@icculus.org)
 **********************************************************/

#include <stdint.h>
#include <stdio.h>
#include "snesc.h"
#include "graph.h"
#include "input.h"
#include "screen1.h"

static void readRegister(const uint8_t index);
static void writeRegister(const uint8_t index);
static void redrawAllRegisters(void);
static void redrawRegister(const uint8_t index);
static void redrawCurrentRegisterField(void);
static void redrawLabels(void);
static void redrawMarker(void);
static void toggleBit(void);

extern uint16_t blockmap[0x400];

#define LINE_BLOCKS 0x20

/* $2188 through $2199 */
#define TOTAL_REGS 18

static const char* LINE_BLANK = "                                ";

/* Current register that the marker is in. The indices are even on the left
 * column and odd on the right, starting with zero. They increment when you
 * move right or down. */
static uint8_t currentReg = 0;

/* Bit offet from MSB in current register */
static uint8_t currentBit = 0;

/* Using a struct didn't work because of memory alignment issues with the,
 * compiler, so there are now three arrays indexed by currentReg as needed: */

/* Array #1: Register addresses */
static uint16_t regAddress[TOTAL_REGS] = {
  0x2188, 0x218E, 0x2189, 0x218F, 0x218A, 0x2190, 0x218B, 0x2191, 0x218C,
  0x2192, 0x218D, 0x2193, 0x2194, 0x2197, 0x2195, 0x2196, 0x2198, 0x2199 };

/* Array #2: Map locations of the register bits on the screen */
static uint16_t regLocation[TOTAL_REGS] = {
  0x0A5, 0x0B4, 0x0E5, 0x0F4, 0x125, 0x134, 0x165, 0x174, 0x1A5,
  0x1B4, 0x1E5, 0x1F4, 0x265, 0x274, 0x2A5, 0x2B4, 0x2E5, 0x2F4 };

/* Array #3: Cached values of each register */
static uint8_t regValue[TOTAL_REGS] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

uint8_t screen1ProcessInput(const uint16_t input) {

  switch(input) {
    /* "Up" on D-Pad: Go up one register (or wrap to bottom) */
    case UP_BUTTON:
      if (currentReg >= 2) currentReg -= 2;
      else currentReg += (TOTAL_REGS - 2);
      break;

    /* "Down" on D-Pad: Go down one row (or wrap to top) */
    case DOWN_BUTTON:
      if (currentReg < (TOTAL_REGS - 2)) currentReg += 2;
      else currentReg -= (TOTAL_REGS - 2);
      break;

    /* "Left" or "Right" on D-Pad: Go one register left/right */
    case LEFT_BUTTON:
    case RIGHT_BUTTON:
      if (currentReg % 2) currentReg -= 1;
      else currentReg += 1;
      break;

    /* "Left Trigger" button: Go left one bit (or wrap to right) */
    case TL_BUTTON:
      if (currentBit == 0) currentBit = 7;
      else currentBit--;
      break;

    /* "Right trigger" button: Go right one bit (or wrap to left) */
    case TR_BUTTON:
      if (currentBit == 7) currentBit = 0;
      else currentBit++;
      break;

    /* "Select" button: Read all */
    case SELECT_BUTTON:
      screen1ReadAllRegisters();
      /* Redraw the entire screen because multiple regs may have changed */
      screen1Redraw();
      break;

    /* "Start" button: Bring up help screen */
    case START_BUTTON:
      return 1;

    /* "X" button: Read current register */
    case X_BUTTON:
      readRegister(currentReg);
      break;

    /* "Y" button: Write current register */
    case Y_BUTTON:
      writeRegister(currentReg);
      break;

    /* "B" button: Toggle current bit */
    case B_BUTTON:
      toggleBit();
      break;

    default:
      break;
  } /* End switch */

  screen1PartialRedraw();
  return 0;
}

/* Toggle a cached bit for one of the registers */
static void toggleBit(void) { 
  regValue[currentReg] ^= (1 << (7-currentBit) );
}

/* Update all cached register values with the current register values */
void screen1ReadAllRegisters(void) {
  uint8_t index;
  for (index=0; index < TOTAL_REGS; index++) {
    regValue[index] = *((uint8_t *)regAddress[index]);
  }
}

/* Update one cached register value with the current register value */
static void readRegister(const uint8_t index) {
  regValue[index] = *((uint8_t *)regAddress[index]);
}

/* Write one cached register value to the register */
static void writeRegister(const uint8_t index) {
  *(uint8_t *)(regAddress[index]) = regValue[index];
}

/* Redraw register bits in the screen */
static void redrawRegister(const uint8_t index) {
  char currentBits[9];
  uint8_t indexReg = index;
  uint8_t indexBit;
  uint8_t value;

  /* Null terminate the string */
  currentBits[8] = 0;

  /* Create the bit string for this register */
  value = regValue[indexReg];
  for (indexBit=0; indexBit < 8; indexBit++) {

    /* Set bit character in string */
    if (value & 1) currentBits[7-indexBit] = '1';
    else currentBits[7-indexBit] = '0';

    /* Shift over the register value one bit */
    value >>= 1;

  } /* End for */

  /* Print the bit string on the screen */
  writestring(currentBits, blockmap, regLocation[indexReg], 0x3F6);
}

/* Redraw all register bits on the screen */
static void redrawAllRegisters(void) {
  uint8_t indexReg;

  /* Sweep through all of the registers */
  for (indexReg=0; indexReg < TOTAL_REGS; indexReg++) {
    redrawRegister(indexReg);
  } /* End for */
}

/* Redraw just the bits of the current register */
static void redrawCurrentRegisterField(void) {
  char value[20];
  uint16_t currentValue = regValue[currentReg];
  if (currentValue <= 0x0F)
    sprintf(value, "%X (0%X)", regAddress[currentReg], currentValue);
  else
    sprintf(value, "%X (%X)", regAddress[currentReg], currentValue);

  writestring(value, blockmap, 0x333, 0x3F6);
}

/* Erase the previous marker and redraw it in the right position */
static void redrawMarker(void) {
  uint16_t markerLocation;
  uint16_t lineBase = 0xC0;

  /* Erase all of the potential marker spots */
  writestring(LINE_BLANK, blockmap, lineBase + 0x000, 0x3F6);
  writestring(LINE_BLANK, blockmap, lineBase + 0x040, 0x3F6);
  writestring(LINE_BLANK, blockmap, lineBase + 0x080, 0x3F6);
  writestring(LINE_BLANK, blockmap, lineBase + 0x0C0, 0x3F6);
  writestring(LINE_BLANK, blockmap, lineBase + 0x100, 0x3F6);
  writestring(LINE_BLANK, blockmap, lineBase + 0x140, 0x3F6);
  writestring(LINE_BLANK, blockmap, lineBase + 0x1C0, 0x3F6);
  writestring(LINE_BLANK, blockmap, lineBase + 0x200, 0x3F6);
  writestring(LINE_BLANK, blockmap, lineBase + 0x240, 0x3F6);

  /* Figure out the location of the marker */
  markerLocation = regLocation[currentReg] + currentBit + LINE_BLOCKS;

  /* Draw the marker */
  writestring("^", blockmap, markerLocation, 0x3F6);
}

/* Redraw all of the labels (static text) on the screen */
static void redrawLabels(void) {
  uint8_t line = 1;

  /* Header label */
  writestring(" SatellaView Register Test ROM", blockmap, (LINE_BLOCKS * line++), 0x3F6);

  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);

  /* Stream register labels */
  writestring("STREAM 1 REGS  STREAM 2 REGS   ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring("-------------  -------------   ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring("2188:????????  218E:???????? RW", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring("2189:????????  218F:???????? RW", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring("218A:????????  2190:???????? RO", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring("218B:????????  2191:???????? RW", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring("218C:????????  2192:???????? RW", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring("218D:????????  2193:???????? RO", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);

  /* Control register labels */
  writestring("   OTHER CONTROL REGISTERS     ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring("   -----------------------     ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring("2194:????????  2197:???????? RW", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring("2195:????????  2196:???????? RO", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring("2198:????????  2199:???????? RW", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);

  /* Footer labels */
  writestring("Current Register: $???? (??)   ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring("   Press [START] for help ", blockmap, (LINE_BLOCKS * line++), 0x3F6);
  writestring(LINE_BLANK, blockmap, (LINE_BLOCKS * line++), 0x3F6);
  setmap(0, (uint8_t*) blockmap);
}

/* Redraw the whole screen from scratch */
void screen1Redraw(void) {
  redrawLabels();
  redrawAllRegisters();
  screen1PartialRedraw();
}

/* Redraw just the current register and marker */
void screen1PartialRedraw(void) {
  redrawMarker();
  redrawRegister(currentReg);
  redrawCurrentRegisterField();
  setmap(0, (uint8_t*) blockmap);
}

