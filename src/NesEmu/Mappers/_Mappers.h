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
/*MAPPER 000*****************************************/
void Mapper000HardReset();
void Mapper000SoftReset();

// These methods should be linked using pointers in nes components
// EX is between $4020 - $7FFF
void Mapper000_WriteEX(unsigned short *address, unsigned char *value);
void Mapper000_ReadEX(unsigned short *address, unsigned char *value);
// PR is between $8000 - $FFFF
void Mapper000_WritePR(unsigned short *address, unsigned char *value);
void Mapper000_ReadPR(unsigned short *address, unsigned char *value);
// CHR $0000 - $1FFF
void Mapper000_WriteCHR(unsigned short *address, unsigned char *value);
void Mapper000_ReadCHR(unsigned short *address, unsigned char *value);
// NMT $2000 - $3EFF
void Mapper000_WriteNMT(unsigned short *address, unsigned char *value);
void Mapper000_ReadNMT(unsigned short *address, unsigned char *value);
// PAL $3F00 - $3FFF
void Mapper000_WritePAL(unsigned short *address, unsigned char *value);
void Mapper000_ReadPAL(unsigned short *address, unsigned char *value);
// Clocks
void Mapper000_OnCPUClock();
void Mapper000_OnPPUClock();
void Mapper000_OnPPUScanlineClock();
void Mapper000_OnPPUAddressUpdate(unsigned short *address);

void Mapper000_OnAPUGetAudioSample(int *board_sample);
void Mapper000_OnAPUClock();
void Mapper000_OnAPUClockSingle();
void Mapper000_OnAPUClockLength();
void Mapper000_OnAPUClockEnvelope();
/*****************************************************/

/*MAPPER 001*****************************************/
char Mapper001_MMCB_Mode;
char Mapper001_MMCA_Mode;
char Mapper001_UseSRAMSwitch;
void Mapper001HardReset();
void Mapper001_OnCPUClock();
void Mapper001_WritePR(unsigned short *address, unsigned char *value);
/*****************************************************/

/*MAPPER 002*****************************************/
void Mapper002HardReset();
void Mapper002_WritePR(unsigned short *address, unsigned char *value);
/*****************************************************/

/*MAPPER 003*****************************************/
void Mapper003_WritePR(unsigned short *address, unsigned char *value);
/*****************************************************/

/*MAPPER 004*****************************************/
char Mapper004_MMC3A_Mode;
void Mapper004HardReset();
void Mapper004_WritePR(unsigned short *address, unsigned char *value);
void Mapper004_OnPPUClock();
void Mapper004_OnPPUAddressUpdate(unsigned short *address);
/*****************************************************/

/*MAPPER 005*****************************************/
char Mapper005_UseSRAMMirroring;
void Mapper005HardReset();

void Mapper005_WriteEX(unsigned short *address, unsigned char *value);
void Mapper005_ReadEX(unsigned short *address, unsigned char *value);
void Mapper005_ReadCHR(unsigned short *address, unsigned char *value);
void Mapper005_WriteNMT(unsigned short *address, unsigned char *value);
void Mapper005_ReadNMT(unsigned short *address, unsigned char *value);

void Mapper005_OnPPUScanlineClock();
void Mapper005_OnAPUGetAudioSample(double *board_sample);
void Mapper005_OnAPUClock();
void Mapper005_OnAPUClockLength();
void Mapper005_OnAPUClockEnvelope();
/*****************************************************/

/*MAPPER 007*****************************************/
char Mapper007_BusConflictEnable;
void Mapper007_WritePR(unsigned short *address, unsigned char *value);
/*****************************************************/

/*MAPPER 009*****************************************/
void Mapper009HardReset();
void Mapper009_WritePR(unsigned short *address, unsigned char *value);
void Mapper009_ReadCHR(unsigned short *address, unsigned char *value);
/*****************************************************/

/*MAPPER 010*****************************************/
void Mapper010HardReset();
void Mapper010_WritePR(unsigned short *address, unsigned char *value);
void Mapper010_ReadCHR(unsigned short *address, unsigned char *value);
/*****************************************************/

/*MAPPER 011*****************************************/
void Mapper011_WritePR(unsigned short *address, unsigned char *value);
/*****************************************************/

/*MAPPER 013*****************************************/
void Mapper013HardReset();
void Mapper013_WritePR(unsigned short *address, unsigned char *value);
/*****************************************************/

/*MAPPER 015*****************************************/
void Mapper015_WritePR(unsigned short *address, unsigned char *value);
/*****************************************************/

/*MAPPER 018*****************************************/
void Mapper018HardReset();
void Mapper018_OnCPUClock();
void Mapper018_WritePR(unsigned short *address, unsigned char *value);
/*****************************************************/

/*MAPPER 021*****************************************/
void Mapper021HardReset();
void Mapper021_OnCPUClock();
void Mapper021_WritePR(unsigned short *address, unsigned char *value);
/*****************************************************/

/*MAPPER 022*****************************************/
void Mapper022HardReset();
void Mapper022_WritePR(unsigned short *address, unsigned char *value);
/*****************************************************/

/*MAPPER 023*****************************************/
void Mapper023HardReset();
void Mapper023_WriteEX(unsigned short *address, unsigned char *value);
void Mapper023_ReadEX(unsigned short *address, unsigned char *value);
void Mapper023_WritePR(unsigned short *address, unsigned char *value);
/*****************************************************/

/*MAPPER 024*****************************************/
void Mapper024HardReset();
void Mapper024_OnCPUClock();
void Mapper024_WritePR(unsigned short *address, unsigned char *value);
void Mapper024_OnAPUGetAudioSample(double *board_sample);
void Mapper024_OnAPUClockSingle();
/*****************************************************/

/*MAPPER 025*****************************************/
void Mapper025HardReset();
void Mapper025_OnCPUClock();
void Mapper025_WritePR(unsigned short *address, unsigned char *value);
/*****************************************************/

/*MAPPER 026*****************************************/
void Mapper026HardReset();
void Mapper026_OnCPUClock();
void Mapper026_WritePR(unsigned short *address, unsigned char *value);
void Mapper026_OnAPUGetAudioSample(double *board_sample);
void Mapper026_OnAPUClockSingle();
/*****************************************************/

/*MAPPER 071*****************************************/
char Mapper071_FirehawkMode;
void Mapper071HardReset();
void Mapper071_WritePR(unsigned short *address, unsigned char *value);
/*****************************************************/

/*MAPPER 087*****************************************/
void Mapper087_WriteEX(unsigned short *address, unsigned char *value);
/*****************************************************/
