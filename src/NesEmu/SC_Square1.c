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
#include "SC_Square1.h"
#include "DIFINES.h"
#include "APU.h"

unsigned char sq1_duty_cycle_sequences[4][8] =
{
    {  0, 0, 0, 0, 0, 0, 0, 1 }, // 12.5%
    {  0, 0, 0, 0, 0, 0, 1, 1 }, // 25.0%
    {  0, 0, 0, 0, 1, 1, 1, 1 }, // 50.0%
    {  1, 1, 1, 1, 1, 1, 0, 0 }, // 75.0% (25.0% negated)
};
unsigned char sq1_duration_table[] =
{
    0x0A, 0xFE, 0x14, 0x02, 0x28, 0x04, 0x50, 0x06, 0xA0, 0x08, 0x3C, 0x0A, 0x0E, 0x0C, 0x1A, 0x0E,
    0x0C, 0x10, 0x18, 0x12, 0x30, 0x14, 0x60, 0x16, 0xC0, 0x18, 0x48, 0x1A, 0x10, 0x1C, 0x20, 0x1E,
};
unsigned char sq1_duty_cycle;
char sq1_length_halt;
char sq1_constant_volume_envelope;
unsigned char sq1_volume_devider_period;
// Reg 2
char sq1_sweep_enable;
unsigned char sq1_sweep_devider_period;
char sq1_sweep_negate;
unsigned char sq1_sweep_shift_count;
// Reg 3
int sq1_timer;

// Controls
int sq1_period_devider;
unsigned char sq1_seqencer;
char sq1_length_enabled;
int sq1_length_counter;
char sq1_envelope_start_flag;
unsigned char sq1_envelope_devider;
unsigned char sq1_envelope_decay_level_counter;
unsigned char sq1_envelope;
int sq1_sweep_counter;
char sq1_sweep_reload;
int sq1_sweep_change;
char sq1_valid_freq;
char sq1_ignore_reload;

void SC_Square1HardReset()
{
    sq1_duty_cycle = 0;
    sq1_length_halt = 0;
    sq1_constant_volume_envelope = 0;
    sq1_volume_devider_period = 0;
    sq1_sweep_enable = 0;
    sq1_sweep_devider_period = 0;
    sq1_sweep_negate = 0;
    sq1_sweep_shift_count = 0;
    sq1_timer = 0;
    sq1_period_devider = 0;
    sq1_seqencer = 0;
    sq1_length_enabled = 0;
    sq1_length_counter = 0;
    sq1_envelope_start_flag = 0;
    sq1_envelope_devider = 0;
    sq1_envelope_decay_level_counter = 0;
    sq1_envelope = 0;
    sq1_sweep_counter = 0;
    sq1_sweep_reload = 0;
    sq1_sweep_change = 0;
    sq1_valid_freq = 0;
    sq1_output = 0;
    sq1_ignore_reload = 0;
}
void SC_Square1SoftReset()
{
    SC_Square1HardReset();
}

void SC_Square1Clock()
{
    sq1_period_devider--;
    if (sq1_period_devider <= 0)
    {
        sq1_period_devider = sq1_timer + 1;
        sq1_seqencer = (sq1_seqencer + 1) & 0x7;
        if (sq1_length_counter > 0 && sq1_valid_freq)
        {
            sq1_output = sq1_duty_cycle_sequences[sq1_duty_cycle][sq1_seqencer] * sq1_envelope;
        }
        else
            sq1_output = 0;
    }
}

void SQ1CalculateValidFreq()
{
    sq1_valid_freq = (sq1_timer >= 0x8) && ((sq1_sweep_negate) || (((sq1_timer + (sq1_timer >> sq1_sweep_shift_count)) & 0x800) == 0));
}

void SC_Square1ClockLength()
{
    if (sq1_length_counter > 0 && !sq1_length_halt)
    {
        sq1_length_counter--;
        if (apu_reg_access_happened)
        {
            // This is not a hack, there is some hidden mechanism in the nes, that do reload and clock stuff
            if (apu_reg_io_addr == 0x3 && apu_reg_access_w)
            {
                sq1_ignore_reload = 1;
            }
        }
    }
    // Sweep unit
    sq1_sweep_counter--;
    if (sq1_sweep_counter == 0)
    {
        sq1_sweep_counter = sq1_sweep_devider_period + 1;
        if (sq1_sweep_enable && sq1_sweep_shift_count > 0 && sq1_valid_freq)
        {
            sq1_sweep_change = sq1_timer >> sq1_sweep_shift_count;
            sq1_timer += sq1_sweep_negate ? ~sq1_sweep_change : sq1_sweep_change;
            SQ1CalculateValidFreq();
        }
    }
    if (sq1_sweep_reload)
    {
        sq1_sweep_counter = sq1_sweep_devider_period + 1;
        sq1_sweep_reload = 0;
    }
}
void SC_Square1ClockEnvelope()
{
    if (sq1_envelope_start_flag)
    {
        sq1_envelope_start_flag = 0;
        sq1_envelope_decay_level_counter = 15;
        sq1_envelope_devider = (sq1_volume_devider_period + 1)&0xFF;
    }
    else
    {
        if (sq1_envelope_devider > 0)
            sq1_envelope_devider--;
        else
        {
            sq1_envelope_devider = (sq1_volume_devider_period + 1)&0xFF;
            if (sq1_envelope_decay_level_counter > 0)
                sq1_envelope_decay_level_counter--;
            else if (sq1_length_halt)
                sq1_envelope_decay_level_counter = 0xF;
        }
    }
    sq1_envelope = sq1_constant_volume_envelope ? sq1_volume_devider_period : sq1_envelope_decay_level_counter;
}

void APUOnRegister4000()
{
// Only writes accepted
    if (!apu_reg_access_w)
        return;
    sq1_duty_cycle = (apu_reg_io_db & 0xC0) >> 6;
    sq1_volume_devider_period = apu_reg_io_db & 0xF;
    sq1_length_halt = (apu_reg_io_db & 0x20) != 0;
    sq1_constant_volume_envelope = (apu_reg_io_db & 0x10) != 0;

    sq1_envelope = sq1_constant_volume_envelope ? sq1_volume_devider_period : sq1_envelope_decay_level_counter;
}
void APUOnRegister4001()
{
    // Only writes accepted
    if (!apu_reg_access_w)
        return;

    sq1_sweep_enable = (apu_reg_io_db & 0x80) == 0x80;
    sq1_sweep_devider_period = (apu_reg_io_db >> 4) & 7;
    sq1_sweep_negate = (apu_reg_io_db & 0x8) == 0x8;
    sq1_sweep_shift_count = apu_reg_io_db & 7;
    sq1_sweep_reload = 1;
    SQ1CalculateValidFreq();
}
void APUOnRegister4002()
{
    // Only writes accepted
    if (!apu_reg_access_w)
        return;
    sq1_timer = (sq1_timer & 0xFF00) | apu_reg_io_db;
    SQ1CalculateValidFreq();
}
void APUOnRegister4003()
{
    // Only writes accepted
    if (!apu_reg_access_w)
        return;

    sq1_timer = (sq1_timer & 0x00FF) | ((apu_reg_io_db & 0x7) << 8);

    if (sq1_length_enabled && !sq1_ignore_reload)
    {
        sq1_length_counter = sq1_duration_table[apu_reg_io_db >> 3];
    }
    if (sq1_ignore_reload)
        sq1_ignore_reload = 0;

    sq1_seqencer = 0;
    sq1_envelope_start_flag = 1;
    SQ1CalculateValidFreq();
}

void SC_Square1OnRegister4015()
{
    sq1_length_enabled = (apu_reg_io_db & 0x01) != 0;
    if (!sq1_length_enabled)
        sq1_length_counter = 0;
}
char SC_Square1ReadRegister4015()
{
    if (sq1_length_counter > 0)
        apu_reg_io_db = (apu_reg_io_db & 0xFE) | 0x01;
}
