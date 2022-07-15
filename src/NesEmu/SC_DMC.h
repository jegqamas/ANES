// This file is part of ANES (Agile Nes)
//
// A Nintendo Entertainment System / Family Computer (Nes/Famicom)
// Emulator written in C.
//
// Copyright � Alaa Ibrahim Hadid 2021 - 2022
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

int dmc_output;
char apu_irq_delta_occur;

void DMCHardReset(int tv_format);
void DMCSoftReset();

void DMCClock();
void DMCDoDMA();
void APUOnRegister4010();
void APUOnRegister4011();
void APUOnRegister4012();
void APUOnRegister4013();
void DMCOn4015();
void DMCRead4015();
void DMCWrite4015(unsigned short *address, unsigned char *value);
