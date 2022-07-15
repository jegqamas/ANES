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

int DEFAULT_PRG_RAM_1KB_COUNT;
int DEFAULT_CHR_RAM_1KB_COUNT;

char MEM_CPU_BUS_W;
unsigned short MEM_CPU_BUS_ADDRESS;

unsigned char mem_wram[0x800];
unsigned char mem_palette_bank[32];
// PRG
char **mem_prg_rom_banks;// The prg ROM blocks, 4KB (0x1000) each.
char **mem_prg_ram_banks;// The prg RAM blocks, 4KB (0x1000) each.
// CHR
char **mem_chr_rom_banks;// The chr ROM blocks, 1KB (0x400) each.
char **mem_chr_ram_banks;// The chr RAM blocks, 1KB (0x400) each.
// NMT
unsigned char mem_nmt_ram_banks[4][0x400];// The nmt RAM blocks, 1KB (0x400) each. It is always 4 KB.

// Blocks configurations
unsigned int mem_prg_ram_block_index[16];// prg ram bank index
unsigned int mem_prg_rom_block_index[16];// prg rom bank index
char mem_prg_block_ram[16];// 1 this block is ram, 0 this block is rom
char mem_prg_block_enabled[16];// only for ram banks: 1 this block enabled, 0 this block is disabled (no access)
char mem_prg_block_writable[16];// only for ram banks: 1 this block is writable, 0 this block is read-only (no access on write)
char mem_prg_block_battery[16];// only for ram banks: 1 this block is battery (should be saved in file), 0 this block is not battery (will not be saved)

unsigned int mem_chr_ram_block_index[8];// chr ram bank index
unsigned int mem_chr_rom_block_index[8];// chr rom bank index
char mem_chr_block_ram[8];// 1 this block is ram, 0 this block is rom
char mem_chr_block_enabled[8];// only for ram banks: 1 this block enabled, 0 this block is disabled (no access)
char mem_chr_block_writable[8];// only for ram banks: 1 this block is writable, 0 this block is read-only (no access on write)
char mem_chr_block_battery[8];// only for ram banks: 1 this block is battery (should be saved in file), 0 this block is not battery (will not be saved)

unsigned int mem_nmt_block_index[4];

// Numbers and masks
unsigned int mem_prg_rom_04kb_count;
unsigned int mem_prg_rom_08kb_count;
unsigned int mem_prg_rom_16kb_count;
unsigned int mem_prg_rom_32kb_count;
unsigned int mem_prg_rom_04kb_mask;
unsigned int mem_prg_rom_08kb_mask;
unsigned int mem_prg_rom_16kb_mask;
unsigned int mem_prg_rom_32kb_mask;

unsigned int mem_prg_ram_04kb_count;
unsigned int mem_prg_ram_08kb_count;
unsigned int mem_prg_ram_16kb_count;
unsigned int mem_prg_ram_32kb_count;
unsigned int mem_prg_ram_04kb_mask;
unsigned int mem_prg_ram_08kb_mask;
unsigned int mem_prg_ram_16kb_mask;
unsigned int mem_prg_ram_32kb_mask;

unsigned int mem_chr_rom_01kb_count;
unsigned int mem_chr_rom_02kb_count;
unsigned int mem_chr_rom_04kb_count;
unsigned int mem_chr_rom_08kb_count;
unsigned int mem_chr_rom_01kb_mask;
unsigned int mem_chr_rom_02kb_mask;
unsigned int mem_chr_rom_04kb_mask;
unsigned int mem_chr_rom_08kb_mask;

unsigned int mem_chr_ram_01kb_count;
unsigned int mem_chr_ram_02kb_count;
unsigned int mem_chr_ram_04kb_count;
unsigned int mem_chr_ram_08kb_count;
unsigned int mem_chr_ram_01kb_mask;
unsigned int mem_chr_ram_02kb_mask;
unsigned int mem_chr_ram_04kb_mask;
unsigned int mem_chr_ram_08kb_mask;

void MEM_HardReset();

void MEM_SET_PRG_DUMP(char *prg_dump, int prg_size);
void MEM_SET_CHR_DUMP(char *chr_dump, int chr_size);
void MEM_SET_TRAINER_DUMP(char *trainer_dump, int trainer_size);

void MEM_SET_MAPPER_PRG_EX(void(*writeAccess)(unsigned short *address, unsigned char *value),
                           void(*readAccess)(unsigned short *address,  unsigned char *value));
void MEM_SET_MAPPER_PRG_PR(void(*writeAccess)(unsigned short *address, unsigned char *value),
                           void(*readAccess)(unsigned short *address,  unsigned char *value));
void MEM_SET_MAPPER_CHR(void(*writeAccess)(unsigned short *address, unsigned char *value),
                        void(*readAccess)(unsigned short *address,  unsigned char *value));
void MEM_SET_MAPPER_NMT(void(*writeAccess)(unsigned short *address, unsigned char *value),
                        void(*readAccess)(unsigned short *address,  unsigned char *value));
void MEM_SET_MAPPER_PAL(void(*writeAccess)(unsigned short *address, unsigned char *value),
                        void(*readAccess)(unsigned short *address,  unsigned char *value));

void MEM_CPUWrite(unsigned short *address, unsigned char *value);
void MEM_CPURead(unsigned short *address, unsigned char *value);
// Read only prg without components clock. Used for bus conflicts in some mappers.
void MEM_CPUReadPRG(unsigned short *address, unsigned char *value);

void MEM_CHRRead(unsigned short *address, unsigned char *value);
void MEM_CHRWrite(unsigned short *address, unsigned char *value);

void MEM_NMTRead(unsigned short *address, unsigned char *value);
void MEM_NMTWrite(unsigned short *address, unsigned char *value);

void MEM_PALRead(unsigned short *address, unsigned char *value);
void MEM_PALWrite(unsigned short *address, unsigned char *value);

void MEM_SWITCH_PRG_04KB_RAM(int prg_area, char is_ram);
void MEM_SWITCH_PRG_08KB_RAM(int prg_area, char is_ram);
void MEM_SWITCH_PRG_16KB_RAM(int prg_area, char is_ram);
void MEM_SWITCH_PRG_32KB_RAM(int prg_area, char is_ram);

void MEM_SWITCH_PRG_04KB(int prg_area, int index);
void MEM_SWITCH_PRG_08KB(int prg_area, int index);
void MEM_SWITCH_PRG_16KB(int prg_area, int index);
void MEM_SWITCH_PRG_32KB(int prg_area, int index);

void MEM_TOGGLE_PRG_04KB_RAM_ENABLE(int prg_area, char is_enabled);
void MEM_TOGGLE_PRG_08KB_RAM_ENABLE(int prg_area, char is_enabled);
void MEM_TOGGLE_PRG_16KB_RAM_ENABLE(int prg_area, char is_enabled);
void MEM_TOGGLE_PRG_32KB_RAM_ENABLE(int prg_area, char is_enabled);

void MEM_TOGGLE_PRG_04KB_RAM_WRITABLE(int prg_area, char is_writable);
void MEM_TOGGLE_PRG_08KB_RAM_WRITABLE(int prg_area, char is_writable);
void MEM_TOGGLE_PRG_16KB_RAM_WRITABLE(int prg_area, char is_writable);
void MEM_TOGGLE_PRG_32KB_RAM_WRITABLE(int prg_area, char is_writable);

void MEM_TOGGLE_PRG_04KB_RAM_BATTERY(int prg_area, char is_battery);
void MEM_TOGGLE_PRG_08KB_RAM_BATTERY(int prg_area, char is_battery);
void MEM_TOGGLE_PRG_16KB_RAM_BATTERY(int prg_area, char is_battery);
void MEM_TOGGLE_PRG_32KB_RAM_BATTERY(int prg_area, char is_battery);

void MEM_SWITCH_CHR_01KB_RAM(int chr_area, char is_ram);
void MEM_SWITCH_CHR_02KB_RAM(int chr_area, char is_ram);
void MEM_SWITCH_CHR_04KB_RAM(int chr_area, char is_ram);
void MEM_SWITCH_CHR_08KB_RAM(int chr_area, char is_ram);

void MEM_SWITCH_CHR_01KB(int chr_area, int index);
void MEM_SWITCH_CHR_02KB(int chr_area, int index);
void MEM_SWITCH_CHR_04KB(int chr_area, int index);
void MEM_SWITCH_CHR_08KB(int chr_area, int index);

void MEM_TOGGLE_CHR_01KB_RAM_ENABLE(int chr_area, char is_enabled);
void MEM_TOGGLE_CHR_02KB_RAM_ENABLE(int chr_area, char is_enabled);
void MEM_TOGGLE_CHR_04KB_RAM_ENABLE(int chr_area, char is_enabled);
void MEM_TOGGLE_CHR_08KB_RAM_ENABLE(int chr_area, char is_enabled);

void MEM_TOGGLE_CHR_01KB_RAM_WRITABLE(int chr_area, char is_writable);
void MEM_TOGGLE_CHR_02KB_RAM_WRITABLE(int chr_area, char is_writable);
void MEM_TOGGLE_CHR_04KB_RAM_WRITABLE(int chr_area, char is_writable);
void MEM_TOGGLE_CHR_08KB_RAM_WRITABLE(int chr_area, char is_writable);

void MEM_TOGGLE_CHR_01KB_RAM_BATTERY(int chr_area, char is_battery);
void MEM_TOGGLE_CHR_02KB_RAM_BATTERY(int chr_area, char is_battery);
void MEM_TOGGLE_CHR_04KB_RAM_BATTERY(int chr_area, char is_battery);
void MEM_TOGGLE_CHR_08KB_RAM_BATTERY(int chr_area, char is_battery);

void MEM_SWITCH_NMT_01KB(char nmt_area, char index);
void MEM_SWITCH_NMT_FROM_MIRORING(unsigned char mirroring);
void MEM_SWITCH_NMT_FROM_MIRORING_REVERSED(unsigned char mirroring);
