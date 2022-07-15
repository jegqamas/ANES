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

int mmc5_pulse2_output;

void SC_MMC5_Pulse2HardReset();
void SC_MMC5_Pulse2SoftReset();

void SC_MMC5_Pulse2Clock();
void SC_MMC5_Pulse2ClockLength();
void SC_MMC5_Pulse2ClockEnvelope();

void SC_MMC5_Pulse2Write5004(unsigned char *value);
void SC_MMC5_Pulse2Write5006(unsigned char *value);
void SC_MMC5_Pulse2Write5007(unsigned char *value);

void SC_MMC5_Pulse2SetEnable(char enable);
char SC_MMC5_Pulse2ReadEnable();
