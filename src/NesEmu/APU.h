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

// DATA REG
unsigned char apu_reg_io_db;// The data bus
unsigned char apu_reg_io_addr;// The address bus
char apu_reg_access_happened;// Triggers when cpu accesses apu bus.
char apu_reg_access_w;// True= write access, False= Read access.
char apu_odd_cycle;
int audio_samples_added;
int audio_w_pos;
int audio_r_pos;
double audio_timer;
unsigned char** audio_samples;

unsigned char PLAYER1_DATA;
unsigned char PLAYER2_DATA;
unsigned char PLAYER3_DATA;
unsigned char PLAYER4_DATA;
char IsFourPlayersConnected;
char IsExternalSoundChannelsEnabled;
char IsStereoNes;
char UseAPULookUpTable;

int apu_channel_pan_sq1;
int apu_channel_pan_sq2;
int apu_channel_pan_nos;
int apu_channel_pan_trl;
int apu_channel_pan_dmc;

void APUInitialize();
void APUSetupMapper(void (*apu_get_sample)(double *output_sample),void (*on_board_clock_length)(),void (*on_board_clock_envelope)(),void (*on_board_clock)(),void (*on_board_clock_single)());
void APUHardReset(int tv_format, int audio_frequency);
void APUSoftReset();

void APUClock();

void APUCPURead(unsigned short *address, unsigned char *value);
void APUCPUWrite(unsigned short *address, unsigned char *value);

void APUBlankAccess();

void APUOnRegister4015();
void APUOnRegister4016();
void APUOnRegister4017();

void APURead4015();
void APURead4016();
void APURead4017();

void APUClockDuration();
void APUClockEnvelope();
void APUCheckIRQ();
void APUUpdatePlaybackNormal();
void APUUpdatePlaybackStereoNes();
void APUApplyChannelPans();
