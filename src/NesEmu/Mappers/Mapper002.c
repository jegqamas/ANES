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

void Mapper002HardReset()
{
    Mapper000HardReset();

    MEM_SWITCH_PRG_16KB(PRG_AREA_C000, mem_prg_rom_16kb_mask);
}
void Mapper002_WritePR(unsigned short *address, unsigned char *value)
{
    MEM_SWITCH_PRG_16KB(PRG_AREA_8000, (*value));
}
