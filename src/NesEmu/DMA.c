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

#include "DMA.h"
#include "NES.h"
#include "Memory.h"
#include "APU.h"
#include "SC_DMC.h"

int dma_DMCDMAWaitCycles;
int dma_OAMDMAWaitCycles;
char dma_isOamDma;
int dma_oamdma_i;
char dma_DMCOn;
char dma_OAMOn;
char dma_DMC_occurring;
char dma_OAM_occurring;
int dma_OAMFinishCounter;
unsigned short dma_Oamaddress;
int dma_OAMCYCLE;
unsigned char dma_latch;
unsigned char dma_dummy;
unsigned short reg_2004;

void DMAHardReset()
{
    dma_DMCDMAWaitCycles = 0;
    dma_OAMDMAWaitCycles = 0;
    dma_isOamDma = 0;
    dma_oamdma_i = 0;
    dma_DMCOn = 0;
    dma_OAMOn = 0;
    dma_DMC_occurring = 0;
    dma_OAM_occurring = 0;
    dma_OAMFinishCounter = 0;
    dma_Oamaddress = 0;
    dma_OAMCYCLE = 0;
    dma_latch = 0;
    reg_2004 = 0x2004;
}
void DMASoftReset()
{
    DMAHardReset();
}

void AssertDMCDMA()
{
    if (dma_OAM_occurring)
    {
        if (dma_OAMCYCLE < 508)
            // OAM DMA is occurring here
            dma_DMCDMAWaitCycles = MEM_CPU_BUS_W ? 1 : 0;
        else
        {
            // Here the oam dma is about to finish
            // Remaining cycles of oam dma determines the dmc dma waiting cycles.
            dma_DMCDMAWaitCycles = 4 - (512 - dma_OAMCYCLE);
        }
    }
    else if (dma_DMC_occurring)
    {
        // DMC occurring now !!? is that possible ?
        // Anyway, let's ignore this call !
        return;
    }
    else
    {
        // Nothing occurring, initialize brand new dma
        // DMC DMA depends on r/w flag for the wait cycles.
        dma_DMCDMAWaitCycles = MEM_CPU_BUS_W ? 3 : 2;
        // After 2 cycles of oam dma, add extra cycle for the waiting.
        if (dma_OAMFinishCounter == 3)
            dma_DMCDMAWaitCycles++;
    }
    dma_isOamDma = 0;
    dma_DMCOn = 1;
}
void AssertOAMDMA(unsigned char *value)
{
    dma_Oamaddress = ((*value) << 8);

    if (dma_OAM_occurring) return;


    // Setup
    // OAM DMA depends on apu odd timer for odd cycles
    dma_OAMDMAWaitCycles = apu_odd_cycle ? 1 : 2;

    dma_isOamDma = 1;
    dma_OAMOn = 1;
}

void DMAClock()
{
    if (dma_OAMFinishCounter > 0)
        dma_OAMFinishCounter--;
    if (!MEM_CPU_BUS_W)// Clocks only on reads
    {
        if (dma_DMCDMAWaitCycles > 0)
            dma_DMCDMAWaitCycles--;
        if (dma_OAMDMAWaitCycles > 0)
            dma_OAMDMAWaitCycles--;
        return;
    }
    if (dma_DMCOn)
    {
        dma_DMC_occurring = 1;
        // This is it !
        dma_DMCOn = 0;
        // Do wait cycles (extra reads)
        if (dma_DMCDMAWaitCycles > 0)
        {
            if (MEM_CPU_BUS_ADDRESS == 0x4016 || MEM_CPU_BUS_ADDRESS == 0x4017)
            {
                MEM_CPURead(&MEM_CPU_BUS_ADDRESS, &dma_dummy);
                dma_DMCDMAWaitCycles--;

                while (dma_DMCDMAWaitCycles > 0)
                {
                    NESClockComponents();
                    dma_DMCDMAWaitCycles--;
                }
            }
            else
            {
                if (dma_DMCDMAWaitCycles > 0)
                {
                    NESClockComponents();
                    dma_DMCDMAWaitCycles--;
                }
                while (dma_DMCDMAWaitCycles > 0)
                {
                    MEM_CPURead(&MEM_CPU_BUS_ADDRESS, &dma_dummy);
                    dma_DMCDMAWaitCycles--;
                }
            }
        }
        // Do DMC DMA
        DMCDoDMA();

        dma_DMC_occurring = 0;
    }
    if (dma_OAMOn)
    {
        dma_OAM_occurring = 1;
        // This is it ! pause the cpu
        dma_OAMOn = 0;
        // Do wait cycles (extra reads)
        if (dma_OAMDMAWaitCycles > 0)
        {
            if (MEM_CPU_BUS_ADDRESS == 0x4016 || MEM_CPU_BUS_ADDRESS == 0x4017)
            {
                MEM_CPURead(&MEM_CPU_BUS_ADDRESS, &dma_dummy);
                dma_OAMDMAWaitCycles--;

                while (dma_OAMDMAWaitCycles > 0)
                {
                    NESClockComponents();
                    dma_OAMDMAWaitCycles--;
                }
            }
            else
            {
                if (dma_OAMDMAWaitCycles > 0)
                {
                    NESClockComponents();
                    dma_OAMDMAWaitCycles--;
                }
                while (dma_OAMDMAWaitCycles > 0)
                {
                    MEM_CPURead(&MEM_CPU_BUS_ADDRESS, &dma_dummy);
                    dma_OAMDMAWaitCycles--;
                }
            }
        }

        // Do OAM DMA
        dma_OAMCYCLE = 0;
        for (dma_oamdma_i = 0; dma_oamdma_i < 256; dma_oamdma_i++)
        {
            MEM_CPURead(&dma_Oamaddress, &dma_latch);
            dma_OAMCYCLE++;
            MEM_CPUWrite(&reg_2004, &dma_latch);
            dma_OAMCYCLE++;
            dma_Oamaddress = (++dma_Oamaddress) & 0xFFFF;
        }
        dma_OAMCYCLE = 0;
        dma_OAMFinishCounter = 5;
        dma_OAM_occurring = 0;
    }
}
