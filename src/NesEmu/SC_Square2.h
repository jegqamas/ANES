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

int sq2_output;

void SC_Square2HardReset();
void SC_Square2SoftReset();

void SC_Square2Clock();
void SC_Square2ClockLength();
void SC_Square2ClockEnvelope();

void APUOnRegister4004();
void APUOnRegister4005();
void APUOnRegister4006();
void APUOnRegister4007();

void SC_Square2OnRegister4015();
char SC_Square2ReadRegister4015();
