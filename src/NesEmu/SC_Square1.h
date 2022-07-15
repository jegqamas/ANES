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

int sq1_output;

void SC_Square1HardReset();
void SC_Square1SoftReset();

void SC_Square1Clock();
void SC_Square1ClockLength();
void SC_Square1ClockEnvelope();

void APUOnRegister4000();
void APUOnRegister4001();
void APUOnRegister4002();
void APUOnRegister4003();

void SC_Square1OnRegister4015();
char SC_Square1ReadRegister4015();
