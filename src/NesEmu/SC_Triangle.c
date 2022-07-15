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

#include "SC_Triangle.h"
#include "DIFINES.h"
#include "APU.h"

unsigned char trl_step_seq[] =
{
    15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15
};
unsigned char trl_duration_table[] =
{
    0x0A, 0xFE, 0x14, 0x02, 0x28, 0x04, 0x50, 0x06, 0xA0, 0x08, 0x3C, 0x0A, 0x0E, 0x0C, 0x1A, 0x0E,
    0x0C, 0x10, 0x18, 0x12, 0x30, 0x14, 0x60, 0x16, 0xC0, 0x18, 0x48, 0x1A, 0x10, 0x1C, 0x20, 0x1E,
};
// Reg1
char trl_liner_control_flag;
unsigned char trl_liner_control_reload;
// Reg2
unsigned short trl_timer;

char trl_length_enabled;
unsigned char trl_length_counter;
char trl_liner_control_reload_flag;
unsigned char trl_liner_counter;
int trl_period_devider;
int trl_step;
char trl_ignore_reload;

void TRLHardReset()
{
    trl_liner_control_flag = 0;
    trl_liner_control_reload = 0;
    trl_timer = 0;
    trl_length_enabled = 0;
    trl_length_counter = 0;
    trl_liner_control_reload_flag = 0;
    trl_liner_counter = 0;
    trl_output = 0;
    trl_period_devider = 0;
    trl_step = 0;
    trl_ignore_reload = 0;
}
void TRLSoftReset()
{
    TRLHardReset();
}

void TRLClock()
{
    trl_period_devider--;
    if (trl_period_devider <= 0)
    {
        trl_period_devider = trl_timer + 1;

        if (trl_length_counter > 0 && trl_liner_counter > 0)
        {
            if (trl_timer >= 4)
            {
                trl_step++;
                trl_step &= 0x1F;
                trl_output = trl_step_seq[trl_step];
            }
        }
    }
}
void TRLClockLength()
{
    if (trl_length_counter > 0 && !trl_liner_control_flag)
    {
        trl_length_counter--;
        if (apu_reg_access_happened)
        {
            // This is not a hack, there is some hidden mechanism in the nes, that do reload and clock stuff
            if (apu_reg_io_addr == 0xB && apu_reg_access_w)
            {
                trl_ignore_reload = 1;
            }
        }
    }
}
void TRLClockEnvelope()
{
    if (trl_liner_control_reload_flag)
    {
        trl_liner_counter = trl_liner_control_reload;
    }
    else
    {
        if (trl_liner_counter > 0)
            trl_liner_counter--;
    }
    if (!trl_liner_control_flag)
        trl_liner_control_reload_flag = 0;
}

void APUOnRegister4008()
{
    // Only writes accepted
    if (!apu_reg_access_w)
        return;
    trl_liner_control_flag = (apu_reg_io_db & 0x80) == 0x80;
    trl_liner_control_reload = apu_reg_io_db & 0x7F;
}
void APUOnRegister400A()
{
    // Only writes accepted
    if (!apu_reg_access_w)
        return;
    trl_timer = (trl_timer & 0x7F00) | apu_reg_io_db;
}
void APUOnRegister400B()
{
    // Only writes accepted
    if (!apu_reg_access_w)
        return;
    trl_timer = (trl_timer & 0x00FF) | (apu_reg_io_db & 0x7) << 8;

    if (trl_length_enabled && !trl_ignore_reload)
        trl_length_counter = trl_duration_table[apu_reg_io_db >> 3];
    if (trl_ignore_reload)
        trl_ignore_reload = 0;
    trl_liner_control_reload_flag = 1;
}

void TRLOn4015()
{
    trl_length_enabled = (apu_reg_io_db & 0x04) != 0;
    if (!trl_length_enabled)
        trl_length_counter = 0;
}
char TRLRead4015()
{
    if (trl_length_counter > 0)
        apu_reg_io_db = (apu_reg_io_db & 0xFB) | 0x04;
}
