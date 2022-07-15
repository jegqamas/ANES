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
#include "../PPU.h"
#include "../APU.h"
#include "../ExternalSoundChannels/SC_MMC5_PCM.h"
#include "../ExternalSoundChannels/SC_MMC5_Pulse1.h"
#include "../ExternalSoundChannels/SC_MMC5_Pulse2.h"

// TODO: split screen and Uchuu Keibitai SDF chr corruption for unknown reason in the intro (not in the split screen).
int ram_protectA;
int ram_protectB;
int ExRAM_mode;
int CHROffset_spr[8];// The indexes to use with spr in spr fetches.
int CHROffsetEX[8];// For extra attr
int CHROffsetSP[8];// For split screen
int chrRegA[8];
int chrRegB[4];

int prgReg[4];
char useSRAMmirroring;
int chr_high;
int chr_mode;
int prg_mode;
char chr_setB_last;
unsigned char temp_val;
unsigned char temp_fill;
int lastAccessVRAM;
int paletteNo;
int shift;
int EXtilenumber;
unsigned char multiplicand;
unsigned char multiplier;
unsigned short product;
char split_enable;
char split_right;
int split_tile;
int split_yscroll;// The y scroll value for split.
char split_doit;// Set to true to make nt changes; get split happening. Used in CHR read access.
int split_watch_tile;// A temp tile number, for the split.
unsigned char irq_line;
unsigned char irq_enable;
int irq_pending;
int irq_current_counter;
int irq_current_inframe;
// External sound channels values
double mmc5mix_channels_out[3];

void Mapper005HardReset()
{
    Mapper000HardReset();

    prgReg[3] = mem_prg_rom_08kb_mask;
    prg_mode = 3;
    MEM_SWITCH_PRG_08KB(PRG_AREA_8000,mem_prg_rom_08kb_mask);
    MEM_SWITCH_PRG_08KB(PRG_AREA_A000,mem_prg_rom_08kb_mask);
    MEM_SWITCH_PRG_08KB(PRG_AREA_C000,mem_prg_rom_08kb_mask);
    MEM_SWITCH_PRG_08KB(PRG_AREA_E000,mem_prg_rom_08kb_mask);

    Switch04kCHREX(0, 0x0000);
    Switch04kCHRSP(0, 0x0000);
    Switch08kCHR_spr(0);

    SC_MMC5_PCMHardReset();
    SC_MMC5_Pulse1HardReset();
    SC_MMC5_Pulse2HardReset();
}

void Mapper005_WriteEX(unsigned short *address, unsigned char *value)
{
    if ((*address) < 0x5000)
        return;
    if ((*address) >= 0x6000)
        return;
    if ((*address) >= 0x5C00)
    {
        if (ExRAM_mode == 2)// Only EX2 is writable.
            mem_nmt_ram_banks[2][(*address) & 0x3FF] = (*value);
        else if (ExRAM_mode < 2)
        {
            if (irq_current_inframe == 0x40)
                mem_nmt_ram_banks[2][(*address) & 0x3FF] = (*value);
            else
                mem_nmt_ram_banks[2][(*address) & 0x3FF] = 0;
        }
        return;
    }
    switch ((*address))
    {
    case 0x5000:
        SC_MMC5_Pulse1Write5000(value);
        break;
    case 0x5002:
        SC_MMC5_Pulse1Write5002(value);
        break;
    case 0x5003:
        SC_MMC5_Pulse1Write5003(value);
        break;
    case 0x5004:
        SC_MMC5_Pulse2Write5004(value);
        break;
    case 0x5006:
        SC_MMC5_Pulse2Write5006(value);
        break;
    case 0x5007:
        SC_MMC5_Pulse2Write5007(value);
        break;
    case 0x5010:
        SC_MMC5_PCMWrite5010(value);
        break;
    case 0x5011:
        SC_MMC5_PCMWrite5011(value);
        break;
    case 0x5015:
    {
        SC_MMC5_Pulse1SetEnable(((*value) & 0x1) != 0);
        SC_MMC5_Pulse2SetEnable(((*value) & 0x2) != 0);
        break;
    }
    case 0x5100:
        prg_mode = (*value) & 0x3;
        break;
    case 0x5101:
        chr_mode = (*value) & 0x3;
        break;
    case 0x5102:
        ram_protectA = (*value) & 0x3;
        UpdateRamProtect();
        break;
    case 0x5103:
        ram_protectB = (*value) & 0x3;
        UpdateRamProtect();
        break;
    case 0x5104:
        ExRAM_mode = (*value) & 0x3;
        break;
    case 0x5105:
        MEM_SWITCH_NMT_FROM_MIRORING((*value));
        break;

    case 0x5113:
    {
        if (!Mapper005_UseSRAMMirroring)
            MEM_SWITCH_PRG_08KB(PRG_AREA_6000, (*value) & 0x7);
        else// Use chip switching (bit 2)...
            MEM_SWITCH_PRG_08KB(PRG_AREA_6000,((*value) >> 2) & 1);
        break;
    }
    case 0x5114:
    {
        if (prg_mode == 3)
        {
            MEM_SWITCH_PRG_08KB_RAM(PRG_AREA_8000, ((*value) & 0x80) == 0);
            MEM_SWITCH_PRG_08KB(PRG_AREA_8000,(*value) & 0x7F);
        }
        break;
    }
    case 0x5115:
    {
        switch (prg_mode)
        {
        case 1:
            MEM_SWITCH_PRG_16KB_RAM(PRG_AREA_8000,((*value) & 0x80) == 0);
            MEM_SWITCH_PRG_16KB(PRG_AREA_8000,((*value) & 0x7F) >> 1);
            break;
        case 2:
            MEM_SWITCH_PRG_16KB_RAM(PRG_AREA_8000,((*value) & 0x80) == 0);
            MEM_SWITCH_PRG_16KB(PRG_AREA_8000,((*value) & 0x7F) >> 1);
            break;
        case 3:
            MEM_SWITCH_PRG_08KB_RAM(PRG_AREA_A000,((*value) & 0x80) == 0);
            MEM_SWITCH_PRG_08KB(PRG_AREA_A000,(*value) & 0x7F);
            break;
        }
        break;
    }
    case 0x5116:
    {
        switch (prg_mode)
        {
        case 2:
        case 3:
            MEM_SWITCH_PRG_08KB_RAM(PRG_AREA_C000,((*value) & 0x80) == 0);
            MEM_SWITCH_PRG_08KB(PRG_AREA_C000,(*value) & 0x7F);
            break;
        }
        break;
    }
    case 0x5117:
    {

        switch (prg_mode)
        {
        case 0:
            MEM_SWITCH_PRG_32KB_RAM(PRG_AREA_8000, 0);
            MEM_SWITCH_PRG_32KB(PRG_AREA_8000,((*value) & 0x7C) >> 2);
            break;
        case 1:
            MEM_SWITCH_PRG_16KB_RAM(PRG_AREA_C000, 0);
            MEM_SWITCH_PRG_16KB(PRG_AREA_C000,((*value) & 0x7F) >> 1);
            break;
        case 2:
            MEM_SWITCH_PRG_08KB_RAM(PRG_AREA_E000, 0);
            MEM_SWITCH_PRG_08KB(PRG_AREA_E000,(*value) & 0x7F);
            break;
        case 3:
            MEM_SWITCH_PRG_08KB_RAM(PRG_AREA_E000, 0);
            MEM_SWITCH_PRG_08KB(PRG_AREA_E000,(*value) & 0x7F);
            break;
        }
        break;
    }

    // SPR SET
    case 0x5120:
    {
        chr_setB_last = 0;
        if (chr_mode == 3)
            Switch01kCHR_spr((*value) | chr_high, 0x0000);
        break;
    }
    case 0x5121:
    {
        chr_setB_last = 0;
        switch (chr_mode)
        {
        case 2:
            Switch02kCHR_spr((*value) | chr_high, 0x0000);
            break;
        case 3:
            Switch01kCHR_spr((*value) | chr_high, 0x0400);
            break;
        }
        break;
    }
    case 0x5122:
    {
        chr_setB_last = 0;
        if (chr_mode == 3)
            Switch01kCHR_spr((*value) | chr_high, 0x0800);
        break;
    }
    case 0x5123:
    {
        chr_setB_last = 0;
        switch (chr_mode)
        {
        case 1:
            Switch04kCHR_spr((*value) | chr_high, 0x0000);
            break;
        case 2:
            Switch02kCHR_spr((*value) | chr_high, 0x0800);
            break;
        case 3:
            Switch01kCHR_spr((*value) | chr_high, 0x0C00);
            break;
        }
        break;
    }
    case 0x5124:
    {
        chr_setB_last = 0;
        if (chr_mode == 3)
            Switch01kCHR_spr((*value) | chr_high, 0x1000);
        break;
    }
    case 0x5125:
    {
        chr_setB_last = 0;
        switch (chr_mode)
        {
        case 2:
            Switch02kCHR_spr((*value) | chr_high, 0x1000);
            break;
        case 3:
            Switch01kCHR_spr((*value) | chr_high, 0x1400);
            break;
        }
        break;
    }
    case 0x5126:
    {
        chr_setB_last = 0;
        if (chr_mode == 3)
            Switch01kCHR_spr((*value) | chr_high, 0x1800);
        break;
    }
    case 0x5127:
    {
        chr_setB_last = 0;
        switch (chr_mode)
        {
        case 0:
            Switch08kCHR_spr((*value) | chr_high);
            break;
        case 1:
            Switch04kCHR_spr((*value) | chr_high, 0x1000);
            break;
        case 2:
            Switch02kCHR_spr((*value) | chr_high, 0x1800);
            break;
        case 3:
            Switch01kCHR_spr((*value) | chr_high, 0x1C00);
            break;
        }
        break;
    }
    // BKG SET
    case 0x5128:
    {
        chr_setB_last = 1;
        if (chr_mode == 3)
        {
            MEM_SWITCH_CHR_01KB(CHR_AREA_0000, (*value) | chr_high);
            MEM_SWITCH_CHR_01KB(CHR_AREA_1000,(*value) | chr_high);
        }
        break;
    }
    case 0x5129:
    {
        chr_setB_last = 1;
        switch (chr_mode)
        {
        case 2:
        {
            MEM_SWITCH_CHR_02KB(CHR_AREA_0000,(*value) | chr_high);
            MEM_SWITCH_CHR_02KB(CHR_AREA_1000,(*value) | chr_high);
            break;
        }
        case 3:
        {
            MEM_SWITCH_CHR_01KB(CHR_AREA_0400,(*value) | chr_high);
            MEM_SWITCH_CHR_01KB(CHR_AREA_1400,(*value) | chr_high);
            break;
        }
        }
        break;
    }
    case 0x512A:
    {
        chr_setB_last = 1;
        if (chr_mode == 3)
        {
            MEM_SWITCH_CHR_01KB(CHR_AREA_0800,(*value) | chr_high);
            MEM_SWITCH_CHR_01KB(CHR_AREA_1800,(*value) | chr_high);
        }
        break;
    }
    case 0x512B:
    {
        chr_setB_last = 1;
        switch (chr_mode)
        {
        case 0:
        {
            Switch04kCHR_bkg(((*value) | chr_high), 0x0000);
            Switch04kCHR_bkg(((*value) | chr_high), 0x1000);
            break;
        }
        case 1:
        {
            MEM_SWITCH_CHR_04KB(CHR_AREA_0000,(*value) | chr_high);
            MEM_SWITCH_CHR_04KB(CHR_AREA_1000,(*value) | chr_high);
            break;
        }
        case 2:
        {
            MEM_SWITCH_CHR_02KB(CHR_AREA_0800,(*value) | chr_high);
            MEM_SWITCH_CHR_02KB(CHR_AREA_1800,(*value) | chr_high);
            break;
        }
        case 3:
        {
            MEM_SWITCH_CHR_01KB(CHR_AREA_0C00,(*value) | chr_high);
            MEM_SWITCH_CHR_01KB(CHR_AREA_1C00,(*value) | chr_high);
            break;
        }
        }
        break;
    }
    case 0x5130:
    {
        chr_high = ((*value) & 0x3) << 8;
        break;
    }

    //Fill-mode tile
    case 0x5106:
        for (int i = 0; i < 0x3C0; i++)
            mem_nmt_ram_banks[3][i] = (*value);
        break;
    //Fill-mode attr
    case 0x5107:
        for (int i = 0x3C0; i < (0x3C0 + 0x40); i++)
        {
            temp_fill = ((2 << ((*value) & 0x03)) | ((*value) & 0x03));
            temp_fill |= ((temp_fill & 0x0F) << 4);
            mem_nmt_ram_banks[3][i] = temp_fill;
        }
        break;
    case 0x5200:
    {
        split_tile = (*value) & 0x1F;
        split_enable = ((*value) & 0x80) == 0x80;
        split_right = ((*value) & 0x40) == 0x40;
        break;
    }
    case 0x5201:
    {
        split_yscroll = (*value);
        break;
    }
    case 0x5202:
    {
        Switch04kCHRSP((*value), (*address) & 0x0000);
        Switch04kCHRSP((*value), (*address) & 0x1000);
        break;
    }
    case 0x5203:
        irq_line = (*value);
        break;
    case 0x5204:
        irq_enable = (*value);
        break;
    case 0x5205:
        multiplicand = (*value);
        product = (multiplicand * multiplier) ;
        break;
    case 0x5206:
        multiplier = (*value);
        product = (multiplicand * multiplier) ;
        break;
    }

}
void Mapper005_ReadEX(unsigned short *address, unsigned char *value)
{
    if ((*address) < 0x5000)
        return;
    if ((*address) >= 0x6000)
        return;
    if ((*address) >= 0x5C00)
    {
        if (ExRAM_mode >= 2)
        {
            (*value) = mem_nmt_ram_banks[2][(*address) & 0x3FF];
            return;
        }
    }
    switch ((*address))
    {
    case 0x5010:
        (*value) = SC_MMC5_PCMRead5010();
        break;
    case 0x5015:
    {
        (*value) = (SC_MMC5_Pulse1ReadEnable() ? 0x1 : 0) | (SC_MMC5_Pulse2ReadEnable() ? 0x2 : 0);
        break;
    }
    case 0x5204:
    {
        (*value) = (irq_current_inframe | irq_pending) & 0xFF;
        irq_pending = 0;
        NES_IRQFlags &= IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0x5205:
        (*value) = (product & 0xFF);
        break;
    case 0x5206:
        (*value) = ((product & 0xFF00) >> 8);
        break;
    }
}
void Mapper005_ReadCHR(unsigned short *address, unsigned char *value)
{

// TODO: implement split mode
    if (ExRAM_mode == 1)// Extended Attribute mode
    {
        if (!ppu_is_sprfetch)
        {
            EXtilenumber = mem_nmt_ram_banks[2][lastAccessVRAM] & 0x3F;
            Switch04kCHREX(EXtilenumber | chr_high, (*address) & 0x1000);
            (*value) = mem_chr_rom_banks[CHROffsetEX[((*address) >> 10) & 0x7]][(*address) & 0x03FF];
        }
        else// Sprites not effected
        {
            (*value) = mem_chr_rom_banks[CHROffset_spr[((*address) >> 10) & 0x7]][(*address) & 0x03FF];
        }
    }
    else
    {
        if (ppu_reg_2000_Sprite_size == 0x10)
        {
            // When in 8x16 sprite mode, both sets of registers are used.
            // The 'A' set is used for sprite tiles, and the 'B' set is used for BG.
            if (!ppu_is_sprfetch)
                (*value) = mem_chr_rom_banks[mem_chr_rom_block_index[((*address) >> 10) & 0x7]][(*address) & 0x03FF];
            else
                (*value) = mem_chr_rom_banks[CHROffset_spr[((*address) >> 10) & 0x7]][(*address) & 0x03FF];
        }
        else
        {
            // When in 8x8 sprite mode, only one set is used for both BG and sprites.
            // Either 'A' or 'B', depending on which set is written to last
            if (chr_setB_last)
                (*value) = mem_chr_rom_banks[mem_chr_rom_block_index[((*address) >> 10) & 0x7]][(*address) & 0x03FF];
            else
                (*value) = mem_chr_rom_banks[CHROffset_spr[((*address) >> 10) & 0x7]][(*address) & 0x03FF];
        }
    }
}
void Mapper005_WriteNMT(unsigned short *address, unsigned char *value)
{
    if (ExRAM_mode == 1)
    {
        if (((*address) & 0x03FF) <= 0x3BF)
        {
            lastAccessVRAM = (*address) & 0x03FF;
        }
    }
}
void Mapper005_ReadNMT(unsigned short *address, unsigned char *value)
{
    /*
               *  %00 = Extra Nametable mode    ("Ex0")
                  %01 = Extended Attribute mode ("Ex1")
                  %10 = CPU access mode         ("Ex2")
                  %11 = CPU read-only mode      ("Ex3")

               * NT Values can be the following:
                    %00 = NES internal NTA
                    %01 = NES internal NTB
                    %10 = use ExRAM as NT
                    %11 = Fill Mode
               */
    if (split_doit)
    {
        // ExRAM is always used as the nametable in split screen mode.
        // return base.NMT[2][address & 0x03FF];
    }
    if (ExRAM_mode == 1)// Extended Attribute mode
    {
        if (((*address) & 0x03FF) <= 0x3BF)
        {
            lastAccessVRAM = (*address) & 0x03FF;
        }
        else
        {
            paletteNo = mem_nmt_ram_banks[2][lastAccessVRAM] & 0xC0;
            // Fix Attribute bits
            shift = ((lastAccessVRAM >> 4 & 0x04) | (lastAccessVRAM & 0x02));
            switch (shift)
            {
            case 0:
                (*value) = (paletteNo >> 6);
                return;
            case 2:
                (*value) = (paletteNo >> 4);
                return;
            case 4:
                (*value) = (paletteNo >> 2);
                return;
            case 6:
                (*value) = (paletteNo >> 0);
                return;
            }
        }
    }
}

void Mapper005_OnPPUScanlineClock()
{
// In frame signal
    irq_current_inframe = (IsInRender() && IsRenderingOn()) ? 0x40 : 0x00;
    if (irq_current_inframe == 0)
    {
        irq_current_inframe = 0x40;
        irq_current_counter = 0;
        irq_pending = 0;
        NES_IRQFlags &= IRQ_FLAG_BOARD_OFF;
    }
    else
    {
        irq_current_counter++;
        if (irq_current_counter == irq_line)
        {
            irq_pending = 0x80;// IRQ pending flag is raised *regardless* of whether or not IRQs are enabled.
            if (irq_enable == 0x80)// Trigger an IRQ on the 6502 if both this flag *and* the IRQ enable flag is set.
                NES_IRQFlags |= IRQ_FLAG_BOARD;
        }
    }
}

void Mapper005_GetPrec(double inVal, double inMax, double outMax, double * val)
{
    (*val) = (outMax * inVal) / inMax;
}
void Mapper005_OnAPUGetAudioSample(double *board_sample)
{
    // Use built-in mixer.

    Mapper005_GetPrec(mmc5_pulse1_output, 0xF, 0.14, &mmc5mix_channels_out[0]);// Normalize Square 1 output from 0-15 into 0 - 14 % of the mix
    Mapper005_GetPrec(mmc5_pulse2_output, 0xF, 0.14, &mmc5mix_channels_out[1]);// Normalize Square 2 output from 0-15 into 0 - 14 % of the mix
    Mapper005_GetPrec(mmc5_pcm_output, 0xFF, 0.44, &mmc5mix_channels_out[2]);// Normalize DMC-PCM output from 0-255 into 0 - 44 % of the mix

    (*board_sample) = (mmc5mix_channels_out[0] + mmc5mix_channels_out[1] + mmc5mix_channels_out[2]);// 0 - 1.0 ( 0 - 100 %)
}
void Mapper005_OnAPUClock()
{
    SC_MMC5_Pulse1Clock();
    SC_MMC5_Pulse2Clock();
}
void Mapper005_OnAPUClockLength()
{

}
void Mapper005_OnAPUClockEnvelope()
{
    SC_MMC5_Pulse1ClockEnvelope();
    SC_MMC5_Pulse2ClockEnvelope();
    SC_MMC5_Pulse1ClockLength();
    SC_MMC5_Pulse2ClockLength();
}

void UpdateRamProtect()
{
    MEM_TOGGLE_PRG_32KB_RAM_WRITABLE(PRG_AREA_0000, (ram_protectA == 0x2) && (ram_protectB == 0x1));
    MEM_TOGGLE_PRG_32KB_RAM_WRITABLE(PRG_AREA_8000, (ram_protectA == 0x2) && (ram_protectB == 0x1));
}
void Switch04kCHR_bkg(int index, int where)
{
    int area = (where >> 10) & 0x07;
    index <<= 2;

    mem_chr_rom_block_index[area] = (index & mem_chr_rom_01kb_mask);
    area++;
    index++;
    mem_chr_rom_block_index[area] = (index & mem_chr_rom_01kb_mask);
    area++;
    index++;
    mem_chr_rom_block_index[area] = (index & mem_chr_rom_01kb_mask);
    area++;
    index++;
    mem_chr_rom_block_index[area] = (index & mem_chr_rom_01kb_mask);
}
void Switch01kCHR_spr(int index, int where)
{
    CHROffset_spr[(where >> 10) & 0x07] = (index & mem_chr_rom_01kb_mask);
}
void Switch02kCHR_spr(int index, int where)
{
    int area = (where >> 10) & 0x07;
    index <<= 1;

    CHROffset_spr[area] = (index & mem_chr_rom_01kb_mask);
    index++;
    CHROffset_spr[area + 1] = (index & mem_chr_rom_01kb_mask);
}
void Switch04kCHR_spr(int index, int where)
{
    int area = (where >> 10) & 0x07;
    index <<= 2;

    CHROffset_spr[area] = (index & mem_chr_rom_01kb_mask);
    area++;
    index++;
    CHROffset_spr[area] = (index & mem_chr_rom_01kb_mask);
    area++;
    index++;
    CHROffset_spr[area] = (index & mem_chr_rom_01kb_mask);
    area++;
    index++;
    CHROffset_spr[area] = (index & mem_chr_rom_01kb_mask);
}
void Switch08kCHR_spr(int index)
{
    index <<= 3;
    CHROffset_spr[0] = (index & mem_chr_rom_01kb_mask);
    index++;
    CHROffset_spr[1] = (index & mem_chr_rom_01kb_mask);
    index++;
    CHROffset_spr[2] = (index & mem_chr_rom_01kb_mask);
    index++;
    CHROffset_spr[3] = (index & mem_chr_rom_01kb_mask);
    index++;
    CHROffset_spr[4] = (index & mem_chr_rom_01kb_mask);
    index++;
    CHROffset_spr[5] = (index & mem_chr_rom_01kb_mask);
    index++;
    CHROffset_spr[6] = (index & mem_chr_rom_01kb_mask);
    index++;
    CHROffset_spr[7] = (index & mem_chr_rom_01kb_mask);
}
void Switch04kCHREX(int index, int where)
{
    int area = (where >> 10) & 0x07;
    index <<= 2;

    CHROffsetEX[area] = (index & mem_chr_rom_01kb_mask);
    area++;
    index++;
    CHROffsetEX[area] = (index & mem_chr_rom_01kb_mask);
    area++;
    index++;
    CHROffsetEX[area] = (index & mem_chr_rom_01kb_mask);
    area++;
    index++;
    CHROffsetEX[area] = (index & mem_chr_rom_01kb_mask);
}
void Switch04kCHRSP(int index, int where)
{
    int area = (where >> 10) & 0x07;
    index <<= 2;

    CHROffsetSP[area] = (index & mem_chr_rom_01kb_mask);
    area++;
    index++;
    CHROffsetSP[area] = (index & mem_chr_rom_01kb_mask);
    area++;
    index++;
    CHROffsetSP[area] = (index & mem_chr_rom_01kb_mask);
    area++;
    index++;
    CHROffsetSP[area] = (index & mem_chr_rom_01kb_mask);
}

