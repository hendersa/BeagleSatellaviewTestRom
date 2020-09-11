/**********************************************************
 * Modified input functions from SNES SDK.
 *
 * License: LGPL (Refer to COPYING file)
 * Copyright 2020 Andrew Henderson (hendersa@icculus.org)
 **********************************************************/

#include "input.h"
#include "init.h"

uint16_t getjoystatus(const uint16_t j)
{
  return snesc_controllers[j];
}

void clearjoy(const uint16_t j)
{
  snesc_controllers[j] = 0;
}

