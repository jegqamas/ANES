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

#include "SC_VRC6_Pulse2.h"
#include "../APU.h"

int sc_vrc6_p2_dutyForm;
int sc_vrc6_p2_dutyStep;
char sc_vrc6_p2_enabled;
char sc_vrc6_p2_mode;
unsigned char sc_vrc6_p2_volume;
int sc_vrc6_p2_freqTimer;
int sc_vrc6_p2_frequency;
int sc_vrc6_p2_cycles;

void SC_VRC6_P2_HardReset()
{
    sc_vrc6_p2_dutyForm = 0;
    sc_vrc6_p2_dutyStep = 0xF;
    sc_vrc6_p2_enabled = 1;
    sc_vrc6_p2_mode = 0;
    sc_vrc6_p2_output = 0;
}
void SC_VRC6_P2_ClockSingle()
{
    if (--sc_vrc6_p2_cycles <= 0)
    {
        sc_vrc6_p2_cycles = (sc_vrc6_p2_frequency << 1) + 2;

        if (sc_vrc6_p2_enabled)
        {
            if (sc_vrc6_p2_mode)
                sc_vrc6_p2_output = sc_vrc6_p2_volume;
            else
            {
                sc_vrc6_p2_dutyStep--;
                if (sc_vrc6_p2_dutyStep < 0)
                    sc_vrc6_p2_dutyStep = 0xF;

                if (sc_vrc6_p2_dutyStep <= sc_vrc6_p2_dutyForm)
                    sc_vrc6_p2_output = sc_vrc6_p2_volume;
                else
                    sc_vrc6_p2_output = 0;
            }
        }
    }
}

void SC_VRC6_P2_Write0(unsigned char *value)
{
    sc_vrc6_p2_mode = ((*value) & 0x80) == 0x80;
    sc_vrc6_p2_dutyForm = (((*value)  & 0x70) >> 4);
    sc_vrc6_p2_volume = ((*value)  & 0xF);
}
void SC_VRC6_P2_Write1(unsigned char *value)
{
    sc_vrc6_p2_frequency = (sc_vrc6_p2_frequency & 0x0F00) | (*value);
}
void SC_VRC6_P2_Write2(unsigned char *value)
{
    sc_vrc6_p2_frequency = (sc_vrc6_p2_frequency & 0x00FF) | (((*value) & 0xF) << 8);
    sc_vrc6_p2_enabled = ((*value) & 0x80) == 0x80;
}
