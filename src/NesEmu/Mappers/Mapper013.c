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

unsigned char  data_temp;

void Mapper013HardReset()
{
    Mapper000HardReset();

    MEM_SWITCH_CHR_08KB_RAM(CHR_AREA_0000,1);
    MEM_SWITCH_CHR_08KB(CHR_AREA_0000,0);
}
void Mapper013_WritePR(unsigned short *address, unsigned char *value)
{
    // Bus conflicts !!
    MEM_CPUReadPRG(address, &data_temp);
    data_temp &= (*value);

    MEM_SWITCH_CHR_04KB(CHR_AREA_1000, data_temp&0x3);
}
