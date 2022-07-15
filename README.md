# ANES (Agile Nes)
 A Nintendo Entertainment System / Family Computer (Nes/Famicom) Emulator written in C
 
### [Download Latest Release](https://github.com/jegqamas/ANES/releases/)

**Please note that this is the official repository of the program, that means official updates on source and releases will be commited/published here.**

ANES implements resolution upscaled method from "Resolution Blocks Upscaler" as described here: <https://github.com/jegqamas/Resolution-Blocks-Upscaler> 

ANES is based on My Nes, A Nintendo Entertainment System / Family Computer (Nes/Famicom) Emulator written in C#.

My Nes is lisenced under GNU General Public License 3 as well. Please see <https://github.com/jegqamas/My-Nes> for more details.

# Introduction

ANES is A Nintendo Entertainment System / Family Computer (Nes/Famicom) emulator written in C. 
An open source .net freeware, licensed under the GNU GENERAL PUBLIC LICENSE; Version 3, 29 June 2007.

The main goal of ANES is to be as accurate as possible, as fast as possible and brings the best gaming play experince possible.

ANES is written in C programming language and uses SDL2 for rendering video and audio and for input handling.
After tests, ANES  can run above 270 FPS in modern pcs with default settings (no commandline config).  

ANES is a pixel accurate emulator, as close as possible to real nes hardware.
ANES uses video resolution upscaling method allows to render video in full-hd resolution and even above.

ANES can pass most of (known at development time of current version) nes tests that test nes hardware behaviors, such as cpu 6502 instructions, 
ppu timing .... etc without any kind of work-arounds (changing some code to make a test pass), by emulating the exact hardware behavior.

ANES Able to decode nes colors without the need of using palette, includes gray scale and emphasize.
Expect real NES COLORS as they should be !!
Also a very good audio mixer is implemented. ANES implements the audio filters that is in real hardware (i.e. dac mixer which can be switched between built-in mixer and lookup table, low-pass and high-pass filters).

# ANES (Agile Nes) Emulation Specification

- CPU 6502: All CPU 6502 instructions implemented including the so called illegal opcodes.
  Exact interrupt timings like interrupt check before the last instruction behavior.
- APU: all Nes 5 sound channels, MMC5 and VRC6 external sound channels.
  Emulates the APU-CPU write/read behaviors (exact apu clock timing)
  Emulate the audio mixer that in real nes hardware (i.e. dac mixer which can be switched between built-in mixer and lookup table, low-pass and high-pass filters).
- PPU: Picture Processor Unit with exact timing as it is in nes real hardware (as close as possible).
  PPU vram-bus and io-bus are implemented as well.
- Colors: decode nes colors as close as possible to the real nes hardware, including gray scale and emphasize. ANES does not use palette indexes.
- TV Formats: NTSC.
- Video Output: Resolution can be upscaled from res 256 x 240 nes basic till 1920 x 1080 Full HD and even higher resolutions. 
- Sound Playback: playback frequency can be changed using command lines, set to 44100 Hz by default.
  
  Bit rate fixed to 16 bit, channels can be set to Mono or Stereo. Audio playback is set to Stereo by default.

  Supports Stereo Nes Mode which is a hack of nes that allows audio channels to outputed in stereo with pan adjusted.

- Boards/mappers: Please see SupportedMappers.txt/SupportedMappers file for more details.
- Controllers: 2 players joypads, each joypad is playable through Keyboard or XBox360 Game Controller (XInput). 
- Game file format: INES (with .nes extension).

# System Requirements And Installation

Usually ANES comes in portable package, which can be installed simply by extracting the content of that package 
anywhere in your machine.

In order to run ANES  correctly in your machine, please make sure that your machine meets up these requirements:

- ANES can run at any platform that can run SDL2 (see <https://wiki.libsdl.org/FrontPage>). "SDL officially supports Windows, Mac OS X, Linux, iOS, and Android"
- CPU: depending on confugration used, ANES can run even on slow pcs. For default settings, ANES run perfect on 2 GHz cpus.
- RAM: ANES usually uses about 60 to 80 MB ram. In other words, since operating system is running perfectly in your machine, 
  you should not worry about ram at all when using ANES for the exception of machines with 128 MB ram or less.
  
- For linux/other platforms users, SDL2 need to be installed first.

To install SDL2 in Ubuntu Linux 20 LTS (it is recommended to take a look here: <https://packages.ubuntu.com/search?keywords=sdl2>):

`sudo apt-get install libsdl2-2.0-0`


Installing dev packages also should work:

`sudo apt-get install libsdl2-dev`

Don't forget to give exe access permission to ANES file (ANES in home folder for example):

`chmod 777 ANES`


# How To Use

ANES is a commandline program, thus it needs commandlines to be configured and to run a game.

To run a game, a commandline like this should be used:

`ANES.exe <path-to-game>`

OR (using " will fix problem of spaces in file path)

`"<path-to-ANES.exe>" "<path-to-game>"`

`<path-to-game>: is the path of game file.`

In Windows (R) platform, ANES can be used simply by placing a shortcut of ANES.exe file, then the shortcut properties in the desktop can be edited to use the commands (in the "Target" field.)

For good quality running in Windows (R) platform:

`<path-to-ANES.exe> <path-to-game> -ures -wins_m_2 -direct3d -afreq_480`

This will run ANES using high res upscale with window size 1280 x 960, uses video driver Direct3D and Audio frequency 48000 Hz. Audio Channels are set to Stereo by default.

This will enable Stereo Nes Mode with window size x 2:

`<path-to-ANES.exe> <path-to-game> -astereones -wins_m_2`

Also, ANES can be used with emu-front-end, game launchers ...etc, a one tested with is Emulators Organizer: <https://github.com/jegqamas/Emulators-Organizer>.
Simply add ANES into EO profile as an emulator and ready to go with assigned console.
Comandlines can be added and managed using emulators and/or roms properties.

Running ANES without commandlines will run in standby mode (snow video output). 
If a file with name "game.nes" is placed next to ANES executable file, ANES will run that game.

For more details about commandlines and how to use them, see "Commandlines.txt/Commandlines" file.

For more details about shortcuts and controls, see "Shortcuts And Controls.txt/Shortcuts And Controls" file.

# Supported Mappers

Please note that this list may get updated from version to another.
I haven't tested all games on all supported mappers... if there is an issue please report in the issues page to update this list.

**Latest supported mappers list for ANES 1.1.310821:**


| Mapper Number | Board Name   | Status |
|---------------|--------------|--------|
| 000           | NROM         | OK     |
| 001           | MMC1         | OK     |
| 002           | UxROM        | OK     |
| 003           | CNROM        | OK     |
| 004           | MMC3         | OK     |
| 005           | MMC5         | Good   |
| 007           | AxROM        | OK     |
| 009           | MMC2         | OK     |
| 010           | MMC4         | OK     |
| 011           | Color Dreams | OK     |
| 013           | CPROM        | OK     |
| 015           | K-1029       | OK     |
| 018           | SS 88006     | OK     |
| 021           | VRC4a        | OK     |
| 022           | VRC2a        | OK     |
| 023           | VRC2b        | Good   |
| 024           | VRC6a        | OK     |
| 025           | VRC4b        | OK     |
| 026           | VRC6b        | OK     |
| 071           | Camerica     | OK     |
