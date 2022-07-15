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

int SDLAudioOut_BufferSizeInKB;
int SDLAudioOut_AudioDeviceIndex;
int SDLAudioOut_AudioFrequency;
int SDLAudioOut_AudioChannels;
char SDLAudioOut_IsStereoNes;
char SDLAudioOut_IsPlaying;
char SDLAudioOut_Initialized;
unsigned char SDLAudioOut_AudioVolume;

void SDLAudioOut_Initialize();
void SDLAudioOut_Reset();
void SDLAudioOut_SetVolume(unsigned char vol_prec);
void SDLAudioOut_Shutdown();
void SDLAudioOut_SignalToggle(char started);
void SDLAudioOut_SubmitSamples(unsigned char** samples, int samples_a);
void SDLAudioOut_Play();
void SDLAudioOut_Pause();
