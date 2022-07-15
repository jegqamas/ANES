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
#include "SC_DMC.h"
#include "DMA.h"
#include "Memory.h"
#include "NES.h"
#include "APU.h"

int dmc_freq_table[3][16] =
{
    //NTSC
    {
        428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106,  84,  72,  54
    },
    //PAL
    {
        398, 354, 316, 298, 276, 236, 210, 198, 176, 148, 132, 118,  98,  78,  66,  50
    },
    //DENDY (same as ntsc for now)
    {
        428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106,  84,  72,  54
    },
};
int dmc_output_a;
int dmc_period_devider;
char dmc_irq_enabled;
char dmc_loop_flag;
unsigned char dmc_rate_index;
unsigned short dmc_addr_refresh;
int dmc_size_refresh;

char dmc_dmaEnabled;
unsigned char dmc_dmaByte;
int dmc_dmaBits;
char dmc_bufferFull;
unsigned char dmc_dmaBuffer;
int dmc_dmaSize;
unsigned short dmc_dmaAddr;
char system_index;



void DMCHardReset(int tv_format)
{
    system_index=tv_format;
    dmc_output_a = 0;
    dmc_output = 0;
    dmc_period_devider = 0;
    dmc_loop_flag = 0;
    dmc_rate_index = 0;

    dmc_irq_enabled = 0;
    dmc_dmaAddr = 0xC000;
    dmc_addr_refresh = 0xC000;
    dmc_size_refresh = 0;
    dmc_dmaBits = 1;
    dmc_dmaByte = 1;
    dmc_period_devider = 0;
    dmc_dmaEnabled = 0;
    dmc_bufferFull = 0;
    dmc_dmaSize = 0;
    apu_irq_delta_occur=0;
}
void DMCSoftReset()
{
    DMCHardReset(system_index);
}

void DMCClock()
{
    dmc_period_devider--;
    if (dmc_period_devider <= 0)
    {
        dmc_period_devider = dmc_freq_table[system_index][dmc_rate_index];
        if (dmc_dmaEnabled)
        {
            if ((dmc_dmaByte & 0x01) != 0)
            {
                if (dmc_output_a <= 0x7D)
                    dmc_output_a += 2;
            }
            else
            {
                if (dmc_output_a >= 0x02)
                    dmc_output_a -= 2;
            }
            dmc_dmaByte >>= 1;
        }
        dmc_dmaBits--;
        if (dmc_dmaBits == 0)
        {
            dmc_dmaBits = 8;
            if (dmc_bufferFull)
            {
                dmc_bufferFull = 0;
                dmc_dmaEnabled = 1;
                dmc_dmaByte = dmc_dmaBuffer;
                // RDY ?
                if (dmc_dmaSize > 0)
                {
                    AssertDMCDMA();
                }
            }
            else
            {
                dmc_dmaEnabled = 0;
            }
        }
        //if (audio_dmc_outputable)
        //{
        dmc_output = dmc_output_a;
        //}
    }
}
void DMCDoDMA()
{
    dmc_bufferFull = 1;

    MEM_CPURead(&dmc_dmaAddr, &dmc_dmaBuffer);

    if (dmc_dmaAddr == 0xFFFF)
        dmc_dmaAddr = 0x8000;
    else
        dmc_dmaAddr++;

    if (dmc_dmaSize > 0)
        dmc_dmaSize--;

    if (dmc_dmaSize == 0)
    {
        if (dmc_loop_flag)
        {
            dmc_dmaSize = dmc_size_refresh;
            dmc_dmaAddr = dmc_addr_refresh;
        }
        else if (dmc_irq_enabled)
        {
            NES_IRQFlags |= IRQ_FLAG_DMC;
            apu_irq_delta_occur = 1;
        }
    }
}
void APUOnRegister4010()
{
    if (!apu_reg_access_w)
        return;
    dmc_irq_enabled = (apu_reg_io_db & 0x80) != 0;
    dmc_loop_flag = (apu_reg_io_db & 0x40) != 0;

    if (!dmc_irq_enabled)
    {
        apu_irq_delta_occur = 0;
        NES_IRQFlags &= IRQ_FLAG_DMC_OFF;
    }
    dmc_rate_index = apu_reg_io_db & 0x0F;
}
void APUOnRegister4011()
{
    if (!apu_reg_access_w)
        return;
    dmc_output_a =  apu_reg_io_db & 0x7F;
}
void APUOnRegister4012()
{
    if (!apu_reg_access_w)
        return;
    dmc_addr_refresh = (apu_reg_io_db  << 6) | 0xC000;
}
void APUOnRegister4013()
{
    if (!apu_reg_access_w)
        return;
    dmc_size_refresh = (apu_reg_io_db << 4) | 0x0001;
}
void DMCOn4015()
{
    // Disable DMC IRQ
    apu_irq_delta_occur = 0;
    NES_IRQFlags &= IRQ_FLAG_DMC_OFF;
}
void DMCRead4015()
{
    if (dmc_dmaSize > 0)
        apu_reg_io_db = (apu_reg_io_db & 0xEF) | 0x10;
}
void DMCWrite4015(unsigned short *address, unsigned char *value)
{
    // DMC DMA
    if (((*value) & 0x10) != 0)
    {
        if (dmc_dmaSize == 0)
        {
            dmc_dmaSize = dmc_size_refresh;
            dmc_dmaAddr = dmc_addr_refresh;
        }
    }
    else
    {
        dmc_dmaSize = 0;
    }

    if (!dmc_bufferFull && dmc_dmaSize > 0)
    {
        AssertDMCDMA();
    }
}
