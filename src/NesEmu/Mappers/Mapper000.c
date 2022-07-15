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
#include "_Mappers.h"
#include "../Memory.h"
#include "../DIFINES.h"
#include "../NES.h"
/*
Mapper 0 is the default nes configuration, these methods will always be linked
into nes component by default, unless other mapper needs to overrode them (or one of them).

*/
void Mapper000HardReset()
{
    printf("ANES: HARD RESET MAPPER 000 \n");

// 1 Setup prg
    MEM_SWITCH_PRG_08KB_RAM(PRG_AREA_4000,1);
    MEM_SWITCH_PRG_08KB(PRG_AREA_4000, mem_prg_ram_08kb_mask);
    MEM_TOGGLE_PRG_08KB_RAM_ENABLE(PRG_AREA_4000,1);// make it enabled
    MEM_TOGGLE_PRG_08KB_RAM_WRITABLE(PRG_AREA_4000,1);// make it writable
    MEM_TOGGLE_PRG_08KB_RAM_BATTERY(PRG_AREA_4000,1);// make it battery so it will be saved into file
// $6000 to 7FFF is sram
    MEM_SWITCH_PRG_08KB_RAM(PRG_AREA_6000,1);// Set it ram
    MEM_SWITCH_PRG_08KB(PRG_AREA_6000,0);// setup indexes
    MEM_TOGGLE_PRG_08KB_RAM_ENABLE(PRG_AREA_6000,1);// make it enabled
    MEM_TOGGLE_PRG_08KB_RAM_WRITABLE(PRG_AREA_6000,1);// make it writable
    MEM_TOGGLE_PRG_08KB_RAM_BATTERY(PRG_AREA_6000,1);// make it battery so it will be saved into file

// $8000 to FFFF is prg rom
    MEM_SWITCH_PRG_32KB_RAM(PRG_AREA_8000,0);// Set it not ram
    MEM_SWITCH_PRG_32KB(PRG_AREA_8000,0);// setup indexes
// Rom banks are not effected by other settings

// CHR
    MEM_SWITCH_CHR_08KB_RAM(CHR_AREA_0000, mem_chr_rom_01kb_count==0);// Depends on game information
    MEM_SWITCH_CHR_08KB(CHR_AREA_0000,0);
    MEM_TOGGLE_CHR_08KB_RAM_ENABLE(CHR_AREA_0000,1);// make it enabled
    MEM_TOGGLE_CHR_08KB_RAM_WRITABLE(CHR_AREA_0000,1);// make it writable
    MEM_TOGGLE_CHR_08KB_RAM_BATTERY(CHR_AREA_0000,1);// make it battery so it will be saved into file

// NMT
    MEM_SWITCH_NMT_FROM_MIRORING(NES_Mirroring);

    printf("ANES: MAPPER 000 HARD RESET SUCCESS \n");

    for (int i=0; i<16; i++)
    {

        printf("ANES: PRG BLOCK IS RAM %d = %d \n",i,mem_prg_block_ram[i]);
    }
    for (int i=0; i<16; i++)
    {

        printf("ANES: PRG ROM INDEX %d = %d \n",i,mem_prg_rom_block_index[i]);
    }
    for (int i=0; i<16; i++)
    {

        printf("ANES: PRG RAM INDEX %d = %d \n",i,mem_prg_ram_block_index[i]);
    }
    for (int i=0; i<8; i++)
    {

        printf("ANES: CHR BLOCK IS RAM %d = %d \n",i,mem_chr_block_ram[i]);
    }

    for (int i=0; i<8; i++)
    {

        printf("ANES: CHR ROM INDEX %d = %d \n",i,mem_chr_rom_block_index[i]);
    }
    for (int i=0; i<8; i++)
    {

        printf("ANES: CHR RAM INDEX %d = %d \n",i,mem_chr_ram_block_index[i]);
    }
    for (int i=0; i<4; i++)
    {

        printf("ANES: NMT INDEX %d = %d \n",i,mem_nmt_block_index[i]);
    }
}
void Mapper000SoftReset() {}

// These methods should be linked using pointers in nes components
// EX is between $4020 - $7FFF
void Mapper000_WriteEX(unsigned short *address, unsigned char *value)
{
    //if ((*value)!=0)
    //    printf("MAPPER 000 EX WRITE: addr %d = %c \n",(*address),(*value));
}
void Mapper000_ReadEX(unsigned short *address, unsigned char *value) {}
// PR is between $8000 - $FFFF
void Mapper000_WritePR(unsigned short *address, unsigned char *value)
{

}
void Mapper000_ReadPR(unsigned short *address, unsigned char *value)
{

}
// CHR $0000 - $1FFF
void Mapper000_WriteCHR(unsigned short *address, unsigned char *value) {}
void Mapper000_ReadCHR(unsigned short *address, unsigned char *value) {}
// NMT $2000 - $3EFF
void Mapper000_WriteNMT(unsigned short *address, unsigned char *value) {}
void Mapper000_ReadNMT(unsigned short *address, unsigned char *value) {}
// PAL $3F00 - $3FFF
void Mapper000_WritePAL(unsigned short *address, unsigned char *value) {}
void Mapper000_ReadPAL(unsigned short *address, unsigned char *value) {}
// Clocks
void Mapper000_OnCPUClock() {}
void Mapper000_OnPPUClock() {}
void Mapper000_OnPPUScanlineClock() {}
void Mapper000_OnPPUAddressUpdate(unsigned short *address) {}

void Mapper000_OnAPUGetAudioSample(int *board_sample) {}
void Mapper000_OnAPUClockSingle() {}
void Mapper000_OnAPUClock() {}
void Mapper000_OnAPUClockLength() {}
void Mapper000_OnAPUClockEnvelope() {}
