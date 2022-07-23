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

#include "NES.h"
#include "DIFINES.h"
#include "ROM/INES.h"
#include "Mappers/_Mappers.h"
#include "APU.h"
#include "CPU.h"
#include "PPU.h"
#include "DMA.h"
#include "Memory.h"
#include "MappersInitiailizer.h"

char pal_add_cycle;
void(*mem_board_on_cpu_clock)();
void(*mem_board_hard_reset)();
void(*mem_board_soft_reset)();
void NESSetupMapperPointers(void (*board_on_cpu_clock)(),void (*board_hard_reset)(),void (*board_soft_reset)())
{
    mem_board_on_cpu_clock=board_on_cpu_clock;
    mem_board_hard_reset=board_hard_reset;
    mem_board_soft_reset=board_soft_reset;
}
void NESInitialize(int game_source_mode, int audio_freq)
{
// Setup pointers
    switch(game_source_mode)
    {
    case GAME_SOURCE_INES:
    {
        // Load file using ines
        if (INES_IsValid)
        {
            // initialize
            NES_GameLoadedName=INES_FileName;

            NES_IRQFlags=0;
            // TODO: setup audio frequency
            NESClockComponents=ClockComponentsBlank;
            NES_AudioFrequency=audio_freq;

            NES_Mirroring=INES_Mirroring;

            // TV FORMAT TODO: setup tv format
            NES_TVFormat=TVFORMAT_NTSC;
            NES_TargetFPS=60.0988;

            // Mapper
            InitializeMappers(INES_MapperNumber);

            // Put dumps into arrays
            MEM_SET_PRG_DUMP(prg_dump,INES_PRGSizeInBytes);
            MEM_SET_CHR_DUMP(chr_dump,INES_CHRSizeInBytes);
            MEM_SET_TRAINER_DUMP(trn_dump,INES_TrainerSizeInBytes);


            // PPU
            PPUInitialize();
            ppu_frame_finished=0;
            // APU
            APUInitialize();

// Set mapper
            SetupMapper(INES_MapperNumber);


            // Close ines, not needed anymore
            INES_Close();

            NESHardReset();

            NESClockComponents=NESClockComponentsNTSC;
            NES_IsGameLoaded=1;
        }
        break;
    }
    }
}
void NESHardReset()
{
    NES_IRQFlags=0;
    MEM_HardReset();
    // Mapper hard reset first
    mem_board_hard_reset();

    CPUHardReset();
    PPUHardReset(NES_TVFormat);
    APUHardReset(NES_TVFormat,NES_AudioFrequency);
    DMAHardReset();
}
void NESSoftReset()
{
    NES_IRQFlags=0;
    MEM_HardReset();
    // Mapper soft reset first
    mem_board_hard_reset();

    CPUSoftReset();
    PPUSoftReset();
    APUSoftReset();
    DMASoftReset();
}

void NESClock()
{
    while(NES_ON)
    {
        if (!NES_PAUSED)
        {
            CPUClock();
        }
    }
}
void NESClockFrame()
{
    audio_w_pos = 0;
    audio_r_pos = 0;
    audio_samples_added = 0;
    audio_timer = 0;
    while(!ppu_frame_finished)
    {
        if (!NES_PAUSED)
        {
            CPUClock();
        }
        if (ppu_frame_finished)
        {
            ppu_frame_finished=0;

            break;
        }
    }
}
void NESClockComponentsNTSC()
{
    PPUClock();

    NESCheckInterrupts();// On the second half of memory access time...

    PPUClock();
    PPUClock();
    APUClock();
    DMAClock();

    mem_board_on_cpu_clock();
}
void NESClockComponentsPALB()
{
    PPUClock();

    NESCheckInterrupts();// On the second half of memory access time...

    PPUClock();
    PPUClock();
    pal_add_cycle++;
    if (pal_add_cycle == 5)
    {
        pal_add_cycle = 0;
        PPUClock();

        //In pal, ppu does 3.2 per 1 cpu cycle
        //here, every 5 cpu cycles, the ppu
        //will do 1 additional cycle
        //0.2 * 5 = 1
    }
    APUClock();
    DMAClock();

    mem_board_on_cpu_clock();
}
void ClockComponentsBlank()
{

}
void NESCheckInterrupts()
{
    if (!cpu_suspend_nmi)
    {
        // The edge detector, see if nmi occurred.
        if (PPU_NMI_Current & !PPU_NMI_Old) // Raising edge, set nmi request
            CPU_NMI_PIN = 1;
        PPU_NMI_Old = PPU_NMI_Current = 0;// NMI detected or not, low both lines for this form ___|-|__
    }
    if (!cpu_suspend_irq)
    {
        // irq level detector
        CPU_IRQ_PIN = (!cpu_flag_i && NES_IRQFlags != 0);
    }
}

