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

#include "SC_MMC5_Pulse1.h"
#include "../APU.h"

unsigned char mmc5_pulse1_duty_cycle_sequences[4][8] =
{
    {  0, 0, 0, 0, 0, 0, 0, 1 }, // 12.5%
    {  0, 0, 0, 0, 0, 0, 1, 1 }, // 25.0%
    {  0, 0, 0, 0, 1, 1, 1, 1 }, // 50.0%
    {  1, 1, 1, 1, 1, 1, 0, 0 }, // 75.0% (25.0% negated)
};
unsigned char mmc5_pulse1_duration_table[] =
{
    0x0A, 0xFE, 0x14, 0x02, 0x28, 0x04, 0x50, 0x06, 0xA0, 0x08, 0x3C, 0x0A, 0x0E, 0x0C, 0x1A, 0x0E,
    0x0C, 0x10, 0x18, 0x12, 0x30, 0x14, 0x60, 0x16, 0xC0, 0x18, 0x48, 0x1A, 0x10, 0x1C, 0x20, 0x1E,
};


unsigned char mmc5_pulse1_duty_cycle;
char mmc5_pulse1_length_halt;
char mmc5_pulse1_constant_volume_envelope;
unsigned char mmc5_pulse1_volume_devider_period;
// Reg 3
int mmc5_pulse1_timer;

// Controls
int mmc5_pulse1_period_devider;
unsigned char mmc5_pulse1_seqencer;
char mmc5_pulse1_length_enabled;
int mmc5_pulse1_length_counter;
char mmc5_pulse1_envelope_start_flag;
unsigned char mmc5_pulse1_envelope_devider;
unsigned char mmc5_pulse1_envelope_decay_level_counter;
unsigned char mmc5_pulse1_envelope;
char mmc5_pulse1_ignore_reload;

void SC_MMC5_Pulse1HardReset()
{
    mmc5_pulse1_duty_cycle = 0;
    mmc5_pulse1_length_halt = 0;
    mmc5_pulse1_constant_volume_envelope = 0;
    mmc5_pulse1_volume_devider_period = 0;
    mmc5_pulse1_timer = 0;
    mmc5_pulse1_period_devider = 0;
    mmc5_pulse1_seqencer = 0;
    mmc5_pulse1_length_enabled = 0;
    mmc5_pulse1_length_counter = 0;
    mmc5_pulse1_envelope_start_flag = 0;
    mmc5_pulse1_envelope_devider = 0;
    mmc5_pulse1_envelope_decay_level_counter = 0;
    mmc5_pulse1_envelope = 0;
    mmc5_pulse1_output = 0;
    mmc5_pulse1_ignore_reload = 0;
}
void SC_MMC5_Pulse1SoftReset()
{
    SC_MMC5_Pulse1HardReset();
}

void SC_MMC5_Pulse1Clock()
{
    mmc5_pulse1_period_devider--;
    if (mmc5_pulse1_period_devider <= 0)
    {
        mmc5_pulse1_period_devider = mmc5_pulse1_timer + 1;
        mmc5_pulse1_seqencer = (mmc5_pulse1_seqencer + 1) & 0x7;
        if (mmc5_pulse1_length_counter > 0)
        {
            mmc5_pulse1_output = mmc5_pulse1_duty_cycle_sequences[mmc5_pulse1_duty_cycle][mmc5_pulse1_seqencer] * mmc5_pulse1_envelope;
        }
        else
            mmc5_pulse1_output = 0;
    }
}
void SC_MMC5_Pulse1ClockLength()
{
    if (mmc5_pulse1_length_counter > 0 && !mmc5_pulse1_length_halt)
    {
        mmc5_pulse1_length_counter--;
    }
}
void SC_MMC5_Pulse1ClockEnvelope()
{
    if (mmc5_pulse1_envelope_start_flag)
    {
        mmc5_pulse1_envelope_start_flag = 0;
        mmc5_pulse1_envelope_decay_level_counter = 15;
        mmc5_pulse1_envelope_devider = (mmc5_pulse1_volume_devider_period + 1)&0xFF;
    }
    else
    {
        if (mmc5_pulse1_envelope_devider > 0)
            mmc5_pulse1_envelope_devider--;
        else
        {
            mmc5_pulse1_envelope_devider = (mmc5_pulse1_volume_devider_period + 1)&0xFF;
            if (mmc5_pulse1_envelope_decay_level_counter > 0)
                mmc5_pulse1_envelope_decay_level_counter--;
            else if (mmc5_pulse1_length_halt)
                mmc5_pulse1_envelope_decay_level_counter = 0xF;
        }
    }
    mmc5_pulse1_envelope = mmc5_pulse1_constant_volume_envelope ? mmc5_pulse1_volume_devider_period : mmc5_pulse1_envelope_decay_level_counter;

}

void SC_MMC5_Pulse1Write5000(unsigned char *value)
{
    mmc5_pulse1_duty_cycle = ((*value) & 0xC0) >> 6;
    mmc5_pulse1_volume_devider_period = (*value)  & 0xF;
    mmc5_pulse1_length_halt = ((*value)  & 0x20) != 0;
    mmc5_pulse1_constant_volume_envelope = ((*value)  & 0x10) != 0;

    mmc5_pulse1_envelope = mmc5_pulse1_constant_volume_envelope ?   mmc5_pulse1_volume_devider_period :   mmc5_pulse1_envelope_decay_level_counter;

}
void SC_MMC5_Pulse1Write5002(unsigned char *value)
{
    mmc5_pulse1_timer = (mmc5_pulse1_timer & 0xFF00) | (*value);
}
void SC_MMC5_Pulse1Write5003(unsigned char *value)
{
    mmc5_pulse1_timer = (  mmc5_pulse1_timer & 0x00FF) | (((*value) & 0x7) << 8);

    if (  mmc5_pulse1_length_enabled)
    {
        mmc5_pulse1_length_counter =  mmc5_pulse1_duration_table[(*value) >> 3];
    }

    mmc5_pulse1_seqencer = 0;
    mmc5_pulse1_envelope_start_flag = 1;
}

void SC_MMC5_Pulse1SetEnable(char enable)
{
    mmc5_pulse1_length_enabled = enable;
    if (!mmc5_pulse1_length_enabled)
        mmc5_pulse1_length_counter = 0;
}
char SC_MMC5_Pulse1ReadEnable()
{
    return (mmc5_pulse1_length_counter > 0);
}
