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

char prg_mode;
unsigned char prg_reg0;
int chr_Reg[8];
int irq_reload;
int irq_counter;
int prescaler;
char irq_mode_cycle;
char irq_enable;
char irq_enable_on_ak;

void Mapper025HardReset()
{
    Mapper000HardReset();
    MEM_SWITCH_PRG_16KB(PRG_AREA_C000, mem_prg_rom_16kb_mask);
    prescaler = 341;
}

// PR is between $8000 - $FFFF
void Mapper025_WritePR(unsigned short *address, unsigned char *value)
{

    switch ((*address))
    {
    case 0x8000:
    case 0x8002:
    case 0x8001:
    case 0x8003:
    case 0x8008:
    case 0x8004:
    case 0x800C:
    {
        prg_reg0 = (*value);
        MEM_SWITCH_PRG_08KB(PRG_AREA_8000, prg_mode ? (mem_prg_rom_08kb_mask - 1) : (prg_reg0 & 0x1F));
        MEM_SWITCH_PRG_08KB(PRG_AREA_C000, prg_mode ? (prg_reg0 & 0x1F) : (mem_prg_rom_08kb_mask - 1));
        break;
    }
    case 0x9000:
    case 0x9002:
    case 0x9008:
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
    case 0x9001:
    case 0x9003:
    case 0x9004:
    case 0x900C:
    {
        prg_mode = ((*value) & 0x2) == 0x2;
        MEM_SWITCH_PRG_08KB(PRG_AREA_8000, prg_mode ? (mem_prg_rom_08kb_mask - 1) : (prg_reg0 & 0x1F));
        MEM_SWITCH_PRG_08KB(PRG_AREA_C000, prg_mode ? (prg_reg0 & 0x1F) : (mem_prg_rom_08kb_mask - 1));
        break;
    }

    case 0xA000:
    case 0xA002:
    case 0xA001:
    case 0xA003:
    case 0xA008:
    case 0xA004:
    case 0xA00C:
    {
        MEM_SWITCH_PRG_08KB(PRG_AREA_A000, (*value) & 0x1F);
        break;
    }
    case 0xB000:
        chr_Reg[0] = (chr_Reg[0] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_0000, chr_Reg[0]);
        break;

    case 0xB002:
    case 0xB008:
        chr_Reg[0] = (chr_Reg[0] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_0000, chr_Reg[0]);
        break;

    case 0xB001:
    case 0xB004:
        chr_Reg[1] = (chr_Reg[1] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_0400, chr_Reg[1]);
        break;

    case 0xB003:
    case 0xB00C:
        chr_Reg[1] = (chr_Reg[1] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_0400, chr_Reg[1]);
        break;

    case 0xC000:
        chr_Reg[2] = (chr_Reg[2] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_0800, chr_Reg[2]);
        break;

    case 0xC002:
    case 0xC008:
        chr_Reg[2] = (chr_Reg[2] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_0800, chr_Reg[2]);
        break;

    case 0xC001:
    case 0xC004:
        chr_Reg[3] = (chr_Reg[3] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_0C00, chr_Reg[3]);
        break;

    case 0xC003:
    case 0xC00C:
        chr_Reg[3] = (chr_Reg[3] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_0C00, chr_Reg[3]);
        break;

    case 0xD000:
        chr_Reg[4] = (chr_Reg[4] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_1000, chr_Reg[4]);
        break;

    case 0xD002:
    case 0xD008:
        chr_Reg[4] = (chr_Reg[4] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_1000, chr_Reg[4]);
        break;

    case 0xD001:
    case 0xD004:
        chr_Reg[5] = (chr_Reg[5] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_1400, chr_Reg[5]);
        break;

    case 0xD003:
    case 0xD00C:
        chr_Reg[5] = (chr_Reg[5] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_1400, chr_Reg[5]);
        break;

    case 0xE000:
        chr_Reg[6] = (chr_Reg[6] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_1800, chr_Reg[6]);
        break;

    case 0xE002:
    case 0xE008:
        chr_Reg[6] = (chr_Reg[6] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_1800, chr_Reg[6]);
        break;

    case 0xE001:
    case 0xE004:
        chr_Reg[7] = (chr_Reg[7] & 0xF0) | ((*value) & 0xF);/*****/
        MEM_SWITCH_CHR_01KB(CHR_AREA_1C00, chr_Reg[7]);
        break;

    case 0xE003:
    case 0xE00C:
        chr_Reg[7] = (chr_Reg[7] & 0x0F) | (((*value) & 0xF) << 4);
        MEM_SWITCH_CHR_01KB(CHR_AREA_1C00, chr_Reg[7]);
        break;

    case 0xF000:
        irq_reload = (irq_reload & 0xF0) | ((*value) & 0xF);/*****/
        break;

    case 0xF002:
    case 0xF008:
        irq_reload = (irq_reload & 0x0F) | (((*value) & 0xF) << 4);
        break;

    case 0xF001:
    case 0xF004:
    {
        irq_mode_cycle = ((*value) & 0x4) == 0x4;
        irq_enable = ((*value) & 0x2) == 0x2;
        irq_enable_on_ak = ((*value) & 0x1) == 0x1;
        if (irq_enable)
        {
            irq_counter = irq_reload;
            prescaler = 341;
        }
        NES_IRQFlags &= IRQ_FLAG_BOARD_OFF;
        break;
    }

    case 0xF003:
    case 0xF00C:
        NES_IRQFlags &= IRQ_FLAG_BOARD_OFF;
        irq_enable = irq_enable_on_ak;
        break;
    }
}
void Mapper025_OnCPUClock()
{
    if (irq_enable)
    {
        if (!irq_mode_cycle)
        {
            if (prescaler > 0)
                prescaler -= 3;
            else
            {
                prescaler = 341;
                irq_counter++;
                if (irq_counter == 0xFF)
                {
                    NES_IRQFlags |= IRQ_FLAG_BOARD;
                    irq_counter = irq_reload;
                }
            }
        }
        else
        {
            irq_counter++;
            if (irq_counter == 0xFF)
            {
                NES_IRQFlags |= IRQ_FLAG_BOARD;
                irq_counter = irq_reload;
            }
        }
    }
}
