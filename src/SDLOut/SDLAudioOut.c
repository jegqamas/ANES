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
#include "SDLAudioOut.h"
#include "SDLVideoOut.h"
#include "SDL2/SDL.h"
#include "../NesEmu/NES.h"

int samples_count;
int buffer_min;
int buffer_limit;
unsigned char block_align;
SDL_AudioSpec specs;
SDL_AudioSpec specs1;
unsigned char* sdl_audio_samples;
unsigned int audio_device_index;
int r_pos;
int w_pos;
char ready;
double volume;
char is_rendering;
char stereo_mode;
unsigned char sample_r;
unsigned char sample_l;
int sdl_samples_added;
int sdl_buffer_size;
int fps_nes_normal;
int fps_nes_missle;
int fps_pl_faster;
int fps_mode;

void AudioCallback(void *userdata, Uint8* stream,  int len);
void FixSpeed()
{
    fps_mode = 0;

    fps_nes_missle = NES_TargetFPS +29;
    fps_pl_faster = NES_TargetFPS - 3;
    fps_nes_normal =NES_TargetFPS;
}
void SDLAudioOut_Initialize()
{
    printf("ANES SDL RENDERERS: Initializing audio ... \n");
    if (SDLAudioOut_BufferSizeInKB==0)
        SDLAudioOut_BufferSizeInKB=4;
    if (SDLAudioOut_AudioFrequency==0)
        SDLAudioOut_AudioFrequency=44100;
    if (SDLAudioOut_AudioChannels==0|| SDLAudioOut_AudioChannels > 2)
        SDLAudioOut_AudioChannels=1;

    stereo_mode = SDLAudioOut_AudioChannels == 2;
    if (stereo_mode)
        printf("ANES SDL RENDERERS: Audio Channels set to 2 (Stereo) \n");
    else
        printf("ANES SDL RENDERERS: Audio Channels set to 1 (Mono) \n");

    printf("ANES SDL RENDERERS: Audio buffer size in KB: %d ... \n",SDLAudioOut_BufferSizeInKB);
    sdl_buffer_size = 4 * 1024;
    buffer_min = 512;
    buffer_limit = sdl_buffer_size + 512;

   // samples_count = (SDLAudioOut_BufferSizeInKB * 4 * 1024) + (buffer_min*2);

 samples_count = SDLAudioOut_BufferSizeInKB  * 1024;

    sdl_audio_samples= (unsigned char*)malloc(samples_count * sizeof(unsigned char));

    for (int i = 0; i < samples_count; i++)
    {
        unsigned char val = (rand() % 20);

        sdl_audio_samples[i] = val;
    }

    printf("ANES SDL RENDERERS: Audio buffer size in bytes: %d ... \n",sdl_buffer_size);
    SDL_Init(SDL_INIT_AUDIO);

    // List audio drivers
    int c = SDL_GetNumAudioDrivers();
    for (int i = 0; i < c; i++)
    {
        printf("ANES SDL RENDERERS: ->Audio driver found: %s \n", SDL_GetAudioDeviceName(i, 0));
    }
    if (SDLAudioOut_AudioDeviceIndex<0)
        SDLAudioOut_AudioDeviceIndex=0;


    printf("ANES SDL RENDERERS: Opening audio device ... \n");
    printf("ANES SDL RENDERERS: ->Audio device to open: %s \n",SDL_GetAudioDeviceName(SDLAudioOut_AudioDeviceIndex, 0));

    specs.channels = SDLAudioOut_AudioChannels;
    specs.format = AUDIO_U8;
    specs.freq = SDLAudioOut_AudioFrequency;// TODO: setup audio frequency
    specs.samples = sdl_buffer_size;
    specs.callback = AudioCallback;
    specs.userdata=NULL;

    block_align=SDLAudioOut_AudioChannels; // channels * bits_per_sample/8, bits_per_sample=8 so just channels.

    audio_device_index = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(SDLAudioOut_AudioDeviceIndex, 0), 0, &specs, &specs1, NULL);
    if (audio_device_index == 0)
    {
        printf("ANES SDL RENDERERS: ERROR while opening audio device: %s \n",SDL_GetError());
    }
    else
    {
        printf("ANES SDL RENDERERS: Audio device opened successfully.\n");
    }
    w_pos = 0;
    r_pos = 0;

    SDLAudioOut_IsPlaying=0;

    SDL_PauseAudioDevice(audio_device_index, 1);

    SDLAudioOut_Initialized=1;

    printf("ANES SDL RENDERERS: SDL Audio initialized successfully.\n");

    SDLAudioOut_SetVolume(SDLAudioOut_AudioVolume);

    sdl_samples_added=0;
    FixSpeed();
}
void SDLAudioOut_SubmitSamples(unsigned char** samples, int samples_a)
{
    if (!SDLAudioOut_Initialized)
        return;


    //if (is_rendering)
    //   return;
    // Nes emu call this method at the end of each frame to fill the sound buffer...
    // Code should still work if this method is not called
    for (int i = 0; i < samples_a; i++)
    {
        sample_r = (unsigned char)(samples[i][0] * volume);
        sample_l = (unsigned char)(samples[i][1] * volume);

        if (!is_rendering)
        {
            sdl_audio_samples[w_pos++ % samples_count] = sample_r;
            if (stereo_mode)
            {
                if (!SDLAudioOut_IsStereoNes)
                {
                    sdl_audio_samples[w_pos++ % samples_count] = sample_r ;
                }
                else
                {
                    sdl_audio_samples[w_pos++ % samples_count] = sample_l ;
                }

            }
            sdl_samples_added++;
        }
    }
}
void AudioCallback(void *userdata, Uint8* stream,  int len)
{
    if (!SDLAudioOut_Initialized)
        return;

    is_rendering = 1;

    // Write it to the data
    for (int i = 0; i < len; i++)
    {
        stream[i]= sdl_audio_samples[r_pos++ % samples_count];
    }

    sdl_samples_added-=len / block_align;

    if (VideoOUT_ENABLE_SPEED_LIMIT && NES_ON)
    {
        if (sdl_samples_added <= buffer_min)
        {
            if (fps_mode != 2)
            {
                fps_mode = 2;
                // PL is faster than nes, make nes faster
                VideoOUT_SetupTargetFPS(fps_nes_missle);
                // Console.WriteLine("ANES SDL RENDERERS: sound switched to OVERCLOCKED NES speed mode.");
            }
        }
        else if (sdl_samples_added >= buffer_limit)
        {
            if (fps_mode != 1)
            {
                fps_mode = 1;
                // nes is faster than PL, make PL faster
                VideoOUT_SetupTargetFPS(fps_pl_faster);
                // Console.WriteLine("ANES SDL RENDERERS: sound switched to PLAYER FASTER speed mode.");
            }
        }
        else
        {
            if (fps_mode != 0)
            {
                fps_mode = 0;
                // between 1000 and 2000, set to normal speed
                VideoOUT_SetupTargetFPS(fps_nes_normal);
                //  Console.WriteLine("ANES SDL RENDERERS: sound switched to normal speed mode.");
            }
        }
    }
    else
    {
        sdl_samples_added = 0;
        w_pos = 0;
        r_pos = w_pos + sdl_buffer_size;
    }

    is_rendering = 0;
}

void SDLAudioOut_Reset()
{
    SDLAudioOut_Shutdown();
    SDLAudioOut_Initialize();
}
void SDLAudioOut_SetVolume(unsigned char vol_prec)
{
    if (vol_prec > 0 && vol_prec <=100)
        volume = vol_prec / 100.0;
    else
        volume = 0;
}
void SDLAudioOut_Shutdown()
{
    SDL_CloseAudio();
}
void SDLAudioOut_SignalToggle(char started)
{
    FixSpeed();
}

void SDLAudioOut_Play()
{
    if (!SDLAudioOut_IsPlaying)
    {
        SDLAudioOut_IsPlaying = 1;
        SDL_PauseAudioDevice(audio_device_index, 0);
    }
    FixSpeed();
}
void SDLAudioOut_Pause()
{
    if (SDLAudioOut_IsPlaying)
    {
        SDLAudioOut_IsPlaying = 0;
        SDL_PauseAudioDevice(audio_device_index, 1);
    }
}
