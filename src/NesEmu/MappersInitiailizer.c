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
#include "Mappers/_Mappers.h"
#include "MappersInitiailizer.h"
#include "Memory.h"
#include "PPU.h"
#include "NES.h"
#include "APU.h"

void InitializeMappers(int mapper_number)
{
    // Setup how much memory should be located as RAM
    // Default settings is 16KB PRG RAM and 8KB CHR RAM
    DEFAULT_PRG_RAM_1KB_COUNT= 16;
    DEFAULT_CHR_RAM_1KB_COUNT= 128;

    // Depending on mapper ...
    // TODO: find a way to setup RAM without swithcing
    switch(mapper_number)
    {
    case 1:
    {
        DEFAULT_PRG_RAM_1KB_COUNT= 64;
        DEFAULT_CHR_RAM_1KB_COUNT= 64;
        break;
    }
    case 5:
    {
        DEFAULT_PRG_RAM_1KB_COUNT= 64;
        DEFAULT_CHR_RAM_1KB_COUNT= 16;
        break;
    }
    }
}
void SetupMapper(int mapper_number)
{
    // First set configuration of mapper 0 as default
    MEM_SET_MAPPER_CHR(Mapper000_WriteCHR,Mapper000_ReadCHR);
    MEM_SET_MAPPER_NMT(Mapper000_WriteNMT,Mapper000_ReadNMT);
    MEM_SET_MAPPER_PAL(Mapper000_WritePAL,Mapper000_ReadPAL);
    MEM_SET_MAPPER_PRG_EX(Mapper000_WriteEX,Mapper000_ReadEX);
    MEM_SET_MAPPER_PRG_PR(Mapper000_WritePR,Mapper000_ReadPR);

    PPUSetupMapper(Mapper000_OnPPUClock,Mapper000_OnPPUScanlineClock,Mapper000_OnPPUAddressUpdate);

    APUSetupMapper(Mapper000_OnAPUGetAudioSample,Mapper000_OnAPUClockLength,Mapper000_OnAPUClockEnvelope,Mapper000_OnAPUClock, Mapper000_OnAPUClockSingle);

    NESSetupMapperPointers(Mapper000_OnCPUClock,Mapper000HardReset,Mapper000SoftReset);

    IsExternalSoundChannelsEnabled=0;

    switch (mapper_number)
    {
    case 1:
    {
        // Depending on game, setup mapper configuration !
        NESSetupMapperPointers(Mapper001_OnCPUClock,Mapper001HardReset,Mapper000SoftReset);
        MEM_SET_MAPPER_PRG_PR(Mapper001_WritePR,Mapper000_ReadPR);
        break;
    }
    case 2:
    {
        NESSetupMapperPointers(Mapper000_OnCPUClock,Mapper002HardReset,Mapper000SoftReset);
        MEM_SET_MAPPER_PRG_PR(Mapper002_WritePR,Mapper000_ReadPR);
        break;
    }
    case 3:
    {
        MEM_SET_MAPPER_PRG_PR(Mapper003_WritePR,Mapper000_ReadPR);
        break;
    }
    case 4:
    {
        // Depending on game, setup mapper configuration !
        NESSetupMapperPointers(Mapper000_OnCPUClock,Mapper004HardReset,Mapper000SoftReset);
        MEM_SET_MAPPER_PRG_PR(Mapper004_WritePR,Mapper000_ReadPR);
        PPUSetupMapper(Mapper004_OnPPUClock,Mapper000_OnPPUScanlineClock,Mapper004_OnPPUAddressUpdate);
        break;
    }
    case 5:
    {
        // Depending on game, setup mapper configuration !
        MEM_SET_MAPPER_PRG_EX(Mapper005_WriteEX,Mapper005_ReadEX);
        MEM_SET_MAPPER_CHR(Mapper000_WriteCHR,Mapper005_ReadCHR);
        MEM_SET_MAPPER_NMT(Mapper005_WriteNMT,Mapper005_ReadNMT);
        PPUSetupMapper(Mapper000_OnPPUClock,Mapper005_OnPPUScanlineClock,Mapper000_OnPPUAddressUpdate);
        NESSetupMapperPointers(Mapper000_OnCPUClock,Mapper005HardReset,Mapper000SoftReset);
        APUSetupMapper(Mapper005_OnAPUGetAudioSample,Mapper005_OnAPUClockLength,Mapper005_OnAPUClockEnvelope,Mapper005_OnAPUClock,Mapper000_OnAPUClockSingle);

        IsExternalSoundChannelsEnabled=1;
        break;
    }
    case 7:
    {
        // Depending on game, setup mapper configuration ! Enable bus conflict for battletoads
        MEM_SET_MAPPER_PRG_PR(Mapper007_WritePR,Mapper000_ReadPR);
        break;
    }
    case 9:
    {
        NESSetupMapperPointers(Mapper000_OnCPUClock,Mapper009HardReset,Mapper000SoftReset);
        MEM_SET_MAPPER_PRG_PR(Mapper009_WritePR,Mapper000_ReadPR);
        MEM_SET_MAPPER_CHR(Mapper000_WriteCHR,Mapper009_ReadCHR);
        break;
    }
    case 10:
    {
        NESSetupMapperPointers(Mapper000_OnCPUClock,Mapper010HardReset,Mapper000SoftReset);
        MEM_SET_MAPPER_PRG_PR(Mapper010_WritePR,Mapper000_ReadPR);
        MEM_SET_MAPPER_CHR(Mapper000_WriteCHR,Mapper010_ReadCHR);
        break;
    }
    case 11:
    {
        MEM_SET_MAPPER_PRG_PR(Mapper011_WritePR, Mapper000_ReadPR);
        break;
    }
    case 13:
    {
        NESSetupMapperPointers(Mapper000_OnCPUClock, Mapper013HardReset, Mapper000SoftReset);
        MEM_SET_MAPPER_PRG_PR(Mapper013_WritePR,Mapper000_ReadPR);
        break;
    }
    case 15:
    {
        MEM_SET_MAPPER_PRG_PR(Mapper015_WritePR,Mapper000_ReadPR);
        break;
    }
    case 18:
    {
        NESSetupMapperPointers(Mapper018_OnCPUClock,Mapper018HardReset,Mapper000SoftReset);
        MEM_SET_MAPPER_PRG_PR(Mapper018_WritePR,Mapper000_ReadPR);
        break;
    }
    case 21:
    {
        NESSetupMapperPointers(Mapper021_OnCPUClock,Mapper021HardReset,Mapper000SoftReset);
        MEM_SET_MAPPER_PRG_PR(Mapper021_WritePR,Mapper000_ReadPR);
        break;
    }
    case 22:
    {
        NESSetupMapperPointers(Mapper000_OnCPUClock,Mapper022HardReset,Mapper000SoftReset);
        MEM_SET_MAPPER_PRG_PR(Mapper022_WritePR,Mapper000_ReadPR);
        break;
    }
    case 23:
    {
        MEM_SET_MAPPER_PRG_EX(Mapper023_WriteEX,Mapper023_ReadEX);
        NESSetupMapperPointers(Mapper000_OnCPUClock,Mapper023HardReset,Mapper000SoftReset);
        MEM_SET_MAPPER_PRG_PR(Mapper023_WritePR,Mapper000_ReadPR);
        break;
    }
    case 24:
    {
        NESSetupMapperPointers(Mapper024_OnCPUClock,Mapper024HardReset,Mapper000SoftReset);
        MEM_SET_MAPPER_PRG_PR(Mapper024_WritePR,Mapper000_ReadPR);
        APUSetupMapper(Mapper024_OnAPUGetAudioSample,Mapper000_OnAPUClockLength,Mapper000_OnAPUClockEnvelope,Mapper000_OnAPUClock,Mapper024_OnAPUClockSingle);
        IsExternalSoundChannelsEnabled=1;
        break;
    }
    case 25:
    {
        NESSetupMapperPointers(Mapper025_OnCPUClock,Mapper025HardReset,Mapper000SoftReset);
        MEM_SET_MAPPER_PRG_PR(Mapper025_WritePR,Mapper000_ReadPR);
        break;
    }
    case 26:
    {
        NESSetupMapperPointers(Mapper026_OnCPUClock,Mapper026HardReset,Mapper000SoftReset);
        MEM_SET_MAPPER_PRG_PR(Mapper026_WritePR,Mapper000_ReadPR);
        APUSetupMapper(Mapper026_OnAPUGetAudioSample,Mapper000_OnAPUClockLength,Mapper000_OnAPUClockEnvelope,Mapper000_OnAPUClock,Mapper026_OnAPUClockSingle);
        IsExternalSoundChannelsEnabled=1;
        break;
    }
    case 71:
    {
        NESSetupMapperPointers(Mapper000_OnCPUClock,Mapper071HardReset,Mapper000SoftReset);
        MEM_SET_MAPPER_PRG_PR(Mapper071_WritePR,Mapper000_ReadPR);
        break;
    }
    case 87:
    {
        MEM_SET_MAPPER_PRG_EX(Mapper087_WriteEX,Mapper000_ReadEX);
        break;
    }
    }
}
