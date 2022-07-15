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

int prg_reg[3];
int chr_reg[8];
int irqRelaod;
int irqCounter;
char irqEnable;
int irqMask;
void Mapper018HardReset()
{
    Mapper000HardReset();
    MEM_SWITCH_PRG_08KB(PRG_AREA_E000, mem_prg_rom_08kb_mask);
}

// PR is between $8000 - $FFFF
void Mapper018_WritePR(unsigned short *address, unsigned char *value)
{
    switch ((*address) & 0xF003)
    {
    case 0x8000:
        prg_reg[0] = (prg_reg[0] & 0xF0) | ((*value) & 0x0F);/*****/
        MEM_SWITCH_PRG_08KB(PRG_AREA_8000, prg_reg[0]);
        break;
    case 0x8001:
        prg_reg[0] = (prg_reg[0] & 0x0F) | (((*value) & 0x0F) << 4);
        MEM_SWITCH_PRG_08KB(PRG_AREA_8000,prg_reg[0]);
        break;
    case 0x8002:
        prg_reg[1] = (prg_reg[1] & 0xF0) | ((*value) & 0x0F);/*****/
        MEM_SWITCH_PRG_08KB(PRG_AREA_A000, prg_reg[1]);
        break;
    case 0x8003:
        prg_reg[1] = (prg_reg[1] & 0x0F) | (((*value) & 0x0F) << 4);
        MEM_SWITCH_PRG_08KB(PRG_AREA_A000, prg_reg[1]);
        break;
    case 0x9000:
        prg_reg[2] = (prg_reg[2] & 0xF0) | ((*value) & 0x0F);/*****/
        MEM_SWITCH_PRG_08KB(PRG_AREA_C000, prg_reg[2]);
        break;
    case 0x9001:
        prg_reg[2] = (prg_reg[2] & 0x0F) | (((*value) & 0x0F) << 4);
        MEM_SWITCH_PRG_08KB(PRG_AREA_C000, prg_reg[2]);
        break;
    case 0xA000:
        chr_reg[0] = (chr_reg[0] & 0xF0) | ((*value) & 0x0F);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_0000, chr_reg[0]);
        break;
    case 0xA001:
        chr_reg[0] = (chr_reg[0] & 0x0F) | (((*value) & 0x0F) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_0000,chr_reg[0]);
        break;
    case 0xA002:
        chr_reg[1] = (chr_reg[1] & 0xF0) | ((*value) & 0x0F);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_0400,chr_reg[1]);
        break;
    case 0xA003:
        chr_reg[1] = (chr_reg[1] & 0x0F) | (((*value) & 0x0F) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_0400,chr_reg[1]);
        break;
    case 0xB000:
        chr_reg[2] = (chr_reg[2] & 0xF0) | ((*value) & 0x0F);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_0800,chr_reg[2]);
        break;
    case 0xB001:
        chr_reg[2] = (chr_reg[2] & 0x0F) | (((*value) & 0x0F) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_0800,chr_reg[2]);
        break;
    case 0xB002:
        chr_reg[3] = (chr_reg[3] & 0xF0) | ((*value) & 0x0F);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_0C00,chr_reg[3]);
        break;
    case 0xB003:
        chr_reg[3] = (chr_reg[3] & 0x0F) | (((*value) & 0x0F) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_0C00,chr_reg[3]);
        break;
    case 0xC000:
        chr_reg[4] = (chr_reg[4] & 0xF0) | ((*value) & 0x0F);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_1000,chr_reg[4]);
        break;
    case 0xC001:
        chr_reg[4] = (chr_reg[4] & 0x0F) | (((*value) & 0x0F) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_1000,chr_reg[4]);
        break;
    case 0xC002:
        chr_reg[5] = (chr_reg[5] & 0xF0) | ((*value) & 0x0F);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_1400,chr_reg[5]);
        break;
    case 0xC003:
        chr_reg[5] = (chr_reg[5] & 0x0F) | (((*value) & 0x0F) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_1400,chr_reg[5]);
        break;
    case 0xD000:
        chr_reg[6] = (chr_reg[6] & 0xF0) | ((*value) & 0x0F);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_1800,chr_reg[6]);
        break;
    case 0xD001:
        chr_reg[6] = (chr_reg[6] & 0x0F) | (((*value) & 0x0F) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_1800,chr_reg[6]);
        break;
    case 0xD002:
        chr_reg[7] = (chr_reg[7] & 0xF0) | ((*value) & 0x0F);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_1C00,chr_reg[7]);
        break;
    case 0xD003:
        chr_reg[7] = (chr_reg[7] & 0x0F) | (((*value) & 0x0F) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_1C00,chr_reg[7]);
        break;
    /*IRQ*/
    case 0xE000:
        irqRelaod = (irqRelaod & 0xFFF0) | ((*value) & 0x0F);
        break;
    case 0xE001:
        irqRelaod = (irqRelaod & 0xFF0F) | (((*value) & 0x0F) << 4);
        break;
    case 0xE002:
        irqRelaod = (irqRelaod & 0xF0FF) | (((*value) & 0x0F) << 8);
        break;
    case 0xE003:
        irqRelaod = (irqRelaod & 0x0FFF) | (((*value) & 0x0F) << 12);
        break;

    case 0xF000:
        irqCounter = irqRelaod;
        NES_IRQFlags &= IRQ_FLAG_BOARD_OFF;
        break;
    case 0xF001:
    {
        irqEnable = ((*value) & 1) == 1;
        if (((*value) & 0x8) == 0x8)
            irqMask = 0x000F;
        else if (((*value) & 0x4) == 0x4)
            irqMask = 0x00FF;
        else if (((*value) & 0x2) == 0x2)
            irqMask = 0x0FFF;
        else
            irqMask = 0xFFFF;
        NES_IRQFlags &= IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0xF002:
        switch ((*value) & 0x3)
        {
        case 0:
            MEM_SWITCH_NMT_FROM_MIRORING(MIRRORING_Horz);
            break;
        case 1:
            MEM_SWITCH_NMT_FROM_MIRORING(MIRRORING_Vert);
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
}
void Mapper018_OnCPUClock()
{
    if (irqEnable)
    {
        if (((irqCounter & irqMask) > 0) && ((--irqCounter & irqMask) == 0))
        {
            irqEnable = 0;
            NES_IRQFlags |= IRQ_FLAG_BOARD;
        }
    }
}
