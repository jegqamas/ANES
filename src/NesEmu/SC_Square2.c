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

#include "SC_Square2.h"
#include "DIFINES.h"
#include "APU.h"

unsigned char sq2_duty_cycle_sequences[4][8] =
{
    //  new byte[] {  0, 1, 0, 0, 0, 0, 0, 0 }, // 12.5%
    //  new byte[] {  0, 1, 1, 0, 0, 0, 0, 0 }, // 25.0%
    //  new byte[] {  0, 1, 1, 1, 1, 0, 0, 0 }, // 50.0%
    // new byte[] {  1, 0, 0, 1, 1, 1, 1, 1 }, // 75.0% (25.0% negated)
    {  0, 0, 0, 0, 0, 0, 0, 1 }, // 12.5%
    {  0, 0, 0, 0, 0, 0, 1, 1 }, // 25.0%
    {  0, 0, 0, 0, 1, 1, 1, 1 }, // 50.0%
    {  1, 1, 1, 1, 1, 1, 0, 0 }, // 75.0% (25.0% negated)
};
unsigned char sq2_duration_table[] =
{
    0x0A, 0xFE, 0x14, 0x02, 0x28, 0x04, 0x50, 0x06, 0xA0, 0x08, 0x3C, 0x0A, 0x0E, 0x0C, 0x1A, 0x0E,
    0x0C, 0x10, 0x18, 0x12, 0x30, 0x14, 0x60, 0x16, 0xC0, 0x18, 0x48, 0x1A, 0x10, 0x1C, 0x20, 0x1E,
};
// Reg 1
unsigned char sq2_duty_cycle;
char sq2_length_halt;
char sq2_constant_volume_envelope;
unsigned char sq2_volume_devider_period;
// Reg 2
char sq2_sweep_enable;
unsigned char sq2_sweep_devider_period;
char sq2_sweep_negate;
unsigned char sq2_sweep_shift_count;
// Reg 3
int sq2_timer;

// Controls
int sq2_period_devider;
unsigned char sq2_seqencer;
char sq2_length_enabled;
int sq2_length_counter;
char sq2_envelope_start_flag;
unsigned char sq2_envelope_devider;
unsigned char sq2_envelope_decay_level_counter;
unsigned char sq2_envelope;
int sq2_sweep_counter;
char sq2_sweep_reload;
int sq2_sweep_change;
char sq2_valid_freq;
char sq2_ignore_reload;

void SC_Square2HardReset()
{
    sq2_duty_cycle = 0;
    sq2_length_halt = 0;
    sq2_constant_volume_envelope = 0;
    sq2_volume_devider_period = 0;
    sq2_sweep_enable = 0;
    sq2_sweep_devider_period = 0;
    sq2_sweep_negate = 0;
    sq2_sweep_shift_count = 0;
    sq2_timer = 0;
    sq2_period_devider = 0;
    sq2_seqencer = 0;
    sq2_length_enabled = 0;
    sq2_length_counter = 0;
    sq2_envelope_start_flag = 0;
    sq2_envelope_devider = 0;
    sq2_envelope_decay_level_counter = 0;
    sq2_envelope = 0;
    sq2_sweep_counter = 0;
    sq2_sweep_reload = 0;
    sq2_sweep_change = 0;
    sq2_valid_freq = 0;
    sq2_output = 0;
    sq2_ignore_reload = 0;
}
void SC_Square2SoftReset()
{
    SC_Square2HardReset();
}

void SC_Square2Clock()
{
    sq2_period_devider--;
    if (sq2_period_devider <= 0)
    {
        sq2_period_devider = sq2_timer + 1;
        sq2_seqencer = (sq2_seqencer + 1) & 0x7;
        if (sq2_length_counter > 0 && sq2_valid_freq)
        {
            sq2_output = sq2_duty_cycle_sequences[sq2_duty_cycle][sq2_seqencer] * sq2_envelope;
        }
        else
            sq2_output = 0;
    }
}
void SQ2CalculateValidFreq()
{
    sq2_valid_freq = (sq2_timer >= 0x8) && ((sq2_sweep_negate) ||
                                            (((sq2_timer + (sq2_timer >> sq2_sweep_shift_count)) & 0x800) == 0));
}

void SC_Square2ClockLength()
{
    if (sq2_length_counter > 0 && !sq2_length_halt)
    {
        sq2_length_counter--;
        if (apu_reg_access_happened)
        {
            // This is not a hack, there is some hidden mechanism in the nes, that do reload and clock stuff
            if (apu_reg_io_addr == 0x7 && apu_reg_access_w)
            {
                sq2_ignore_reload = 1;
            }
        }
    }
    // Sweep unit
    sq2_sweep_counter--;
    if (sq2_sweep_counter == 0)
    {
        sq2_sweep_counter = sq2_sweep_devider_period + 1;
        if (sq2_sweep_enable && sq2_sweep_shift_count > 0 && sq2_valid_freq)
        {
            sq2_sweep_change = sq2_timer >> sq2_sweep_shift_count;
            sq2_timer += sq2_sweep_negate ? -sq2_sweep_change : sq2_sweep_change;
            SQ2CalculateValidFreq();
        }
    }
    else if (sq2_sweep_reload)
    {
        sq2_sweep_counter = sq2_sweep_devider_period + 1;
        sq2_sweep_reload = 0;
    }
}
void SC_Square2ClockEnvelope()
{
    if (sq2_envelope_start_flag)
    {
        sq2_envelope_start_flag = 0;
        sq2_envelope_decay_level_counter = 15;
        sq2_envelope_devider = (sq2_volume_devider_period + 1)&0xFF;
    }
    else
    {
        if (sq2_envelope_devider > 0)
            sq2_envelope_devider--;
        else
        {
            sq2_envelope_devider = (sq2_volume_devider_period + 1)&0xFF;
            if (sq2_envelope_decay_level_counter > 0)
                sq2_envelope_decay_level_counter--;
            else if (sq2_length_halt)
                sq2_envelope_decay_level_counter = 0xF;
        }
    }
    sq2_envelope = sq2_constant_volume_envelope ? sq2_volume_devider_period : sq2_envelope_decay_level_counter;
}

void APUOnRegister4004()
{
    // Only writes accepted
    if (!apu_reg_access_w)
        return;
    sq2_duty_cycle = (apu_reg_io_db & 0xC0) >> 6;
    sq2_volume_devider_period = apu_reg_io_db & 0xF;
    sq2_length_halt = (apu_reg_io_db & 0x20) != 0;
    sq2_constant_volume_envelope = (apu_reg_io_db & 0x10) != 0;

    sq2_envelope = sq2_constant_volume_envelope ? sq2_volume_devider_period : sq2_envelope_decay_level_counter;
}
void APUOnRegister4005()
{
    // Only writes accepted
    if (!apu_reg_access_w)
        return;

    sq2_sweep_enable = (apu_reg_io_db & 0x80) == 0x80;
    sq2_sweep_devider_period = (apu_reg_io_db >> 4) & 7;
    sq2_sweep_negate = (apu_reg_io_db & 0x8) == 0x8;
    sq2_sweep_shift_count = apu_reg_io_db & 7;
    sq2_sweep_reload = 1;
    SQ2CalculateValidFreq();
}
void APUOnRegister4006()
{
    // Only writes accepted
    if (!apu_reg_access_w)
        return;
    sq2_timer = (sq2_timer & 0xFF00) | apu_reg_io_db;
    SQ2CalculateValidFreq();
}
void APUOnRegister4007()
{
    // Only writes accepted
    if (!apu_reg_access_w)
        return;

    sq2_timer = (sq2_timer & 0x00FF) | ((apu_reg_io_db & 0x7) << 8);

    if (sq2_length_enabled && !sq2_ignore_reload)
        sq2_length_counter = sq2_duration_table[apu_reg_io_db >> 3];
    if (sq2_ignore_reload)
        sq2_ignore_reload = 0;

    sq2_seqencer = 0;
    sq2_envelope_start_flag = 1;
    SQ2CalculateValidFreq();
}

void SC_Square2OnRegister4015()
{
    sq2_length_enabled = (apu_reg_io_db & 0x02) != 0;
    if (!sq2_length_enabled)
        sq2_length_counter = 0;
}
char SC_Square2ReadRegister4015()
{
    if (sq2_length_counter > 0)
        apu_reg_io_db = (apu_reg_io_db & 0xFD) | 0x02;
}
