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

#include "SDLOut/SDLVideoOut.h"
#include "SDLOut/SDLAudioOut.h"
#include "SDLOut/SDLInput.h"
#include "SDLOut/SDL_RENDERERS_DEFINES.h"
#include "NesEmu/NES.h"
#include "NesEmu/APU.h"
#include "NesEmu/ROM/INES.h"
#include "NesEmu/DIFINES.h"
#include "NesEmu/Mappers/_Mappers.h"
#include "VERSION.h"

int main(int argc,char* args[])
{
    printf("%s \n",APP_TITLE);
    printf("----------------------- \n");
    printf("%s \n",APP_SHORT_DESC);
    printf(" \n");
    printf("Version %s \n",APP_VERSION);
    printf("----------------------- \n");
    printf(" \n");
    printf("%s \n",APP_COPYRIGHT_DESC);
    printf(" \n");
    printf("  %s \n",APP_COPYRIGHT);
    printf("  %s \n",APP_EMAIL);
    printf(" \n");
    printf("Website/Repository link: %s \n",APP_REP_LINK);
    printf(" \n");
    printf("----------------------- \n");
    printf(" This program is free software: you can redistribute it and/or modify \n");
    printf(" it under the terms of the GNU General Public License as published by \n");
    printf(" the Free Software Foundation, either version 3 of the License, or \n");
    printf(" (at your option) any later version. \n");
    printf("  \n");
    printf(" This program is distributed in the hope that it will be useful, \n");
    printf(" but WITHOUT ANY WARRANTY; without even the implied warranty of \n");
    printf(" MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the \n");
    printf(" GNU General Public License for more details. \n");
    printf(" \n");
    printf(" You should have received a copy of the GNU General Public License \n");
    printf(" along with this program.If not, see<http://www.gnu.org/licenses/>. \n");
    printf(" \n");
    printf("ANES is based on My Nes, A Nintendo Entertainment System / Family Computer (Nes/Famicom) Emulator written in C#. \n");
    printf("My Nes is lisenced under GNU General Public License 3 as well. Please see <https://github.com/jegqamas/My-Nes> for more details. \n");
    printf(" \n");
    printf("NES is either a trademark or registered trademark of Nintendo of America Inc. \n");
    printf("Famicom is either a trademark or registered trademark of Nintendo Co., LTD. \n");
    printf("All other trademarks are property of their respective owners. \n");
    printf("ANES (Agile Nes) / My Nes is not affiliated with or endorsed by any of the companies mentioned. \n");
    printf("----------------------- \n");
    // Just in case
    NES_TargetFPS=60;
// Video
    VideoOUT_Driver= "opengl";
    VideoOUT_Filter= "0";// Linear
    VideoOUT_ENABLE_ASPECT_RATIO_KEEP=0;
    VideoOUT_ENABLE_SPEED_LIMIT=1;
    VideoOUT_SHOW_FPS=0;
    VideoOUT_WindowSizeMultiply = 1;// For nes res it will be big enough
    // Controllers
    Player1_ControlMode = SDLInput_ControlMode_Keyboard;
    Player1_GC_Index = 0;

    Player2_ControlMode = SDLInput_ControlMode_NONE;
    Player2_GC_Index = 1;
    // Audio
    SDLAudioOut_BufferSizeInKB=21;// 22 correct, 24 is safe
    SDLAudioOut_AudioFrequency=44100;
    SDLAudioOut_AudioVolume=100;
    SDLAudioOut_AudioChannels = 2;// Stereo by default

    // STEREO NES
    SDLAudioOut_IsStereoNes=0;// by renderer, not stereo nes by default
    IsStereoNes=0;// by apu, not stereo nes by default

    apu_channel_pan_sq1=150;
    apu_channel_pan_sq2=50;
    apu_channel_pan_nos=100;
    apu_channel_pan_trl=100;
    apu_channel_pan_dmc=100;

    int target_res_w=640;
    int target_res_h=480;
    char res_use_upscale=0;

    printf("command lines count %d \n",argc);
    // Command lines
    for (int i=2; i <argc; i++)
    {
        if (!strcmp(args[i], "-opengl"))
        {
            VideoOUT_Driver= "opengl";
        }
        if (!strcmp(args[i],"-dircet3d"))
        {
            VideoOUT_Driver= "dircet3d";
        }
        if (!strcmp(args[i], "-f_point"))
        {
            VideoOUT_Filter= "0";
        }
        if (!strcmp(args[i], "-f_linear"))
        {
            VideoOUT_Filter= "1";
        }
        if (!strcmp(args[i], "-kar"))
        {
            VideoOUT_ENABLE_ASPECT_RATIO_KEEP=1;
        }
        if (!strcmp(args[i], "-nkar"))
        {
            VideoOUT_ENABLE_ASPECT_RATIO_KEEP=0;
        }
        if (!strcmp(args[i], "-wins_m_1"))
        {
            VideoOUT_WindowSizeMultiply = 1;
        }
        if (!strcmp(args[i], "-wins_m_2"))
        {
            VideoOUT_WindowSizeMultiply = 2;
        }
        if (!strcmp(args[i], "-wins_m_3"))
        {
            VideoOUT_WindowSizeMultiply = 3;
        }
        if (!strcmp(args[i], "-wins_m_4"))
        {
            VideoOUT_WindowSizeMultiply = 4;
        }
        if (!strcmp(args[i], "-wins_m_5"))
        {
            VideoOUT_WindowSizeMultiply = 5;
        }
        if (!strcmp(args[i], "-wins_m_6"))
        {
            VideoOUT_WindowSizeMultiply = 6;
        }
        if (!strcmp(args[i], "-wins_m_7"))
        {
            VideoOUT_WindowSizeMultiply = 7;
        }
        if (!strcmp(args[i], "-wins_m_8"))
        {
            VideoOUT_WindowSizeMultiply = 8;
        }
        if (!strcmp(args[i], "-wins_m_9"))
        {
            VideoOUT_WindowSizeMultiply = 9;
        }
        if (!strcmp(args[i], "-wins_m_10"))
        {
            VideoOUT_WindowSizeMultiply = 10;
        }
        if (!strcmp(args[i], "-ures"))
        {
            res_use_upscale=1;
        }
        if (!strcmp(args[i], "-nures"))
        {
            res_use_upscale=0;
        }
        if (!strcmp(args[i], "-res_4_3"))
        {
            target_res_w=640;
            target_res_h=480;
        }
        if (!strcmp(args[i], "-res_hdtv"))
        {
            target_res_w=1280;
            target_res_h=720;
        }
        if (!strcmp(args[i], "-res_4_3_2"))
        {
            target_res_w=1280;
            target_res_h=960;
        }
        if (!strcmp(args[i], "-res_fullhd"))
        {
            target_res_w=1920;
            target_res_h=1080;
        }
        if (!strcmp(args[i], "-res_2k"))
        {
            target_res_w=2560;
            target_res_h=1440;
        }
        if (!strcmp(args[i], "-res_4k"))
        {
            target_res_w=3840;
            target_res_h=2160;
        }
        if (!strcmp(args[i], "-pl1_k"))
        {
            Player1_ControlMode = SDLInput_ControlMode_Keyboard;
        }
        if (!strcmp(args[i], "-pl1_gc"))
        {
            Player1_ControlMode = SDLInput_ControlMode_GameController;
        }
        if (!strcmp(args[i], "-pl1_gc_1"))
        {
            Player1_GC_Index = 0;
        }
        if (!strcmp(args[i], "-pl1_gc_2"))
        {
            Player1_GC_Index = 1;
        }
        if (!strcmp(args[i], "-pl2_k"))
        {
            Player2_ControlMode = SDLInput_ControlMode_Keyboard;
        }
        if (!strcmp(args[i], "-pl2_gc"))
        {
            Player2_ControlMode = SDLInput_ControlMode_GameController;
        }
        if (!strcmp(args[i], "-pl2_gc_1"))
        {
            Player2_GC_Index = 0;
        }
        if (!strcmp(args[i], "-pl2_gc_2"))
        {
            Player2_GC_Index = 1;
        }
        if (!strcmp(args[i], "-alt"))
        {
            UseAPULookUpTable=1;
        }
        if (!strcmp(args[i], "-afreq_110"))
        {
            SDLAudioOut_AudioFrequency= 11025;
        }
        if (!strcmp(args[i], "-afreq_220"))
        {
            SDLAudioOut_AudioFrequency= 22050;
        }
        if (!strcmp(args[i], "-afreq_441"))
        {
            SDLAudioOut_AudioFrequency=44100;
        }
        if (!strcmp(args[i], "-afreq_480"))
        {
            SDLAudioOut_AudioFrequency=48000;
        }
        if (!strcmp(args[i], "-afreq_882"))
        {
            SDLAudioOut_AudioFrequency=88200;
        }
        if (!strcmp(args[i], "-afreq_960"))
        {
            SDLAudioOut_AudioFrequency=96000;
        }
        if (!strcmp(args[i], "-abuff_1"))
        {
            SDLAudioOut_BufferSizeInKB= 21;
        }
        if (!strcmp(args[i], "-abuff_2"))
        {
            SDLAudioOut_BufferSizeInKB= 22;
        }
        if (!strcmp(args[i], "-abuff_3"))
        {
            SDLAudioOut_BufferSizeInKB= 23;
        }
        if (!strcmp(args[i], "-abuff_4"))
        {
            SDLAudioOut_BufferSizeInKB= 24;
        }
        if (!strcmp(args[i], "-abuff_5"))
        {
            SDLAudioOut_BufferSizeInKB= 25;
        }
        if (!strcmp(args[i], "-abuff_6"))
        {
            SDLAudioOut_BufferSizeInKB= 26;
        }
        if (!strcmp(args[i], "-abuff_7"))
        {
            SDLAudioOut_BufferSizeInKB= 27;
        }
        if (!strcmp(args[i], "-abuff_8"))
        {
            SDLAudioOut_BufferSizeInKB= 28;
        }
        if (!strcmp(args[i], "-amono"))
        {
            SDLAudioOut_AudioChannels= 1;
        }
        if (!strcmp(args[i], "-astereo"))
        {
            SDLAudioOut_AudioChannels= 2;
        }
        if (!strcmp(args[i], "-astereones"))
        {
            IsStereoNes= 1;
            SDLAudioOut_IsStereoNes=1;
            SDLAudioOut_AudioChannels= 2;// Force stereo !
        }
        if (!strcmp(args[i], "-mmc1a"))
        {
            Mapper001_MMCA_Mode=1;
        }
        if (!strcmp(args[i], "-mmc1sr"))
        {
            Mapper001_UseSRAMSwitch=1;
        }
        if (!strcmp(args[i], "-mmc3a"))
        {
            Mapper004_MMC3A_Mode=1;
        }
        if (!strcmp(args[i], "-mmc5m"))
        {
            Mapper005_UseSRAMMirroring=1;
        }
        if (!strcmp(args[i], "-axrom_b"))
        {
            Mapper007_BusConflictEnable=1;
        }
        if (!strcmp(args[i], "-camerica_fh"))
        {
            Mapper071_FirehawkMode=1;
        }
    }

    if (argc < 2)
    {
        // No parameter is passed, load game ines format
        INES_LoadFile("game.nes");

    }
    else
    {
        // Other commands will override the settings
        //ExecuteCommands(argc, args);

        // We are expecting file path
        INES_LoadFile(args[1]);
    }
    if (INES_IsValid)
    {
        // Load it into nes emu
        NESInitialize(GAME_SOURCE_INES, SDLAudioOut_AudioFrequency);

        if (NES_IsGameLoaded)
        {
            NES_ON=1;
            NES_PAUSED=0;

            //VideoOUT_EMU_SIGNAL_ON=1;
            VideoOut_ENABLE_EMU_ON_DELAY=1;
            VideoOut_EMU_ON_DELAY_TIMER=7;
            VideoOUT_SetupEmuFrameClock(NESClockFrame);

            printf("ANES: Game is loaded !! \n");
        }
    }
    VideoOUT_SetupTargetFPS(NES_TargetFPS);
    // READY !!
    // Initialize
    // Render res must be 256 x 240 (nes buffer), target res can be set to any
    // res higher than 256 x 240 but must be a multiple of
    // w * n x h * i  while n = 1, 1.5,2,2.5,3,3.5,...etc and i = 1, 1.5,2,2.5,3,3.5,...etc
    // use upscale should be set to 1 in order to use target res, otherwise render res 256 x 240 will
    // be used.
    VideoOUT_Initialize(256,240,target_res_w,target_res_h,res_use_upscale);

    // Turn it on !!
    VideoOUT_PAUSED=0;
    VideoOUT_ON=1;

    SDLInputInitialize();

    SDLAudioOut_SignalToggle(NES_IsGameLoaded);
    SDLAudioOut_Initialize();

// Run on loop with target fps unless VideoOUT_ON is off
    VideoOUT_Run();

    // Reached here means program finish
    VideoOUT_Shutdown();

    return 0;
}
