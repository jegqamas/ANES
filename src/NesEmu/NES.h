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
char NES_ON;
char NES_PAUSED;

char NES_IsGameLoaded;// indicates if nes emu is loaded with a game and ready to run
char* NES_GameLoadedName;// indicates if nes emu is loaded with a game and ready to run
int NES_Mirroring;// Default mirroring

int NES_IRQFlags;
int NES_TargetFPS;
int NES_TVFormat;
int NES_AudioFrequency;

// Calling this assumes that game is loaded and ready to go
void NESInitialize(int game_source_mode, int audio_freq);
void NESSetupMapperPointers(void (*board_on_cpu_clock)(),void (*board_hard_reset)(),void (*board_soft_reset)());
void NESHardReset();
void NESSoftReset();
void NESClock();
void NESClockFrame();
void (*NESClockComponents)();

void NESClockComponentsNTSC();
void NESClockComponentsPALB();
void ClockComponentsBlank();
void NESCheckInterrupts();
