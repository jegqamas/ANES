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

#include "SC_VRC6_Sawtooth.h"
#include "../APU.h"

unsigned char sc_vrc6_st_AccumRate;
int sc_vrc6_st_accumClock;
unsigned char sc_vrc6_st_accumulationRegister;
int sc_vrc6_st_frequency;
int sc_vrc6_st_freqTimer;
int sc_vrc6_st_cycles;
char sc_vrc6_st_enabled;


void SC_VRC6_ST_HardReset()
{

}
void SC_VRC6_ST_ClockSingle()
{
    if (--sc_vrc6_st_cycles <= 0)
    {
        sc_vrc6_st_cycles = sc_vrc6_st_freqTimer;

        if (sc_vrc6_st_enabled)
        {
            sc_vrc6_st_accumClock++;
            switch (++sc_vrc6_st_accumClock)
            {
            case 2:
            case 4:
            case 6:
            case 8:
            case 10:
            case 12:
                sc_vrc6_st_accumulationRegister += sc_vrc6_st_AccumRate;
                break;

            case 14:
                sc_vrc6_st_accumulationRegister = 0;
                sc_vrc6_st_accumClock = 0;
                break;
            }
            sc_vrc6_st_output = ((sc_vrc6_st_accumulationRegister >> 3) & 0x1F);

        }
        else sc_vrc6_st_output = 0;
    }
}

void SC_VRC6_ST_UpdateFrequency()
{
    sc_vrc6_st_freqTimer = (sc_vrc6_st_frequency + 1) * 2;
}

void SC_VRC6_ST_Write0(unsigned char *value)
{
    sc_vrc6_st_AccumRate = ((*value) & 0x3F);
}
void SC_VRC6_ST_Write1(unsigned char *value)
{
    sc_vrc6_st_frequency = (sc_vrc6_st_frequency & 0x0F00) | (*value);
    SC_VRC6_ST_UpdateFrequency();
}
void SC_VRC6_ST_Write2(unsigned char *value)
{
    sc_vrc6_st_frequency = (sc_vrc6_st_frequency & 0x00FF) | (((*value) & 0xF) << 8);
    sc_vrc6_st_enabled = ((*value) & 0x80) == 0x80;
    SC_VRC6_ST_UpdateFrequency();
}
