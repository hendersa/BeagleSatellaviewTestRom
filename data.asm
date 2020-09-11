;*********************************************************
;* Modified data table file from SNES SDK.
;*
;* License: LGPL (Refer to COPYING file)
;* Copyright 2020 Andrew Henderson (hendersa@icculus.org)
;**********************************************************

.include "hdr.asm"

.section ".rodata" superfree

tiles1:
.incbin "data/tiles1.dat"
tiles2:
.incbin "data/tiles2.dat"
bg1map:
.incbin "data/bg1.map"
bg2map:
.incbin "data/bg2.map"
palette:
.incbin "data/palette.dat"
backpal:
.incbin "data/backpal.dat"

.ends
