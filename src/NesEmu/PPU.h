// This file is part of ANES (Agile Nes)
//
// A Nintendo Entertainment System / Family Computer (Nes/Famicom)
// Emulator written in C.
//
// Copyright © Alaa Ibrahim Hadid 2021 - 2022
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.If not, see<http://www.gnu.org/licenses/>.
//
// Author email: mailto:alaahadidfreeware@gmail.com
//

char PPU_NMI_Current;
char PPU_NMI_Old;
char ppu_frame_finished;
char ppu_is_sprfetch;
unsigned char ppu_reg_2000_Sprite_size;

unsigned int ppu_screen_pixels[256 * 240];

void PPUInitialize();
void PPUSetupMapper(void (*on_ppu_clock)(),void (*on_ppu_scanline_tick)(),
                    void (*on_ppu_address_update)(unsigned short *address));
void PPUHardReset(int tv_format);
void PPUSoftReset();
void PPUShutdown();
void PPUClock();

void PPUScanlineRender();
void PPUScanlineVBLANKStart();
void PPUScanlineVBLANKEnd();
void PPUScanlineVBLANK();
void PPUHClock_000_Idle();
void PPUHClock_1_256_BKGClocks();
void PPUHClock_257_320_SPRClocks();
void PPUHClock_321_336_DUMClocks();
void PPUHClock_337_340_DUMClocks();

void RenderPixel();
char IsRenderingOn();
char IsInRender();

void PPU_CPUWrite(unsigned short *address, unsigned char *value);
void PPU_CPURead(unsigned short *address, unsigned char *value);

void PPUBKFetch0();
void PPUBKFetch1();
void PPUBKFetch2();
void PPUBKFetch3();
void PPUBKFetch4();
void PPUBKFetch5();
void PPUBKFetch6();
void PPUBKFetch7();

void PPUSPRFetch0();
void PPUSPRFetch1();
void PPUSPRFetch2();
void PPUSPRFetch3();

void PPUOamReset();
void PPUOamClear();
void PPUOamEvFetch();
void PPUOamPhase0();
void PPUOamPhase1();
void PPUOamPhase2();
void PPUOamPhase3();
void PPUOamPhase4();
void PPUOamPhase5();
void PPUOamPhase6();
void PPUOamPhase7();
void PPUOamPhase8();

void PPUOnRegister2000();
void PPUOnRegister2001();
void PPUOnRegister2002();
void PPUOnRegister2003();
void PPUOnRegister2004();
void PPUOnRegister2005();
void PPUOnRegister2006();
void PPUOnRegister2007();
void PPURead2000();
void PPURead2001();
void PPURead2002();
void PPURead2003();
void PPURead2004();
void PPURead2005();
void PPURead2006();
void PPURead2007();
