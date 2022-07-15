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

#include "../ExternalSoundChannels/SC_VRC6_Pulse1.h"
#include "../ExternalSoundChannels/SC_VRC6_Pulse2.h"
#include "../ExternalSoundChannels/SC_VRC6_Sawtooth.h"

int irq_reload;
int irq_counter;
int prescaler;
char irq_mode_cycle;
char irq_enable;
char irq_enable_on_ak;

double vrc6_playback_outs[3];

void Mapper026HardReset()
{
    Mapper000HardReset();
    MEM_SWITCH_PRG_08KB(PRG_AREA_E000, mem_prg_rom_08kb_mask);

    SC_VRC6_P1_HardReset();
    SC_VRC6_P2_HardReset();
    SC_VRC6_ST_HardReset();
}

// PR is between $8000 - $FFFF
void Mapper026_WritePR(unsigned short *address, unsigned char *value)
{
    switch ((*address))
    {
    case 0x8000:
    case 0x8002:
    case 0x8001:
    case 0x8003:
        MEM_SWITCH_PRG_16KB(PRG_AREA_8000, (*value));
        break;
    case 0x9000:
        SC_VRC6_P1_Write0(value);
        break;
    case 0x9002:
        SC_VRC6_P1_Write1(value);
        break;
    case 0x9001:
        SC_VRC6_P1_Write2(value);
        break;
    case 0xA000:
        SC_VRC6_P2_Write0(value);
        break;
    case 0xA002:
        SC_VRC6_P2_Write1(value);
        break;
    case 0xA001:
        SC_VRC6_P2_Write2(value);
        break;
    case 0xB000:
        SC_VRC6_ST_Write0(value);
        break;
    case 0xB002:
        SC_VRC6_ST_Write1(value);
        break;
    case 0xB001:
        SC_VRC6_ST_Write2(value);
        break;
    case 0xB003:
    {
        switch (((*value) & 0xC) >> 2)
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
    case 0xC000:
    case 0xC001:
    case 0xC002:
    case 0xC003:
        MEM_SWITCH_PRG_08KB(PRG_AREA_C000, (*value));
        break;
    case 0xD000:
        MEM_SWITCH_CHR_01KB(CHR_AREA_0000, (*value));
        break;
    case 0xD002:
        MEM_SWITCH_CHR_01KB(CHR_AREA_0400,(*value));
        break;
    case 0xD001:
        MEM_SWITCH_CHR_01KB(CHR_AREA_0800,(*value));
        break;
    case 0xD003:
        MEM_SWITCH_CHR_01KB(CHR_AREA_0C00,(*value));
        break;
    case 0xE000:
        MEM_SWITCH_CHR_01KB(CHR_AREA_1000,(*value));
        break;
    case 0xE002:
        MEM_SWITCH_CHR_01KB(CHR_AREA_1400,(*value));
        break;
    case 0xE001:
        MEM_SWITCH_CHR_01KB(CHR_AREA_1800,(*value));
        break;
    case 0xE003:
        MEM_SWITCH_CHR_01KB(CHR_AREA_1C00,(*value));
        break;
    case 0xF000:
        irq_reload = (*value);
        break;
    case 0xF002:
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
    case 0xF001:
        NES_IRQFlags &= IRQ_FLAG_BOARD_OFF;
        irq_enable = irq_enable_on_ak;
        break;
    }
}
void Mapper026_OnCPUClock()
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
void Mapper026_GetPrec(double inVal, double inMax, double outMax, double * val)
{
    (*val) = (outMax * inVal) / inMax;
}
void Mapper026_OnAPUGetAudioSample(double *board_sample)
{
   // (*board_sample)= (audio_pulse_table[sc_vrc6_p1_output + sc_vrc6_p2_output] + audio_tnd_table[sc_vrc6_st_output]) * vrc6_playback_amplitude;
    // Use built-in mixer.

    Mapper026_GetPrec(sc_vrc6_p1_output, 0xF, 0.14, &vrc6_playback_outs[0]);// Normalize Square 1 output from 0-15 into 0 - 14 % of the mix
    Mapper026_GetPrec(sc_vrc6_p2_output, 0xF, 0.14, &vrc6_playback_outs[1]);// Normalize Square 2 output from 0-15 into 0 - 14 % of the mix
    Mapper026_GetPrec(sc_vrc6_st_output, 0xFF, 0.44, &vrc6_playback_outs[2]);// Normalize DMC-PCM output from 0-255 into 0 - 44 % of the mix

    (*board_sample) = (vrc6_playback_outs[0] + vrc6_playback_outs[1] + vrc6_playback_outs[2]);// 0 - 1.0 ( 0 - 100 %)
}
void Mapper026_OnAPUClockSingle()
{
    SC_VRC6_P1_ClockSingle();
    SC_VRC6_P2_ClockSingle();
    SC_VRC6_ST_ClockSingle();
}
