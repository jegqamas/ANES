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

#include "DIFINES.h"
#include "SC_Noise.h"
#include "APU.h"

unsigned char nos_duration_table[] =
{
    0x0A, 0xFE, 0x14, 0x02, 0x28, 0x04, 0x50, 0x06, 0xA0, 0x08, 0x3C, 0x0A, 0x0E, 0x0C, 0x1A, 0x0E,
    0x0C, 0x10, 0x18, 0x12, 0x30, 0x14, 0x60, 0x16, 0xC0, 0x18, 0x48, 0x1A, 0x10, 0x1C, 0x20, 0x1E,
};
unsigned short nos_freq_table[3][16] =
{
    //NTSC
    {
        4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068
    },
    //PAL
    {
        4, 7, 14, 30, 60, 88, 118, 148, 188, 236, 354, 472, 708,  944, 1890, 3778
    },
    //DENDY (same as ntsc for now)
    {
        4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068
    }
};
// Reg 1
char nos_length_halt;
char nos_constant_volume_envelope;
unsigned char nos_volume_devider_period;

// Reg 3
unsigned short nos_timer;
char nos_mode;

// Controls
int nos_period_devider;
char nos_length_enabled;
int nos_length_counter;
char nos_envelope_start_flag;
unsigned char nos_envelope_devider;
unsigned char nos_envelope_decay_level_counter;
unsigned char nos_envelope;
int nos_shift_reg;
int nos_feedback;
char nos_ignore_reload;
int system_index;

void NOSHardReset(int tv_format)
{
    system_index=tv_format;
    nos_length_halt = 0;
    nos_constant_volume_envelope = 0;
    nos_volume_devider_period = 0;
    nos_shift_reg = 1;
    nos_timer = 0;
    nos_mode = 0;
    nos_period_devider = 0;
    nos_length_enabled = 0;
    nos_length_counter = 0;
    nos_envelope_start_flag = 0;
    nos_envelope_devider = 0;
    nos_envelope_decay_level_counter = 0;
    nos_envelope = 0;
    nos_output = 0;
    nos_feedback = 0;
    nos_ignore_reload = 0;
}
void NOSSoftReset()
{
    NOSHardReset(system_index);
}

void NOSClock()
{
    nos_period_devider--;
    if (nos_period_devider <= 0)
    {
        nos_period_devider = nos_timer;

        if (nos_mode)
            nos_feedback = ((nos_shift_reg >> 6) & 0x1) ^ (nos_shift_reg & 0x1);
        else
            nos_feedback = ((nos_shift_reg >> 1) & 0x1) ^ (nos_shift_reg & 0x1);
        nos_shift_reg >>= 1;
        nos_shift_reg = (nos_shift_reg & 0x3FFF) | ((nos_feedback & 1) << 14);

        if (nos_length_counter > 0 && ((nos_shift_reg & 1) == 0))
        {
            nos_output = nos_envelope;
        }
        else
            nos_output = 0;
    }
}
void NOSClockLength()
{
    if (nos_length_counter > 0 && !nos_length_halt)
    {
        nos_length_counter--;
        if (apu_reg_access_happened)
        {
            // This is not a hack, there is some hidden mechanism in the nes, that do reload and clock stuff
            if (apu_reg_io_addr == 0xF && apu_reg_access_w)
            {
                nos_ignore_reload = 1;
            }
        }
    }
}
void NOSClockEnvelope()
{
    if (nos_envelope_start_flag)
    {
        nos_envelope_start_flag = 0;
        nos_envelope_decay_level_counter = 15;
        nos_envelope_devider = (nos_volume_devider_period + 1)&0xFF;
    }
    else
    {
        if (nos_envelope_devider > 0)
            nos_envelope_devider--;
        else
        {
            nos_envelope_devider = (nos_volume_devider_period + 1)&0xFF;
            if (nos_envelope_decay_level_counter > 0)
                nos_envelope_decay_level_counter--;
            else if (nos_length_halt)
                nos_envelope_decay_level_counter = 0xF;
        }
    }
    nos_envelope = nos_constant_volume_envelope ? nos_volume_devider_period : nos_envelope_decay_level_counter;
}

void APUOnRegister400C()
{
    // Only writes accepted
    if (!apu_reg_access_w)
        return;
    nos_volume_devider_period = apu_reg_io_db & 0xF;
    nos_length_halt = (apu_reg_io_db & 0x20) != 0;
    nos_constant_volume_envelope = (apu_reg_io_db & 0x10) != 0;

    nos_envelope = nos_constant_volume_envelope ? nos_volume_devider_period : nos_envelope_decay_level_counter;
}
void APUOnRegister400E()
{
    // Only writes accepted
    if (!apu_reg_access_w)
        return;
    nos_timer = nos_freq_table[system_index][apu_reg_io_db & 0x0F] / 2;

    nos_mode = (apu_reg_io_db & 0x80) == 0x80;
}
void APUOnRegister400F()
{
    // Only writes accepted
    if (!apu_reg_access_w)
        return;

    if (nos_length_enabled && !nos_ignore_reload)
        nos_length_counter = nos_duration_table[apu_reg_io_db >> 3];
    if (nos_ignore_reload)
        nos_ignore_reload = 0;
    nos_envelope_start_flag = 1;
}

void NOSOn4015()
{
    nos_length_enabled = (apu_reg_io_db & 0x08) != 0;
    if (!nos_length_enabled)
        nos_length_counter = 0;
}
char NOSRead4015()
{
    if (nos_length_counter > 0)
        apu_reg_io_db = (apu_reg_io_db & 0xF7) | 0x08;
}
