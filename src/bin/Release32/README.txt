MY C NES
------
A Nintendo Entertainment System / Family Computer (Nes/Famicom) Emulator written in C.

This emulator is base on My Nes emulator with improved emulation engine.
My Nes emulator can be found here: <https://github.com/alaahadid/My-Nes>

Both My Nes and My C Nes are Written, Developed and Programmed by Alaa Ibrahim Hadid.

Copyright © Alaa Ibrahim Hadid 2021
All rights reserved.

Email: alaahadidfreewares@gmail.com

My C Nes Official Repository: <https://github.com/alaahadid/My-C-Nes>
My C Nes Official Wiki: <https://github.com/alaahadid/My-C-Nes/wiki>
My C Nes Contact Email: <mailto:alaahadidfreewares@gmail.com>

Copyright Notice
----------------
This program is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. 
If not, see <http://www.gnu.org/licenses/>.

NES is either a trademark or registered trademark of Nintendo of America Inc.
Famicom is either a trademark or registered trademark of Nintendo Co., LTD.
All other trademarks are property of their respective owners.
My Nes is not affiliated with or endorsed by any of the companies mentioned.

Introduction
------------
My C Nes is A Nintendo Entertainment System / Family Computer (Nes/Famicom) emulator written in C. 
An open source .net freeware, licensed under the GNU GENERAL PUBLIC LICENSE; Version 3, 29 June 2007.

The main goal of My Nes is to be as accurate as possible, as fast as possible and brings the best gaming play experince possible.

My C Nes is written in C programming language and uses SDL2 for rendering video and audio and for input handling.
After tests, My C Nes can run above 270 FPS in modern pcs with default settings (no commandline config).  

My C Nes is a pixel accurate emulator, as close as possible to real nes hardware.
My C Nes uses video resolution upscaling method allows to render video in full-hd resolution and even above.

My C Nes can pass all basic nes tests that test nes hardware behaviors, such as cpu 6502 instructions, 
ppu timing .... etc. However, My C Nes pass most of these tests (known at development time of current version) 
without any kind of work-arounds (changing some code to make a test pass), by emulating the exact hardware behavior.

My C Nes Able to decode nes colors without the need of using palette, includes gray scale and emphasize.
Expect real NES COLORS as they should be !!
Also the audio mixer that in real nes hardware is implemented (i.e. dac mixer, low-pass and high-pass filters).

My C Nes Emulation Specification
--------------------------------
- CPU 6502: All CPU 6502 instructions implemented including the so called illegal opcodes.
  Exact interrupt timings like interrupt check before the last instruction behavior.
- APU: all Nes 5 sound channels.
  Emulates the APU-CPU write/read behaviors (exact apu clock timing)
  Emulate the audio mixer that in real nes hardware(i.e. dac mixer, low-pass and high-pass filters).
- PPU: Picture Processor Unit with exact timing as it is in nes real hardware (as close as possible).
  PPU vram-bus and io-bus are implemented as well.
- Colors: decode nes colors as close as possible to the real nes hardware, including gray scale and emphasize. My C Nes does not use palette indexes.
- TV Formats: NTSC.
- Video Output: Resolution can be upscaled from res 256 x 240 nes basic till 1920 x 1080 Full HD and even higher resolutions. 
- Sound Playback: playback frequency can be changed using command lines, set to 44100 Hz by default.
  Bit rate fixed to 16 bit, channels fixed to Mono.
- Boards: NROM, MMC1, UxROM, MMC3, MMC5, AxROM and Camerica.
- Controllers: 2 players joypads, each joypad is playable through Keyboard or XBox360 Game Controller (XInput). 
- Game file format: INES (with .nes extension).

System Requirements And Installation
------------------------------------
Usually My C Nes comes in portable package, which can be installed simply by extracting the content of that package 
anywhere in your machine.

In order to run My Nes correctly in your machine, please make sure that your machine meets up these requirements:

- My Nes can run at any platform that can run SDL2 (see <https://wiki.libsdl.org/FrontPage>). "SDL officially supports Windows, Mac OS X, Linux, iOS, and Android"
- Latest C++ Runtime package from Microsoft. (OPTIONAL. IF THE EMU DOES NOT RUN IN WIN. Try the latest version of runtime first, 
  if My C Nes doensn't work, installing older version of this package may work.)
- CPU: depending on confugration used, My C Nes can run on slow pcs. For default settings, My C Nes run perfect on 2 GHz cpus.
- RAM: My Nes usually uses about 60 to 80 MB ram. In other words, since operating system is running perfectly in your machine, 
  you should not worry about ram at all when using My C Nes for the exception of machines with 128 MB ram or less.

How To Use
----------
My C Nes is a commandline program, thus it needs commandlines to be configured and to run a game.

To run a game, a commandline like this should be used:

MyCNes <path-to-game>

OR (using " will fix probmel of spaces in file path)

"<path-to-MyCNes>" "<path-to-game>"

<path-to-game>: is the path of game file.


Running My C Nes without commandlines will run in standby mode (snow video output). 
If a file with name "game.nes" is placed next to MyCNes executable file, MyCNes will run that game.

For more details about commandlines and how to use them, see "commandlines" file.

For more details about shortcuts and controls, see "shortcuts and controls" file.


END OF DOCUMENT

Written by Alaa Ibrahim Hadid.
For My C Nes version 1.0.
Last edit 27 May 2021 07:43
----------------------------
