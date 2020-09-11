This is the source code and makefile to build the BeagleSatella SatellaView
test ROM (BeagleSatellaTest.sfc). The purpose of this ROM is to provide a UI
for reading/writing the address range of $2188 to $2199 of a Super Famicom or 
Super Nintendo. This range is the memory-mapped IO interface for the registers 
of the SatellaView satellite modem peripheral. The ROM can be executed under an
SNES/SFC emulator or on real hardware via a flash cartridge.

A built ROM is located in the "prebuilt" directory for convenience.

*** Installing the toolchain ***

The code is built using the toolchain provided in Optixx's SNES SDK: 

- 816-tcc compiler
- wla-65816 assembler
- wlalink linker

In particular, the ROM's makefile assumes that you will use Ace17's fork of the
SNES SDK, which installs the toolchain in /tmp/snes-sdk:

https://github.com/Ace17/snes-sdk.git

Download, build, and install the SDK. Be aware that your /tmp directory will
most likely be cleaned out on your next boot, so you'll need to reinstall the
SDK if you reboot and then want to build the ROM again. 

*** Building the ROM ***

With the toolchain installed, change into the source directory for the ROM and 
build it by running "make". The BeagleSatellaTest.sfc ROM will build and be 
copied into the "bin" directory. The ROM is suitable for execution under an
emulator or on real hardware.

*** Running under an emulator ***

Simply use your SNES emulator of choice to run the ROM. Be aware that your 
emulator may not enable/support the Satellaview registers if it detects that
the ROM you are running is not the BS-X BIOS/cartridge ROM. Without enabling
Satellaview support, reads or writes to the Satellaview registers won't be 
correct. In addition, not all of the registers in $2188-$2199 may be mapped
into the emulator's memory space.
 
*** Running on real hardware ***

If you want to run the ROM on real hardware, your flash cart may require you to
pad the rom so that it is 4 megabits in size. The default ROM is built as a 2 
megabit image. To pad the ROM, use the uCON64 utility, which you can get from 
here:

https://ucon64.sourceforge.io

To pad the ROM, execute the following command twice (as each run pads by 1Mb):

$ ucon64 --pad --nbs --chk ./ROMNAME.sfc

*** Credits ***

The source code for this project is built upon two sources. The first is the 
example source code included in the SNES SDK. The second is the Joypad test
ROM (which is also derived from the SDK example code):

https://github.com/Modrigue/SNES_Joypad_Test_Rom 

Many of the changes that were made to these were to simplify the code and
change to using stdint.h datatypes.

*** Licenses ***

All ROM source code within this project inherits the Lesser GPL (LGPL) license
from the original SNES SDK. Likewise, all content (graphics, palettes, etc.)
located in the "data" directory inherit the Creative Commons Attribution 
ShareAlike 3.0 Unported (CC BY-SA 3.0) license from the original SNES SDK.  
 
*** Contact ***

This ROM was written by Andrew Henderson (hendersa@icculus.org) and is part of
the BeagleSatella project. BeagleSatella is an effort to develop a 
software/hardware platform that can interface with SNES/SFC consoles as a 
external peripheral. Learn more about BeagleSatella here:

https://www.beaglesatella.org

