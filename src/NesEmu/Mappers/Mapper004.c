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


char flag_c;
char flag_p;
int address_8001;
int chr_reg[6];
int prg_reg[4];
// IRQ
char irq_enabled;
unsigned char irq_counter;
int old_irq_counter;
unsigned char irq_reload;
char irq_clear;

int old_vram_address;
int new_vram_address;
int ppu_cycles_timer;
char ppuA12TogglesOnRaisingEdge;

void OnPPUA12RaisingEdge()
{
    old_irq_counter = irq_counter;

    if (irq_counter == 0 || irq_clear)
        irq_counter = irq_reload;
    else
        irq_counter = (irq_counter - 1) & 0xFF;

    if ((!Mapper004_MMC3A_Mode || old_irq_counter != 0 || irq_clear) && irq_counter == 0 && irq_enabled)
        NES_IRQFlags |= IRQ_FLAG_BOARD;

    irq_clear = 0;
}
void SetupPRG()
{
    MEM_SWITCH_PRG_08KB(PRG_AREA_8000,prg_reg[flag_p ? 2 : 0]);
    MEM_SWITCH_PRG_08KB(PRG_AREA_A000,prg_reg[1]);
    MEM_SWITCH_PRG_08KB(PRG_AREA_C000,prg_reg[flag_p ? 0 : 2]);
    MEM_SWITCH_PRG_08KB(PRG_AREA_E000,prg_reg[3]);
}
void SetupCHR()
{
    if (!flag_c)
    {
        MEM_SWITCH_CHR_02KB(CHR_AREA_0000,chr_reg[0]>>1);
        MEM_SWITCH_CHR_02KB(CHR_AREA_0800,chr_reg[1]>>1);

        MEM_SWITCH_CHR_01KB(CHR_AREA_1000,chr_reg[2]);
        MEM_SWITCH_CHR_01KB(CHR_AREA_1400,chr_reg[3]);
        MEM_SWITCH_CHR_01KB(CHR_AREA_1800,chr_reg[4]);
        MEM_SWITCH_CHR_01KB(CHR_AREA_1C00,chr_reg[5]);
    }
    else
    {
        MEM_SWITCH_CHR_02KB(CHR_AREA_1000,chr_reg[0]>>1);
        MEM_SWITCH_CHR_02KB(CHR_AREA_1800,chr_reg[1]>>1);

        MEM_SWITCH_CHR_01KB(CHR_AREA_0000,chr_reg[2]);
        MEM_SWITCH_CHR_01KB(CHR_AREA_0400,chr_reg[3]);
        MEM_SWITCH_CHR_01KB(CHR_AREA_0800,chr_reg[4]);
        MEM_SWITCH_CHR_01KB(CHR_AREA_0C00,chr_reg[5]);
    }
}
void Mapper004HardReset()
{
    Mapper000HardReset();
// Flags
    flag_c = flag_p = 0;
    address_8001 = 0;

    prg_reg[0] = 0;
    prg_reg[1] = 1;
    prg_reg[2] = mem_prg_rom_08kb_mask-1;
    prg_reg[3] = mem_prg_rom_08kb_mask;
    SetupPRG();

    // CHR
    for (int i = 0; i < 6; i++)
        chr_reg[i] = 0;

    // IRQ
    irq_enabled = 0;
    irq_counter = 0;
    irq_reload = 0xFF;
    old_irq_counter = 0;
    irq_clear = 0;
    ppuA12TogglesOnRaisingEdge=1;

    printf("Mapper 004 hard reset success \n");
}
void Mapper004_WritePR(unsigned short *address, unsigned char *value)
{
    switch ((*address) & 0xE001)
    {
    case 0x8000:
    {
        address_8001 = (*value) & 0x7;
        flag_c = ((*value)  & 0x80) != 0;
        flag_p = ((*value)  & 0x40) != 0;
        SetupCHR();
        SetupPRG();
        break;
    }
    case 0x8001:
    {
        switch (address_8001)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        {
            chr_reg[address_8001] = (*value);

            SetupCHR();
            break;
        }

        case 6:
        case 7:
        {
            prg_reg[address_8001 - 6] = (*value) & mem_prg_rom_08kb_mask;

            SetupPRG();
            break;
        }
        }
        break;
    }
    case 0xA000:
    {
        if (NES_Mirroring != MIRRORING_Full)
        {
            MEM_SWITCH_NMT_FROM_MIRORING(((*value)  & 1) == 1 ? MIRRORING_Horz : MIRRORING_Vert);
        }
        break;
    }
    case 0xA001:
    {
        MEM_TOGGLE_PRG_08KB_RAM_ENABLE(PRG_AREA_6000,((*value)  & 0x80) != 0);// make it enabled
        MEM_TOGGLE_PRG_08KB_RAM_WRITABLE(PRG_AREA_6000,((*value)  & 0x40) == 0);// make it writable
        break;
    }
    case 0xC000:
        irq_reload = (*value) ;
        break;
    case 0xC001:
        if (Mapper004_MMC3A_Mode)
            irq_clear = 1;
        irq_counter = 0;
        break;
    case 0xE000:
        irq_enabled = 0;
        NES_IRQFlags &= IRQ_FLAG_BOARD_OFF;
        break;
    case 0xE001:
        irq_enabled = 1;
        break;
    }
}
void Mapper004_OnPPUClock()
{
    ppu_cycles_timer++;
}
void Mapper004_OnPPUAddressUpdate(unsigned short *address)
{
    old_vram_address = new_vram_address;
    new_vram_address = (*address) & 0x1000;
    if (ppuA12TogglesOnRaisingEdge)
    {
        if (old_vram_address < new_vram_address)
        {
            if (ppu_cycles_timer > 8)
            {
                OnPPUA12RaisingEdge();
            }
            ppu_cycles_timer = 0;
        }
    }
    else
    {
        if (old_vram_address > new_vram_address)
        {
            if (ppu_cycles_timer > 8)
            {
                OnPPUA12RaisingEdge();
            }
            ppu_cycles_timer = 0;
        }
    }
}
