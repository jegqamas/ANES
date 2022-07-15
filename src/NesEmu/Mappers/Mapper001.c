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

int address_reg;
unsigned int reg[4];
unsigned int shift ;
unsigned int buffer ;
char flag_p;
char flag_c;
char flag_s;
char enable_wram_enable;
int prg_hijackedbit;
char use_hijacked;
char use_sram_switch;
int sram_switch_mask;
int cpuCycles;

void UpdateCHR()
{
    if (!flag_c)
        MEM_SWITCH_CHR_08KB(CHR_AREA_0000, reg[1] >> 1);
    else
    {
        MEM_SWITCH_CHR_04KB(CHR_AREA_0000,reg[1]);
        MEM_SWITCH_CHR_04KB(CHR_AREA_1000,reg[2]);
    }
    // SRAM
    if (use_sram_switch)
        MEM_SWITCH_PRG_08KB(PRG_AREA_6000,(reg[1] & sram_switch_mask) >> 3);
}
void UpdatePRG()
{
    if (!flag_p)
    {
        MEM_SWITCH_PRG_32KB(PRG_AREA_8000,((reg[3] & 0xF) | prg_hijackedbit) >> 1);
    }
    else
    {
        if (flag_s)
        {
            MEM_SWITCH_PRG_16KB(PRG_AREA_8000,(reg[3] & 0xF) | prg_hijackedbit);
            MEM_SWITCH_PRG_16KB(PRG_AREA_C000,0xF | prg_hijackedbit);
        }
        else
        {
            MEM_SWITCH_PRG_16KB(PRG_AREA_8000,prg_hijackedbit);
            MEM_SWITCH_PRG_16KB(PRG_AREA_C000,(reg[3] & 0xF) | prg_hijackedbit);
        }
    }
}
void Mapper001HardReset()
{
    Mapper000HardReset();

    cpuCycles = 0;
    // Registers
    address_reg = 0;
    reg[0] = 0x0C;
    flag_c = 0;
    flag_s = flag_p = 1;
    prg_hijackedbit = 0;
    reg[1] = reg[2] = reg[3] = 0;
    // Buffers
    buffer = 0;
    shift = 0;
    if (Mapper001_MMCB_Mode)
    {
        MEM_TOGGLE_PRG_08KB_RAM_ENABLE(PRG_AREA_6000,0);// make it disabled
    }
    enable_wram_enable = !Mapper001_MMCA_Mode;

    //  use hijacked
    use_hijacked = (mem_prg_rom_16kb_mask & 0x10) == 0x10;
    if (use_hijacked)
        prg_hijackedbit = 0x10;
    // SRAM Switch ?
    use_sram_switch = 0;
    if (Mapper001_UseSRAMSwitch)
    {
        use_sram_switch = 1;
        sram_switch_mask = use_hijacked ? 0x08 : 0x18;
        sram_switch_mask &= mem_prg_ram_08kb_mask << 3;

        if (sram_switch_mask == 0)
            use_sram_switch = 0;
    }
    MEM_SWITCH_PRG_16KB(PRG_AREA_C000,0xF | prg_hijackedbit);
}
void Mapper001_OnCPUClock()
{
    if (cpuCycles > 0)
        cpuCycles--;
}
void Mapper001_WritePR(unsigned short *address, unsigned char *value)
{
// Too close writes ignored !
    if (cpuCycles > 0)
    {
        return;
    }
    cpuCycles = 3;// Make save cycles ...
    //Temporary reg port ($8000-FFFF):
    //[r... ...d]
    //r = reset flag
    //d = data bit

    //r is set
    if (((*value)& 0x80) == 0x80)
    {
        reg[0] |= 0x0C;//bits 2,3 of reg $8000 are set (16k PRG mode, $8000 swappable)
        flag_s = flag_p = 1;
        shift = buffer = 0;//hidden temporary reg is reset
        return;
    }
    //d is set
    if (((*value) & 0x01) == 0x01)
        buffer |= (1 << shift);//'d' proceeds as the next bit written in the 5-bit sequence
    if (++shift < 5)
        return;
    // If this completes the 5-bit sequence:
    // - temporary reg is copied to actual internal reg (which reg depends on the last address written to)
    address_reg = ((*address) & 0x7FFF) >> 13;
    reg[address_reg] = buffer;

    // - temporary reg is reset (so that next write is the "first" write)
    shift = buffer = 0;

    // Update internal registers ...
    switch (address_reg)
    {
    case 0:// $8000-9FFF [Flags and mirroring]
    {
        // Flags
        flag_c = (reg[0] & 0x10) != 0;
        flag_p = (reg[0] & 0x08) != 0;
        flag_s = (reg[0] & 0x04) != 0;
        UpdatePRG();
        UpdateCHR();
        // Mirroring
        switch (reg[0] & 3)
        {
        case 0:
            MEM_SWITCH_NMT_FROM_MIRORING(MIRRORING_OneScA);
            break;
        case 1:
            MEM_SWITCH_NMT_FROM_MIRORING(MIRRORING_OneScB);
            break;
        case 2:
            MEM_SWITCH_NMT_FROM_MIRORING(MIRRORING_Vert);
            break;
        case 3:
            MEM_SWITCH_NMT_FROM_MIRORING(MIRRORING_Horz);
            break;
        }
        break;
    }
    case 1:// $A000-BFFF [CHR REG 0]
    {
        // CHR
        if (!flag_c)
            MEM_SWITCH_CHR_08KB(CHR_AREA_0000, reg[1] >> 1);
        else
            MEM_SWITCH_CHR_04KB(CHR_AREA_0000,reg[1]);
        // SRAM
        if (use_sram_switch)
            MEM_SWITCH_PRG_08KB(PRG_AREA_6000, (reg[1] & sram_switch_mask) >> 3);
        // PRG hijack
        if (use_hijacked)
        {
            prg_hijackedbit = reg[1] & 0x10;
            UpdatePRG();
        }
        break;
    }
    case 2:// $C000-DFFF [CHR REG 1]
    {
        // CHR
        if (flag_c)
            MEM_SWITCH_CHR_04KB(CHR_AREA_1000, reg[2]);
        // SRAM
        if (use_sram_switch)
            MEM_SWITCH_PRG_08KB(PRG_AREA_6000,(reg[2] & sram_switch_mask) >> 3);
        // PRG hijack
        if (use_hijacked)
        {
            prg_hijackedbit = reg[2] & 0x10;
            UpdatePRG();
        }
        break;
    }
    case 3:// $E000-FFFF [PRG REG]
    {
        if (enable_wram_enable)
            MEM_TOGGLE_PRG_08KB_RAM_ENABLE(PRG_AREA_6000,(reg[3] & 0x10) == 0);
        UpdatePRG();
        break;
    }
    }
}
