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

#include "_Mappers.h"
#include "../Memory.h"
#include "../DIFINES.h"
#include "../PPU.h"

unsigned char chr_reg0A;
unsigned char chr_reg0B;
unsigned char chr_reg1A;
unsigned char chr_reg1B;
unsigned char latch_a;
unsigned char latch_b;

void Mapper009HardReset()
{
    latch_a = 0xFE;
    latch_b = 0xFE;

    Mapper000HardReset();

    MEM_SWITCH_PRG_08KB(PRG_AREA_A000, mem_prg_rom_08kb_mask - 2);
    MEM_SWITCH_PRG_08KB(PRG_AREA_C000, mem_prg_rom_08kb_mask - 1);
    MEM_SWITCH_PRG_08KB(PRG_AREA_E000, mem_prg_rom_08kb_mask);

    chr_reg0B = 4;

}
void Mapper009_WritePR(unsigned short *address, unsigned char *value)
{
    switch ((*address) & 0xF000)
    {
    case 0xA000:
    {
        MEM_SWITCH_PRG_08KB(PRG_AREA_8000, (*value));
        break;
    }
    case 0xB000:
    {
        chr_reg0A =  (*value);
        if (latch_a == 0xFD)
            MEM_SWITCH_CHR_04KB(CHR_AREA_0000, chr_reg0A);
        break;
    }
    case 0xC000:
    {
        chr_reg0B =  (*value);
        if (latch_a == 0xFE)
            MEM_SWITCH_CHR_04KB(CHR_AREA_0000,chr_reg0B);
        break;
    }
    case 0xD000:
    {
        chr_reg1A =  (*value);
        if (latch_b == 0xFD)
            MEM_SWITCH_CHR_04KB(CHR_AREA_1000,chr_reg1A);
        break;
    }
    case 0xE000:
    {
        chr_reg1B =  (*value);
        if (latch_b == 0xFE)
            MEM_SWITCH_CHR_04KB(CHR_AREA_1000,chr_reg1B);
        break;
    }
    case 0xF000:
    {
        MEM_SWITCH_NMT_FROM_MIRORING(( (*value) & 0x1) == 0x1 ? MIRRORING_Horz : MIRRORING_Vert);
        break;
    }
    }
}
void Mapper009_ReadCHR(unsigned short *address, unsigned char *value)
{
    if ((*address) == 0x0FD8 == 0x0FD8 && latch_a != 0xFD)
    {
        latch_a = 0xFD;
        MEM_SWITCH_CHR_04KB(CHR_AREA_0000, chr_reg0A);
    }
    else if ((*address) == 0x0FE8 == 0x0FE8 && latch_a != 0xFE)
    {
        latch_a = 0xFE;
        MEM_SWITCH_CHR_04KB(CHR_AREA_0000,chr_reg0B);
    }
    else if (((*address) >= 0x1FD8 && (*address) <= 0x1FDF) && latch_b != 0xFD)
    {
        latch_b = 0xFD;
        MEM_SWITCH_CHR_04KB(CHR_AREA_1000,chr_reg1A);
    }
    else if (((*address) >= 0x1FE8 && (*address) <= 0x1FEF) && latch_b != 0xFE)
    {
        latch_b = 0xFE;
        MEM_SWITCH_CHR_04KB(CHR_AREA_1000,chr_reg1B);
    }
}
