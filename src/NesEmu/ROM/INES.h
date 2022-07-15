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
// Load rom file header, this will set INES_FileName and INES_IsValid
// fileName: the complete rom file path
void INES_LoadFile(char *file_name);

// Close and free resources
void INES_Close();

char *prg_dump;
char *chr_dump;
char *trn_dump;

char *INES_FileName;// Loaded file path
char INES_IsValid;// Indicates if this rom is loaded and valid
char INES_HasBattery;// Indicates if this rom is battery-packed
char INES_IsPlaychoice10;// Indicates if this rom is battery-packed
char INES_IsVSUnisystem;// Indicates if this rom is VS-Unisystem
char INES_HasTrainer;// Indicates if this rom has trainer

int INES_PRGCount;// PRG pages count
int INES_PRGSizeInBytes;// PRG bank size in bytes
int INES_CHRCount;// CHR pages count
int INES_CHRSizeInBytes;// CHR bank size in bytes
int INES_TrainerSizeInBytes; // The trainer dump size in bytes.
int INES_MapperNumber;// INES mapper number
int INES_Mirroring;// Default mirroring
