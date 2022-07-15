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

#include "Memory.h"
#include "PPU.h"
#include "APU.h"
#include "NES.h"

unsigned int prg_block;
unsigned int prg_index_in_block;
unsigned int chr_block;
unsigned int chr_index_in_block;

// Pointers for board/mapper
// EX is between $4020 - $7FFF
void(*mapper_on_prg_ex_write)(unsigned short *address, unsigned char *value);
void(*mapper_on_prg_ex_read)(unsigned short *address, unsigned char *value);
// PR is between $8000 - $FFFF
void(*mapper_on_prg_pr_write)(unsigned short *address, unsigned char *value);
void(*mapper_on_prg_pr_read)(unsigned short *address, unsigned char *value);
// CHR $0000 - $1FFF
void(*mapper_on_chr_write)(unsigned short *address, unsigned char *value);
void(*mapper_on_chr_read)(unsigned short *address, unsigned char *value);
// CHR $0000 - $1FFF
void(*mapper_on_nmt_write)(unsigned short *address, unsigned char *value);
void(*mapper_on_nmt_read)(unsigned short *address, unsigned char *value);
// CHR $0000 - $1FFF
void(*mapper_on_pal_write)(unsigned short *address, unsigned char *value);
void(*mapper_on_pal_read)(unsigned short *address, unsigned char *value);

void MEM_HardReset()
{
    // memory
    for (int i=0; i<0x800; i++)
        mem_wram[i]=0;

    mem_wram[0x0008] = 0xF7;
    mem_wram[0x0008] = 0xF7;
    mem_wram[0x0009] = 0xEF;
    mem_wram[0x000A] = 0xDF;
    mem_wram[0x000F] = 0xBF;

    // Miscellaneous, real NES loads values similar to these during power up
    mem_palette_bank[0]= 0x09;
    mem_palette_bank[1]= 0x01;
    mem_palette_bank[2]= 0x00;
    mem_palette_bank[3]= 0x01;
    mem_palette_bank[4]= 0x00;
    mem_palette_bank[5]= 0x02;
    mem_palette_bank[6]= 0x02;
    mem_palette_bank[7]= 0x0D;
    mem_palette_bank[8]= 0x08;
    mem_palette_bank[9]= 0x10;
    mem_palette_bank[10]= 0x08;
    mem_palette_bank[11]= 0x24;
    mem_palette_bank[12]= 0x00;
    mem_palette_bank[13]= 0x00;
    mem_palette_bank[14]= 0x04;
    mem_palette_bank[15]= 0x2C;
    mem_palette_bank[16]= 0x09;
    mem_palette_bank[17]= 0x01;
    mem_palette_bank[18]= 0x34;
    mem_palette_bank[19]= 0x03;
    mem_palette_bank[20]= 0x00;
    mem_palette_bank[21]= 0x04;
    mem_palette_bank[22]= 0x00;
    mem_palette_bank[23]= 0x14;
    mem_palette_bank[24]= 0x08;
    mem_palette_bank[25]= 0x3A;
    mem_palette_bank[26]= 0x00;
    mem_palette_bank[27]= 0x02;
    mem_palette_bank[28]= 0x00;
    mem_palette_bank[29]= 0x20;
    mem_palette_bank[30]= 0x2C;
    mem_palette_bank[31]= 0x08;
    /*for (int i=0; i<16; i++)
    {
        mem_prg_rom_block_index[i]=i;
    }
    for (int i=0; i<16; i++)
    {
        mem_prg_ram_block_index[i]=i;
    }

    for (int i=0; i<8; i++)
    {
        mem_chr_rom_block_index[i]=i;
    }
    for (int i=0; i<8; i++)
    {
        mem_chr_ram_block_index[i]=i;
    }
    for (int i=0; i<4; i++)
    {
        mem_nmt_block_index[i]=i;
    }*/
}
void MEM_SET_PRG_DUMP(char *prg_dump, int prg_size)
{
// 1 setup prg configuration
    int prg_rom_size_KB = prg_size / 1024;
    mem_prg_rom_04kb_count = prg_rom_size_KB / 4;
    mem_prg_rom_08kb_count = prg_rom_size_KB / 8;
    mem_prg_rom_16kb_count = prg_rom_size_KB / 16;
    mem_prg_rom_32kb_count = prg_rom_size_KB / 32;

    mem_prg_rom_04kb_mask= mem_prg_rom_04kb_count - 1;
    mem_prg_rom_08kb_mask = mem_prg_rom_08kb_count - 1;
    mem_prg_rom_16kb_mask = mem_prg_rom_16kb_count - 1;
    mem_prg_rom_32kb_mask = mem_prg_rom_32kb_count - 1;

    // 2 copy content of prg rom dump
    mem_prg_rom_banks=(char **) malloc(mem_prg_rom_04kb_count* sizeof(char *));

    int index = 0;
    for (int i = 0; i < prg_size; i += 0x1000)
    {
        mem_prg_rom_banks[index] =(char *) malloc(0x1000 * sizeof(char));
        for (int j = 0; j < 0x1000; j++)
        {
            mem_prg_rom_banks[index][j] = prg_dump[i + j];
        }
        index++;
    }
    printf("ANES: PRG Dump copied success, %d bytes copied. \n",prg_size);
    printf("ANES: PRG ROM 4KB Count: %d \n",mem_prg_rom_04kb_count);
    printf("ANES: PRG ROM 8KB Count: %d  \n",mem_prg_rom_08kb_count);
    printf("ANES: PRG ROM 16KB Count: %d  \n",mem_prg_rom_16kb_count);
    printf("ANES: PRG ROM 32KB Count: %d  \n",mem_prg_rom_32kb_count);
    // 3 setup prg ram
    // TODO: use database to probably setup prg ram, for now, it uses 8 kb only.
    int prg_ram_size_KB = DEFAULT_PRG_RAM_1KB_COUNT;
    mem_prg_ram_04kb_count = prg_ram_size_KB / 4;
    mem_prg_ram_08kb_count = prg_ram_size_KB / 8;
    mem_prg_ram_16kb_count = prg_ram_size_KB / 16;
    mem_prg_ram_32kb_count = prg_ram_size_KB / 32;
    mem_prg_ram_04kb_mask= mem_prg_ram_04kb_count-1;
    mem_prg_ram_08kb_mask = mem_prg_ram_08kb_count - 1;
    mem_prg_ram_16kb_mask = mem_prg_ram_16kb_count - 1;
    mem_prg_ram_32kb_mask = mem_prg_ram_32kb_count - 1;

    mem_prg_ram_banks=(char **) malloc(mem_prg_ram_04kb_count* sizeof(char *));
    for (int i=0; i<mem_prg_ram_04kb_count; i++)
    {
        mem_prg_ram_banks[i] =(char *) malloc(0x1000 * sizeof(char));
    }

    printf("ANES: PRG RAM 4KB Count: %d \n",mem_prg_ram_04kb_count);
    printf("ANES: PRG RAM 8KB Count: %d  \n",mem_prg_ram_08kb_count);
    printf("ANES: PRG RAM 16KB Count: %d  \n",mem_prg_ram_16kb_count);
    printf("ANES: PRG RAM 32KB Count: %d  \n",mem_prg_ram_32kb_count);
    // Other setup is required to be done by mapper
}
void MEM_SET_CHR_DUMP(char *chr_dump, int chr_size)
{
    int chr_rom_size_KB = chr_size / 1024;
    mem_chr_rom_01kb_count = chr_rom_size_KB / 1;
    mem_chr_rom_02kb_count = mem_chr_rom_01kb_count / 2;
    mem_chr_rom_04kb_count = mem_chr_rom_01kb_count / 4;
    mem_chr_rom_08kb_count = mem_chr_rom_01kb_count / 8;
    mem_chr_rom_01kb_mask= mem_chr_rom_01kb_count-1;
    mem_chr_rom_02kb_mask = mem_chr_rom_02kb_count - 1;
    mem_chr_rom_04kb_mask = mem_chr_rom_04kb_count - 1;
    mem_chr_rom_08kb_mask = mem_chr_rom_08kb_count - 1;

    mem_chr_rom_banks=(char **) malloc(mem_chr_rom_01kb_count* sizeof(char *));
    int index = 0;
    for (int i = 0; i < chr_size; i += 0x400)
    {
        mem_chr_rom_banks[index] =(char *) malloc(0x400 * sizeof(char));
        for (int j = 0; j < 0x400; j++)
        {
            mem_chr_rom_banks[index][j] = chr_dump[i + j];
        }
        index++;
    }
    printf("ANES: CHR Dump copied success, %d bytes copied. \n",chr_size);
    printf("ANES: CHR ROM 1KB Count: %d \n",mem_chr_rom_01kb_count);
    printf("ANES: CHR ROM 2KB Count: %d  \n",mem_chr_rom_02kb_count);
    printf("ANES: CHR ROM 4KB Count: %d  \n",mem_chr_rom_04kb_count);
    printf("ANES: CHR ROM 8KB Count: %d  \n",mem_chr_rom_08kb_count);

    int chr_ram_size_KB = DEFAULT_CHR_RAM_1KB_COUNT;
    mem_chr_ram_01kb_count = chr_ram_size_KB / 1;
    mem_chr_ram_02kb_count = mem_chr_ram_01kb_count / 2;
    mem_chr_ram_04kb_count = mem_chr_ram_01kb_count / 4;
    mem_chr_ram_08kb_count = mem_chr_ram_01kb_count / 8;
    mem_chr_ram_01kb_mask= mem_chr_ram_01kb_count-1;
    mem_chr_ram_02kb_mask = mem_chr_ram_02kb_count - 1;
    mem_chr_ram_04kb_mask = mem_chr_ram_04kb_count - 1;
    mem_chr_ram_08kb_mask = mem_chr_ram_08kb_count - 1;

    mem_chr_ram_banks=(char **) malloc(mem_chr_ram_01kb_count* sizeof(char *));
    for (int i=0; i<mem_chr_ram_01kb_count; i++)
    {
        mem_chr_ram_banks[i] =(char *) malloc(0x400 * sizeof(char));
    }
    printf("ANES: CHR RAM 1KB Count: %d \n",mem_chr_ram_01kb_count);
    printf("ANES: CHR RAM 2KB Count: %d  \n",mem_chr_ram_02kb_count);
    printf("ANES: CHR RAM 4KB Count: %d  \n",mem_chr_ram_04kb_count);
    printf("ANES: CHR RAM 8KB Count: %d  \n",mem_chr_ram_08kb_count);
}
void MEM_SET_TRAINER_DUMP(char *trainer_dump, int trainer_size)
{
    // Copy trainer to first PRG RAM bank, assuming first bank is switched into 0x6000 area
    for (int j=0 ; j< trainer_size; j++)
    {
        mem_prg_ram_banks[0][j] = trainer_dump[j];
    }
}

void MEM_SET_MAPPER_PRG_EX(void(*writeAccess)(unsigned short *address, unsigned char *value),
                           void(*readAccess)(unsigned short *address,  unsigned char *value))
{
    mapper_on_prg_ex_write=writeAccess;
    mapper_on_prg_ex_read=readAccess;
}
void MEM_SET_MAPPER_PRG_PR(void(*writeAccess)(unsigned short *address, unsigned char *value),
                           void(*readAccess)(unsigned short *address,  unsigned char *value))
{
    mapper_on_prg_pr_write=writeAccess;
    mapper_on_prg_pr_read=readAccess;
}
void MEM_SET_MAPPER_CHR(void(*writeAccess)(unsigned short *address, unsigned char *value),
                        void(*readAccess)(unsigned short *address,  unsigned char *value))
{
    mapper_on_chr_write=writeAccess;
    mapper_on_chr_read=readAccess;
}
void MEM_SET_MAPPER_NMT(void(*writeAccess)(unsigned short *address, unsigned char *value),
                        void(*readAccess)(unsigned short *address,  unsigned char *value))
{
    mapper_on_nmt_write=writeAccess;
    mapper_on_nmt_read=readAccess;
}
void MEM_SET_MAPPER_PAL(void(*writeAccess)(unsigned short *address, unsigned char *value),
                        void(*readAccess)(unsigned short *address,  unsigned char *value))
{
    mapper_on_pal_write=writeAccess;
    mapper_on_pal_read=readAccess;
}

void MEM_CPUWrite(unsigned short *address, unsigned char *value)
{
    MEM_CPU_BUS_W=0;
    MEM_CPU_BUS_ADDRESS=(*address);
    NESClockComponents();

    if ((*address) < 0x2000)
    {
        // CPU WRAM
        mem_wram[(*address) & 0x7FF]=(*value);
    }
    else if ((*address) < 0x4000)
    {
        // PPU Registers
        PPU_CPUWrite(address,value);
    }
    else if ((*address) < 0x4020)
    {
        // APU Registers
        APUCPUWrite(address,value);
    }
    else
    {
        // PRG RAM-ROM
        if ((*address) < 0x8000)
            mapper_on_prg_ex_write(address, value);
        else
            mapper_on_prg_pr_write(address, value);
        // PRG RAM-ROM
        prg_block = ((*address) >> 12) & 0xF;
        if (mem_prg_block_ram[prg_block])
        {
            // It is a ram, get index in this block for ram
            prg_index_in_block = mem_prg_ram_block_index[prg_block] & mem_prg_ram_04kb_mask;
            // See if we can read from this memory block
            if (mem_prg_block_enabled[prg_block])
                if (mem_prg_block_writable[prg_block])
                    mem_prg_ram_banks[prg_index_in_block][(*address) & 0xFFF]=(*value);
        }
    }
}
void MEM_CPURead(unsigned short *address, unsigned char *value)
{
    MEM_CPU_BUS_W=1;
    MEM_CPU_BUS_ADDRESS=(*address);
    NESClockComponents();

    if ((*address) < 0x2000)
    {
        // CPU WRAM
        (*value) = mem_wram[(*address) & 0x7FF];
    }
    else if ((*address) < 0x4000)
    {
        // PPU Registers
        PPU_CPURead(address,value);
    }
    else if ((*address) < 0x4020)
    {
        // APU Registers
        APUCPURead(address,value);
    }
    else
    {


        // PRG RAM-ROM
        prg_block = ((*address) >> 12) & 0xF;
        if (mem_prg_block_ram[prg_block])
        {
            // It is a ram, get index in this block for ram
            prg_index_in_block = mem_prg_ram_block_index[prg_block] & mem_prg_ram_04kb_mask;
            // See if we can read from this memory block
            if (mem_prg_block_enabled[prg_block])
                (*value) = mem_prg_ram_banks[prg_index_in_block][(*address) & 0xFFF];
            else
                (*value) = 0;
        }
        else
        {
            prg_index_in_block = mem_prg_rom_block_index[prg_block] & mem_prg_rom_04kb_mask;
            (*value) = mem_prg_rom_banks[prg_index_in_block][(*address) & 0xFFF];
        }

        if ((*address)  < 0x8000)
            mapper_on_prg_ex_read(address, value);
        else
            mapper_on_prg_pr_read(address, value);
        //printf("MAPPER 000 PR READ: addr %d = %d \n",(*address),(*value));
    }
}
void MEM_CPUReadPRG(unsigned short *address, unsigned char *value)
{
    // PRG RAM-ROM
    prg_block = ((*address) >> 12) & 0xF;
    if (mem_prg_block_ram[prg_block])
    {
        // It is a ram, get index in this block for ram
        prg_index_in_block = mem_prg_ram_block_index[prg_block] & mem_prg_ram_04kb_mask;
        // See if we can read from this memory block
        if (mem_prg_block_enabled[prg_block])
            (*value) = mem_prg_ram_banks[prg_index_in_block][(*address) & 0xFFF];
        else
            (*value) = 0;
    }
    else
    {
        prg_index_in_block = mem_prg_rom_block_index[prg_block] & mem_prg_rom_04kb_mask;
        (*value) = mem_prg_rom_banks[prg_index_in_block][(*address) & 0xFFF];
    }
}

void MEM_CHRRead(unsigned short *address, unsigned char *value)
{

    // CHR
    // 00-07 means patterntables
    // 08-11 means nametables, should not included
    // 12-15 nametables mirrors, should not included as well
    chr_block = ((*address) >> 10) & 0x7;// 0x0000 - 0x1FFF, 0-7.

    if (mem_chr_block_ram[chr_block])
    {
        chr_index_in_block = mem_chr_ram_block_index[chr_block]&mem_chr_ram_01kb_mask;
        if (mem_chr_block_enabled[chr_block])
            (*value) = mem_chr_ram_banks[chr_index_in_block][(*address) & 0x3FF];
        else
            (*value) = 0;
    }
    else
    {
        chr_index_in_block = mem_chr_rom_block_index[chr_block]&mem_chr_rom_01kb_mask;
        (*value) = mem_chr_rom_banks[chr_index_in_block][(*address) & 0x3FF];
    }

    mapper_on_chr_read(address, value);
}
void MEM_CHRWrite(unsigned short *address, unsigned char *value)
{
    // CHR
    // 00-07 means patterntables
    // 08-11 means nametables, should not included
    // 12-15 nametables mirrors, should not included as well
    chr_block = ((*address) >> 10) & 0x7;// 0x0000 - 0x1FFF, 0-7.

    if (mem_chr_block_ram[chr_block])
    {
        chr_index_in_block = mem_chr_ram_block_index[chr_block]&mem_chr_ram_01kb_mask;
        if (mem_chr_block_enabled[chr_block])
            if (mem_chr_block_writable[chr_block])
                mem_chr_ram_banks[chr_index_in_block][(*address) & 0x3FF]= (*value);
    }
    mapper_on_chr_write(address, value);
}

void MEM_NMTRead(unsigned short *address, unsigned char *value)
{

    // NMT
    chr_block = ((*address) >> 10) & 0x3;// 0x2000 - 0x2C00, 0-3.
    chr_index_in_block = mem_nmt_block_index[chr_block] & 0x3;

    (*value) = mem_nmt_ram_banks[chr_index_in_block][(*address) & 0x3FF];

    mapper_on_nmt_read(address, value);
}
void MEM_NMTWrite(unsigned short *address, unsigned char *value)
{
    // NMT
    chr_block = ((*address) >> 10) & 0x3;// 0x2000 - 0x2C00, 0-3.
    chr_index_in_block = mem_nmt_block_index[chr_block] & 0x3;

    mem_nmt_ram_banks[chr_index_in_block][(*address) & 0x3FF]= (*value);

    mapper_on_nmt_write(address, value);
}

void MEM_PALRead(unsigned short *address, unsigned char *value)
{

    if (((*address) & 3) != 0)
        (*value)  = mem_palette_bank[(*address) & 0x1F];
    else
        (*value)  = mem_palette_bank[(*address) & 0x0C];

    mapper_on_pal_read(address,value);
}
void MEM_PALWrite(unsigned short *address, unsigned char *value)
{
    if (((*address) & 3) != 0)
        mem_palette_bank[(*address) & 0x1F] = (*value);
    else
        mem_palette_bank[(*address) & 0x0C] = (*value);

    mapper_on_pal_write(address, value);
}

void MEM_SWITCH_PRG_04KB_RAM(int prg_area, char is_ram)
{
    mem_prg_block_ram[prg_area]=is_ram;
}
void MEM_SWITCH_PRG_08KB_RAM(int prg_area, char is_ram)
{
    mem_prg_block_ram[prg_area]=is_ram;
    mem_prg_block_ram[prg_area+1]=is_ram;
}
void MEM_SWITCH_PRG_16KB_RAM(int prg_area, char is_ram)
{
    mem_prg_block_ram[prg_area]=is_ram;
    mem_prg_block_ram[prg_area+1]=is_ram;
    mem_prg_block_ram[prg_area+2]=is_ram;
    mem_prg_block_ram[prg_area+3]=is_ram;
}
void MEM_SWITCH_PRG_32KB_RAM(int prg_area, char is_ram)
{
    mem_prg_block_ram[prg_area]=is_ram;
    mem_prg_block_ram[prg_area+1]=is_ram;
    mem_prg_block_ram[prg_area+2]=is_ram;
    mem_prg_block_ram[prg_area+3]=is_ram;
    mem_prg_block_ram[prg_area+4]=is_ram;
    mem_prg_block_ram[prg_area+5]=is_ram;
    mem_prg_block_ram[prg_area+6]=is_ram;
    mem_prg_block_ram[prg_area+7]=is_ram;
}

void MEM_SWITCH_PRG_04KB(int prg_area, int index)
{
    if (mem_prg_block_ram[prg_area])
    {
        mem_prg_ram_block_index[prg_area]=index ;
    }
    else
    {
        mem_prg_rom_block_index[prg_area]=index;
    }
}
void MEM_SWITCH_PRG_08KB(int prg_area, int index)
{
    index*=2;
    if (mem_prg_block_ram[prg_area])
    {
        mem_prg_ram_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_ram_block_index[prg_area]=index;
    }
    else
    {
        mem_prg_rom_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_rom_block_index[prg_area]=index;
    }
}
void MEM_SWITCH_PRG_16KB(int prg_area, int index)
{
    index*=4;
    if (mem_prg_block_ram[prg_area])
    {
        mem_prg_ram_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_ram_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_ram_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_ram_block_index[prg_area]=index;
    }
    else
    {
        mem_prg_rom_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_rom_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_rom_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_rom_block_index[prg_area]=index;
    }
}
void MEM_SWITCH_PRG_32KB(int prg_area, int index)
{
    index*=8;
    if (mem_prg_block_ram[prg_area])
    {
        mem_prg_ram_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_ram_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_ram_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_ram_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_ram_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_ram_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_ram_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_ram_block_index[prg_area]=index;
    }
    else
    {
        mem_prg_rom_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_rom_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_rom_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_rom_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_rom_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_rom_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_rom_block_index[prg_area]=index;
        index++;
        prg_area++;
        mem_prg_rom_block_index[prg_area]=index;
    }
}

void MEM_TOGGLE_PRG_04KB_RAM_ENABLE(int prg_area, char is_enabled)
{
    mem_prg_block_enabled[prg_area]=is_enabled;
}
void MEM_TOGGLE_PRG_08KB_RAM_ENABLE(int prg_area, char is_enabled)
{
    mem_prg_block_enabled[prg_area]=is_enabled;
    mem_prg_block_enabled[prg_area+1]=is_enabled;
}
void MEM_TOGGLE_PRG_16KB_RAM_ENABLE(int prg_area, char is_enabled)
{
    mem_prg_block_enabled[prg_area]=is_enabled;
    mem_prg_block_enabled[prg_area+1]=is_enabled;
    mem_prg_block_enabled[prg_area+2]=is_enabled;
    mem_prg_block_enabled[prg_area+3]=is_enabled;
}
void MEM_TOGGLE_PRG_32KB_RAM_ENABLE(int prg_area, char is_enabled)
{
    mem_prg_block_enabled[prg_area]=is_enabled;
    mem_prg_block_enabled[prg_area+1]=is_enabled;
    mem_prg_block_enabled[prg_area+2]=is_enabled;
    mem_prg_block_enabled[prg_area+3]=is_enabled;
    mem_prg_block_enabled[prg_area+4]=is_enabled;
    mem_prg_block_enabled[prg_area+5]=is_enabled;
    mem_prg_block_enabled[prg_area+6]=is_enabled;
    mem_prg_block_enabled[prg_area+7]=is_enabled;
}

void MEM_TOGGLE_PRG_04KB_RAM_WRITABLE(int prg_area, char is_writable)
{
    mem_prg_block_writable[prg_area]=is_writable;
}
void MEM_TOGGLE_PRG_08KB_RAM_WRITABLE(int prg_area, char is_writable)
{
    mem_prg_block_writable[prg_area]=is_writable;
    mem_prg_block_writable[prg_area+1]=is_writable;
}
void MEM_TOGGLE_PRG_16KB_RAM_WRITABLE(int prg_area, char is_writable)
{
    mem_prg_block_writable[prg_area]=is_writable;
    mem_prg_block_writable[prg_area+1]=is_writable;
    mem_prg_block_writable[prg_area+2]=is_writable;
    mem_prg_block_writable[prg_area+3]=is_writable;
}
void MEM_TOGGLE_PRG_32KB_RAM_WRITABLE(int prg_area, char is_writable)
{
    mem_prg_block_writable[prg_area]=is_writable;
    mem_prg_block_writable[prg_area+1]=is_writable;
    mem_prg_block_writable[prg_area+2]=is_writable;
    mem_prg_block_writable[prg_area+3]=is_writable;
    mem_prg_block_writable[prg_area+4]=is_writable;
    mem_prg_block_writable[prg_area+5]=is_writable;
    mem_prg_block_writable[prg_area+6]=is_writable;
    mem_prg_block_writable[prg_area+7]=is_writable;
}

void MEM_TOGGLE_PRG_04KB_RAM_BATTERY(int prg_area, char is_battery)
{
    mem_prg_block_battery[prg_area]=is_battery;
}
void MEM_TOGGLE_PRG_08KB_RAM_BATTERY(int prg_area, char is_battery)
{
    mem_prg_block_battery[prg_area]=is_battery;
    mem_prg_block_battery[prg_area+1]=is_battery;
}
void MEM_TOGGLE_PRG_16KB_RAM_BATTERY(int prg_area, char is_battery)
{
    mem_prg_block_battery[prg_area]=is_battery;
    mem_prg_block_battery[prg_area+1]=is_battery;
    mem_prg_block_battery[prg_area+2]=is_battery;
    mem_prg_block_battery[prg_area+3]=is_battery;
}
void MEM_TOGGLE_PRG_32KB_RAM_BATTERY(int prg_area, char is_battery)
{
    mem_prg_block_battery[prg_area]=is_battery;
    mem_prg_block_battery[prg_area+1]=is_battery;
    mem_prg_block_battery[prg_area+2]=is_battery;
    mem_prg_block_battery[prg_area+3]=is_battery;
    mem_prg_block_battery[prg_area+4]=is_battery;
    mem_prg_block_battery[prg_area+5]=is_battery;
    mem_prg_block_battery[prg_area+6]=is_battery;
    mem_prg_block_battery[prg_area+7]=is_battery;
}

void MEM_SWITCH_CHR_01KB_RAM(int chr_area, char is_ram)
{
    mem_chr_block_ram[chr_area]=is_ram;
}
void MEM_SWITCH_CHR_02KB_RAM(int chr_area, char is_ram)
{
    mem_chr_block_ram[chr_area]=is_ram;
    mem_chr_block_ram[chr_area+1]=is_ram;
}
void MEM_SWITCH_CHR_04KB_RAM(int chr_area, char is_ram)
{
    mem_chr_block_ram[chr_area]=is_ram;
    mem_chr_block_ram[chr_area+1]=is_ram;
    mem_chr_block_ram[chr_area+2]=is_ram;
    mem_chr_block_ram[chr_area+3]=is_ram;
}
void MEM_SWITCH_CHR_08KB_RAM(int chr_area, char is_ram)
{
    mem_chr_block_ram[chr_area]=is_ram;
    mem_chr_block_ram[chr_area+1]=is_ram;
    mem_chr_block_ram[chr_area+2]=is_ram;
    mem_chr_block_ram[chr_area+3]=is_ram;
    mem_chr_block_ram[chr_area+4]=is_ram;
    mem_chr_block_ram[chr_area+5]=is_ram;
    mem_chr_block_ram[chr_area+6]=is_ram;
    mem_chr_block_ram[chr_area+7]=is_ram;
}

void MEM_SWITCH_CHR_01KB(int chr_area, int index)
{
    if (mem_chr_block_ram[chr_area])
    {
        mem_chr_ram_block_index[chr_area]=index ;
    }
    else
    {
        mem_chr_rom_block_index[chr_area]=index;
    }
}
void MEM_SWITCH_CHR_02KB(int chr_area, int index)
{
    index*=2;
    if (mem_chr_block_ram[chr_area])
    {
        mem_chr_ram_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_ram_block_index[chr_area]=index;
    }
    else
    {
        mem_chr_rom_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_rom_block_index[chr_area]=index;
    }
}
void MEM_SWITCH_CHR_04KB(int chr_area, int index)
{
    index*=4;
    if (mem_chr_block_ram[chr_area])
    {


        mem_chr_ram_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_ram_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_ram_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_ram_block_index[chr_area]=index;


    }
    else
    {
        mem_chr_rom_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_rom_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_rom_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_rom_block_index[chr_area]=index;
    }
}
void MEM_SWITCH_CHR_08KB(int chr_area, int index)
{
    index*=8;
    if (mem_chr_block_ram[chr_area])
    {
        mem_chr_ram_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_ram_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_ram_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_ram_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_ram_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_ram_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_ram_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_ram_block_index[chr_area]=index;
    }
    else
    {
        mem_chr_rom_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_rom_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_rom_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_rom_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_rom_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_rom_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_rom_block_index[chr_area]=index;
        index++;
        chr_area++;
        mem_chr_rom_block_index[chr_area]=index;
    }
}

void MEM_TOGGLE_CHR_01KB_RAM_ENABLE(int chr_area, char is_enabled)
{
    mem_chr_block_enabled[chr_area]=is_enabled;
}
void MEM_TOGGLE_CHR_02KB_RAM_ENABLE(int chr_area, char is_enabled)
{
    mem_chr_block_enabled[chr_area]=is_enabled;
    mem_chr_block_enabled[chr_area+1]=is_enabled;
}
void MEM_TOGGLE_CHR_04KB_RAM_ENABLE(int chr_area, char is_enabled)
{
    mem_chr_block_enabled[chr_area]=is_enabled;
    mem_chr_block_enabled[chr_area+1]=is_enabled;
    mem_chr_block_enabled[chr_area+2]=is_enabled;
    mem_chr_block_enabled[chr_area+3]=is_enabled;
}
void MEM_TOGGLE_CHR_08KB_RAM_ENABLE(int chr_area, char is_enabled)
{
    mem_chr_block_enabled[chr_area]=is_enabled;
    mem_chr_block_enabled[chr_area+1]=is_enabled;
    mem_chr_block_enabled[chr_area+2]=is_enabled;
    mem_chr_block_enabled[chr_area+3]=is_enabled;
    mem_chr_block_enabled[chr_area+4]=is_enabled;
    mem_chr_block_enabled[chr_area+5]=is_enabled;
    mem_chr_block_enabled[chr_area+6]=is_enabled;
    mem_chr_block_enabled[chr_area+7]=is_enabled;
}

void MEM_TOGGLE_CHR_01KB_RAM_WRITABLE(int chr_area, char is_writable)
{
    mem_chr_block_writable[chr_area]=is_writable;
}
void MEM_TOGGLE_CHR_02KB_RAM_WRITABLE(int chr_area, char is_writable)
{
    mem_chr_block_writable[chr_area]=is_writable;
    mem_chr_block_writable[chr_area+1]=is_writable;
}
void MEM_TOGGLE_CHR_04KB_RAM_WRITABLE(int chr_area, char is_writable)
{
    mem_chr_block_writable[chr_area]=is_writable;
    mem_chr_block_writable[chr_area+1]=is_writable;
    mem_chr_block_writable[chr_area+2]=is_writable;
    mem_chr_block_writable[chr_area+3]=is_writable;
}
void MEM_TOGGLE_CHR_08KB_RAM_WRITABLE(int chr_area, char is_writable)
{
    mem_chr_block_writable[chr_area]=is_writable;
    mem_chr_block_writable[chr_area+1]=is_writable;
    mem_chr_block_writable[chr_area+2]=is_writable;
    mem_chr_block_writable[chr_area+3]=is_writable;
    mem_chr_block_writable[chr_area+4]=is_writable;
    mem_chr_block_writable[chr_area+5]=is_writable;
    mem_chr_block_writable[chr_area+6]=is_writable;
    mem_chr_block_writable[chr_area+7]=is_writable;
}

void MEM_TOGGLE_CHR_01KB_RAM_BATTERY(int chr_area, char is_battery)
{
    mem_chr_block_battery[chr_area]=is_battery;
}
void MEM_TOGGLE_CHR_02KB_RAM_BATTERY(int chr_area, char is_battery)
{
    mem_chr_block_battery[chr_area]=is_battery;
    mem_chr_block_battery[chr_area+1]=is_battery;
}
void MEM_TOGGLE_CHR_04KB_RAM_BATTERY(int chr_area, char is_battery)
{
    mem_chr_block_battery[chr_area]=is_battery;
    mem_chr_block_battery[chr_area+1]=is_battery;
    mem_chr_block_battery[chr_area+2]=is_battery;
    mem_chr_block_battery[chr_area+3]=is_battery;
}
void MEM_TOGGLE_CHR_08KB_RAM_BATTERY(int chr_area, char is_battery)
{
    mem_chr_block_battery[chr_area]=is_battery;
    mem_chr_block_battery[chr_area+1]=is_battery;
    mem_chr_block_battery[chr_area+2]=is_battery;
    mem_chr_block_battery[chr_area+3]=is_battery;
    mem_chr_block_battery[chr_area+4]=is_battery;
    mem_chr_block_battery[chr_area+5]=is_battery;
    mem_chr_block_battery[chr_area+6]=is_battery;
    mem_chr_block_battery[chr_area+7]=is_battery;
}
void MEM_SWITCH_NMT_01KB(char nmt_area, char index)
{
    mem_nmt_block_index[nmt_area]=index;
}
void MEM_SWITCH_NMT_FROM_MIRORING(unsigned char mirroring)
{
    mem_nmt_block_index[0] = mirroring & 0x3;
    mem_nmt_block_index[1] = (mirroring >> 2) & 0x3;
    mem_nmt_block_index[2] = (mirroring >> 4) & 0x3;
    mem_nmt_block_index[3] = (mirroring >> 6) & 0x3;
}
void MEM_SWITCH_NMT_FROM_MIRORING_REVERSED(unsigned char mirroring)
{
    mem_nmt_block_index[3] = mirroring & 0x3;
    mem_nmt_block_index[2] = (mirroring >> 2) & 0x3;
    mem_nmt_block_index[1] = (mirroring >> 4) & 0x3;
    mem_nmt_block_index[0] = (mirroring >> 6) & 0x3;
}
