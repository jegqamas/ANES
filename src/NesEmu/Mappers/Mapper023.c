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
#include "../NES.h"

int chr_Reg[8];
unsigned char security;

void Mapper023HardReset()
{
    Mapper000HardReset();

    MEM_SWITCH_PRG_16KB(PRG_AREA_C000, mem_prg_rom_16kb_mask);
    security = 0;
}
void Mapper023_WriteEX(unsigned short *address, unsigned char *value)
{
    if ((*address) == 0x6000)
        security = ((*value) & 0x1);
}
void Mapper023_ReadEX(unsigned short *address, unsigned char *value)
{
    if ((*address)< 0x6000)
        return;
    if ((*address) == 0x6000)
        (*value) = security;
    else
        (*value) = 0;
}
void Mapper023_WritePR(unsigned short *address, unsigned char *value)
{
    switch ((*address))
    {
    case 0x8000:
    case 0x8001:
    case 0x8002:
    case 0x8003:
        MEM_SWITCH_PRG_08KB(PRG_AREA_8000,(*value) & 0x0F);
        break;
    case 0x9000:
    case 0x9001:
    case 0x9002:
    case 0x9003:
    {
        switch ((*value) & 0x3)
        {
        case 0:
            MEM_SWITCH_NMT_FROM_MIRORING(MIRRORING_Vert);
            break;
        case 1:
            MEM_SWITCH_NMT_FROM_MIRORING(MIRRORING_Horz);
            break;
        case 2:
            MEM_SWITCH_NMT_FROM_MIRORING(MIRRORING_OneScA);
            break;
        case 3:
            MEM_SWITCH_NMT_FROM_MIRORING(MIRRORING_OneScB);
            break;
        }
        break;
    }
    case 0xA000:
    case 0xA001:
    case 0xA002:
    case 0xA003:
        MEM_SWITCH_PRG_08KB(PRG_AREA_A000,(*value) & 0x0F);
        break;
    case 0xB000:
        chr_Reg[0] = (chr_Reg[0] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_0000, chr_Reg[0]);
        break;
    case 0xB001:
        chr_Reg[0] = (chr_Reg[0] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_0000,chr_Reg[0]);
        break;
    case 0xB002:
        chr_Reg[1] = (chr_Reg[1] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_0400,chr_Reg[1]);
        break;
    case 0xB003:
        chr_Reg[1] = (chr_Reg[1] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_0400,chr_Reg[1]);
        break;
    case 0xC000:
        chr_Reg[2] = (chr_Reg[2] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_0800,chr_Reg[2]);
        break;
    case 0xC001:
        chr_Reg[2] = (chr_Reg[2] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_0800,chr_Reg[2]);
        break;
    case 0xC002:
        chr_Reg[3] = (chr_Reg[3] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_0C00,chr_Reg[3]);
        break;
    case 0xC003:
        chr_Reg[3] = (chr_Reg[3] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_0C00,chr_Reg[3]);
        break;
    case 0xD000:
        chr_Reg[4] = (chr_Reg[4] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_1000,chr_Reg[4]);
        break;
    case 0xD001:
        chr_Reg[4] = (chr_Reg[4] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_1000,chr_Reg[4]);
        break;
    case 0xD002:
        chr_Reg[5] = (chr_Reg[5] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_1400,chr_Reg[5]);
        break;
    case 0xD003:
        chr_Reg[5] = (chr_Reg[5] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_1400,chr_Reg[5]);
        break;
    case 0xE000:
        chr_Reg[6] = (chr_Reg[6] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_1800,chr_Reg[6]);
        break;
    case 0xE001:
        chr_Reg[6] = (chr_Reg[6] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_1800,chr_Reg[6]);
        break;
    case 0xE002:
        chr_Reg[7] = (chr_Reg[7] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_1C00,chr_Reg[7]);
        break;
    case 0xE003:
        chr_Reg[7] = (chr_Reg[7] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_1C00,chr_Reg[7]);
        break;
    }
}
