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
#include "../NES.h"
#include "../DIFINES.h"
#include "SC_MMC5_PCM.h"

char mmc5_pcm_readMode;
char mmc5_pcm_PCMIRQenable;
char mmc5_pcm_irqTrip;
unsigned char mmc5_pcm_tempdata;

void SC_MMC5_PCMHardReset()
{
    mmc5_pcm_output = 0;
    mmc5_pcm_readMode = 0;
    mmc5_pcm_PCMIRQenable = 0;
    mmc5_pcm_irqTrip = 0;
    mmc5_pcm_tempdata=0;
}
void SC_MMC5_PCMSoftReset()
{
    SC_MMC5_PCMHardReset();
}

void SC_MMC5_PCMWrite5010(unsigned char *value)
{
    mmc5_pcm_readMode = ((*value) & 0x1) == 0x1;
    mmc5_pcm_PCMIRQenable = ((*value) & 0x80) == 0x80;
    // Update irq
    if (mmc5_pcm_PCMIRQenable && mmc5_pcm_irqTrip)
        NES_IRQFlags |= IRQ_FLAG_BOARD;
}
void SC_MMC5_PCMWrite5011(unsigned char *value)
{
    if (! mmc5_pcm_readMode)
    {
        if ((*value) == 0)
        {
            mmc5_pcm_irqTrip = 1;
        }
        else
        {
            mmc5_pcm_irqTrip = 0;
            mmc5_pcm_output = (*value);
        }
        // Update irq
        if ( mmc5_pcm_PCMIRQenable &&  mmc5_pcm_irqTrip)
            NES_IRQFlags |= IRQ_FLAG_BOARD;
    }
}
unsigned char SC_MMC5_PCMRead5010()
{
    mmc5_pcm_tempdata = (mmc5_pcm_irqTrip & mmc5_pcm_PCMIRQenable) ? 0x80 : 0;

    mmc5_pcm_irqTrip = 0;
    NES_IRQFlags &= IRQ_FLAG_BOARD_OFF;
    return mmc5_pcm_tempdata;
}
