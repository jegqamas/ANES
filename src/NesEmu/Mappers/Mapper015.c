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

int temp;
void Mapper015_WritePR(unsigned short *address, unsigned char *value)
{
    switch ((*address) & 0x3)
    {
    case 0:
        MEM_SWITCH_PRG_16KB(PRG_AREA_8000, (*value) & 0x3F);
        MEM_SWITCH_PRG_16KB(PRG_AREA_C000,((*value) & 0x3F) | 1);
        break;
    case 1:
        MEM_SWITCH_PRG_16KB(PRG_AREA_8000,(*value) & 0x3F);
        MEM_SWITCH_PRG_16KB(PRG_AREA_C000,mem_prg_rom_16kb_mask);
        break;
    case 2:
        temp = (*value) << 1;
        temp = (((*value) & 0x3F) << 1) | (((*value) >> 7) & 1);
        MEM_SWITCH_PRG_08KB(PRG_AREA_8000,temp);
        MEM_SWITCH_PRG_08KB(PRG_AREA_A000,temp);
        MEM_SWITCH_PRG_08KB(PRG_AREA_C000,temp);
        MEM_SWITCH_PRG_08KB(PRG_AREA_E000,temp);
        break;

    case 3:
        MEM_SWITCH_PRG_16KB(PRG_AREA_8000,(*value) & 0x3F);
        MEM_SWITCH_PRG_16KB(PRG_AREA_C000,(*value) & 0x3F);
        break;

    }
    MEM_SWITCH_NMT_FROM_MIRORING(((*value) & 0x40) == 0x40 ? MIRRORING_Horz : MIRRORING_Vert);
}
