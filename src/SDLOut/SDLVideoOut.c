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

/*
Implements Resolution Blocks Upscaler method, see : <https://github.com/alaahadid/Resolution-Blocks-Upscaler>.
Resolution Blocks Upscaler is Written by Alaa Ibrahim Hadid.

Resolution Blocks Upscaler is licensed under the MIT License.

MIT License

Copyright (c) 2021 Alaa Ibrahim Hadid 2021 - 2022

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <stdio.h>
#include <string.h>
#include "SDLVideoOut.h"
#include "SDLAudioOut.h"
#include "SDLInput.h"
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>
#include "../NesEmu/PPU.h"
#include "../NesEmu/APU.h"
#include "../NesEmu/NES.h"
#include "../VERSION.h"

int add_x;
int add_y;
char add_x_use_flip_flop;
char add_y_use_flip_flop;
char add_x_flip_flop;
char add_y_flip_flop;
int current_x_t;
int current_y_t;
unsigned int current_pixel;
// Render information
int VideoOUT_RENDER_BUFFER_SIZE;
// SDL 2 Render stuff
SDL_Window* window;// Window to show
SDL_Renderer* renderer;// The renderer to use to render stuff on the window
SDL_Rect* targetPixelsRect;// The window screen pixels (presented in screen)
SDL_Rect* renderPixelsRect;// The render screen pixels original (i.e. input from emu, 256 x 224 for example)
Uint32* SCREEN_BUFFER;// temp screen buffer.

// Emu out pixels stuff
SDL_Texture* pixels_texture;// The texture that holds the pixels (image) to show
Uint32* buffer_pixels;// Screen pixels buffer.
int buffer_size;// Screen buffer size.

// FPS TEXT
char fps_text[20];
double fps_avg;
double fps_count;
double vid_fps;

long fps_time_finish_time;// the frame end time (time point)
long fps_time_start_time;// the frame start time (time point)
long fps_time_current;// how much time in seconds the rendering process toke
long fps_time_token;// how much time in seconds the rendering process toke
long fps_time_dead;// how much time remains for the target frame rate. For example, if a frame toke 3 milliseconds to complete, we'll have 16-3=13 dead time for 60 fps.
long fps_time_period;// how much time in seconds a frame should take for target fps. i.e. ~16 milliseconds for 60 fps.

long timer_time_last;// the frame end time (time point)
long timer_time_start;// the frame start time (time point)
long timer_time_token;// how much time in seconds the rendering process toke

SDL_Event event;// Events picked up from SDL2

void(*emu_clock_frame)();

struct timespec now_time;

void ToggleFullscreen()
{
    if((SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED)== SDL_WINDOW_MAXIMIZED)
    {
        SDL_RestoreWindow(window);
    }
    else if((SDL_GetWindowFlags(window) & SDL_WINDOWEVENT_MINIMIZED)== SDL_WINDOWEVENT_MINIMIZED)
    {
        SDL_RestoreWindow(window);
    }
    else
    {
        SDL_MaximizeWindow(window);
        SDL_WarpMouseInWindow(window, 0, 0);
    }
}

void GetTimeNanoSeconds(long * nanoseconds_val)
{
    clock_gettime(CLOCK_REALTIME, &now_time);
    (*nanoseconds_val)=now_time.tv_nsec;
}
void GetTimeMicrosecondsSeconds(long * microseconds_val)
{
    clock_gettime(CLOCK_REALTIME, &now_time);
    (*microseconds_val)=now_time.tv_sec * 1000;
}

// PUBLIC METHODS//////////////////////////////
// Initialize the video provider
void VideoOUT_Initialize(int render_width, int render_height, int target_res_width, int target_res_height, int use_res_upscale)
{
    // Setup res settings
    VideoOUT_RENDER_WIDTH=render_width;
    VideoOUT_RENDER_HEIGHT=render_height;
    VideoOUT_RES_USE_UPSCALE=use_res_upscale;
    VideoOUT_RES_WIDTH=target_res_width;
    VideoOUT_RES_HEIGHT=target_res_height;


    printf("ANES SDL RENDERERS: Renderer resolution is set to %d x %d \n",VideoOUT_RENDER_WIDTH,VideoOUT_RENDER_HEIGHT);

    printf("ANES SDL RENDERERS: RES UPSCALE is set to %d x %d \n",VideoOUT_RES_WIDTH,VideoOUT_RES_HEIGHT);


    if (!VideoOUT_RES_USE_UPSCALE)
        VideoOUT_SetRenderRectangle(0,0,VideoOUT_RENDER_WIDTH,VideoOUT_RENDER_HEIGHT);
    else
        VideoOUT_SetRenderRectangle(0,0,VideoOUT_RES_WIDTH,VideoOUT_RES_HEIGHT);

    if (VideoOUT_RES_USE_UPSCALE)
        printf("ANES SDL RENDERERS: Set to use RES UPSCALE\n");
    else
        printf("ANES SDL RENDERERS: Set to NOT TO use RES UPSCALE\n");


    printf("ANES SDL RENDERERS: Initializing video ...\n");
    SDL_Init(SDL_INIT_VIDEO);

    // Set video driver and configuration
    // TODO: allow to change driver
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, VideoOUT_Driver);
    printf("ANES SDL RENDERERS: video driver set to %s \n",VideoOUT_Driver);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, VideoOUT_Filter);

    printf("ANES SDL RENDERERS: filter = %s\n",VideoOUT_Filter =="0" ? "Point":"Linear");

    /*Important !!
      VSYNC cannot be enabled because it is a none-multithread emu
    */
    //SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    //printf("ANES SDL RENDERERS: VIDEO VSYNC IS ENABLED\n");

    /*Enable X11 mode*/
    //SDL_SetHint(SDL_HINT_VIDEO_X11_XVIDMODE, "1");
    //printf("ANES SDL RENDERERS: VIDEO X11 IS ENABLED\n");

    printf("ANES SDL RENDERERS: creating window...\n");

    char name[200];
    if (NES_IsGameLoaded)
        sprintf(name,"%s %s - %s",APP_TITLE,APP_VERSION, NES_GameLoadedName);
    else
        sprintf(name,"%s %s",APP_TITLE,APP_VERSION);

    // TODO: setup window title
    if (VideoOUT_RES_USE_UPSCALE)
        window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  VideoOUT_RES_WIDTH * VideoOUT_WindowSizeMultiply, VideoOUT_RES_HEIGHT * VideoOUT_WindowSizeMultiply, SDL_WINDOW_RESIZABLE | SDL_WINDOW_FOREIGN);
    else
        window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  640 * VideoOUT_WindowSizeMultiply, 480 * VideoOUT_WindowSizeMultiply, SDL_WINDOW_RESIZABLE | SDL_WINDOW_FOREIGN);

    printf("ANES SDL RENDERERS: window created successfully.\n");
    // TODO: toggle fullscreen mode here
    printf("ANES SDL RENDERERS: creating renderer for video ...\n");

    // Create the renderer that will render the texture.
    if (VideoOUT_ENABLE_VSYNC)
    {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
        printf("ANES SDL RENDERERS: VSYNC IS ENABLED !!.\n");
    }
    else
    {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
    }

    printf("ANES SDL RENDERERS: renderer is ready !\n");
    printf("ANES SDL RENDERERS: creating textures and buffers...\n");

    if (VideoOUT_RES_USE_UPSCALE)
    {
        // Create the texture
        pixels_texture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STATIC,
                                           VideoOUT_RES_WIDTH,
                                           VideoOUT_RES_HEIGHT);

        buffer_size = VideoOUT_RES_WIDTH * VideoOUT_RES_HEIGHT;
    }
    else
    {
        pixels_texture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STATIC,
                                           VideoOUT_RENDER_WIDTH,
                                           VideoOUT_RENDER_HEIGHT);

        buffer_size = VideoOUT_RENDER_WIDTH * VideoOUT_RENDER_HEIGHT;
    }

    add_x = (int)floor((double)VideoOUT_RES_WIDTH / VideoOUT_RENDER_WIDTH);
    add_x_use_flip_flop = ((double)VideoOUT_RES_WIDTH / VideoOUT_RENDER_WIDTH) - add_x != 0;

    add_y = (int)floor((double)VideoOUT_RES_HEIGHT / VideoOUT_RENDER_HEIGHT);
    add_y_use_flip_flop = ((double)VideoOUT_RES_HEIGHT / VideoOUT_RENDER_HEIGHT) - add_y != 0;

    // Buffers.
    SCREEN_BUFFER = (Uint32*)malloc(buffer_size * sizeof(Uint32));
    buffer_pixels= (Uint32*)malloc(buffer_size * sizeof(Uint32));
    //buffer_pixels = new Uint32[buffer_size];
    VideoOUT_RENDER_BUFFER_SIZE=VideoOUT_RENDER_WIDTH*VideoOUT_RENDER_HEIGHT;
    printf("ANES SDL RENDERERS: done !!\n");

    targetPixelsRect = (SDL_Rect* )malloc(sizeof(SDL_Rect));
    VideoOUT_CalculateTargetRect();

    VideoOUT_INITIALIZED = 1;
    printf("ANES SDL RENDERERS: video initialized successfully.\n");
}
void VideoOUT_Shutdown()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    free(targetPixelsRect);
    free(renderPixelsRect);
    free(SCREEN_BUFFER);
    SDL_DestroyTexture(pixels_texture);
    free(buffer_pixels);
}
void VideoOUT_Run()
{
    while (VideoOUT_ON)
    {
        if (!VideoOUT_PAUSED)
            VideoOUT_RunThreaded();
    }
}
// Run the video on thread one frame.
void VideoOUT_RunThreaded()
{
    //fps_time_start = SDL_GetTicks64();


    if (!VideoOUT_EMU_SIGNAL_ON)// Emu is off, draw snow.
    {
        for (int i = 0; i < buffer_size; i++)
        {
            unsigned char val = (rand() % 255);

            buffer_pixels[i] = (Uint32)((val << 16) | (val << 8) | val);
        }
        if (VideoOut_ENABLE_EMU_ON_DELAY)
        {
            if (VideoOut_EMU_ON_DELAY_TIMER >0)
            {
                VideoOut_EMU_ON_DELAY_TIMER--;
            }
            else
            {
                VideoOut_ENABLE_EMU_ON_DELAY=0;
                VideoOUT_EMU_SIGNAL_ON=1;

                if (VideoOut_ENABLE_EMU_ON_DELAY_START_WITH_FULLSCREEN)
                {
                    VideoOut_ENABLE_EMU_ON_DELAY_START_WITH_FULLSCREEN=0;
                    SDL_MaximizeWindow(window);
                     SDL_WarpMouseInWindow(window, 0, 0);
                }
            }
        }
    }
    else
    {
        // Run emulator
        emu_clock_frame();

        // 1 get video buffer
        if (!VideoOUT_RES_USE_UPSCALE)
        {
            memcpy(buffer_pixels,ppu_screen_pixels, buffer_size * sizeof(Uint32));
        }
        else
        {
            // Implements Resolution Blocks Upscaler method, see : <https://github.com/alaahadid/Resolution-Blocks-Upscaler>.
            current_x_t = 0;
            current_y_t = 0;
            add_y_flip_flop = 0;
            add_x_flip_flop = 0;

            for (int i = 0; i < VideoOUT_RENDER_BUFFER_SIZE; i++)// no skip
            {
                current_pixel = ppu_screen_pixels[i];

                // Fill a block
                for (int y_t = 0; y_t < add_y; y_t++)
                {
                    for (int x_t = 0; x_t < add_x; x_t++)
                    {
                        // Modify pixel here to change target pixel for effects ...
                        buffer_pixels[(current_x_t + x_t) + ((current_y_t + y_t) * VideoOUT_RES_WIDTH)] = current_pixel;
                    }
                }
                current_x_t += add_x;
                if (current_x_t >= VideoOUT_RES_WIDTH)
                {
                    current_x_t = 0;
                    current_y_t += add_y;
                    if (current_y_t >= VideoOUT_RES_HEIGHT)
                    {
                        current_y_t = 0;
                    }

                    if (add_y_use_flip_flop)
                    {
                        add_y_flip_flop = !add_y_flip_flop;
                        if (add_y_flip_flop)
                            add_y++;
                        else
                            add_y--;
                    }
                }

                if (add_x_use_flip_flop)
                {
                    add_x_flip_flop = !add_x_flip_flop;
                    if (add_x_flip_flop)
                        add_x++;
                    else
                        add_x--;
                }
            }
        }


        if (NES_PAUSED)
            SDLAudioOut_Pause();
        else if (!SDLAudioOut_IsPlaying)
            SDLAudioOut_Play();
        // 2 get audio buffer
        SDLAudioOut_SubmitSamples(audio_samples,audio_samples_added);
        //ANES::ClockFrame();
    }

    VideoOUT_IS_RENDERING = 1;
    // 1 clear the renderer
    SDL_RenderClear(renderer);

    // 2 update texture nad copy pixels
    if (!VideoOUT_RES_USE_UPSCALE)
        SDL_UpdateTexture(pixels_texture, NULL, buffer_pixels, VideoOUT_RENDER_WIDTH * sizeof(Uint32));
    else
        SDL_UpdateTexture(pixels_texture, NULL, buffer_pixels, VideoOUT_RES_WIDTH * sizeof(Uint32));

    SDL_RenderCopy(renderer, pixels_texture, renderPixelsRect, targetPixelsRect);

    // 3 present the renderer to user
    SDL_RenderPresent(renderer);

    // 4 Check input events
    VideoOUT_CheckEvents();

    // 5 HANDLE FPS limit
    GetTimeNanoSeconds(&fps_time_finish_time);
    //printf("start = %d nanosecond \n",fps_time_start_time);  printf("finish = %d nanosecond \n",fps_time_finish_time);
    fps_time_token = fps_time_finish_time - fps_time_start_time;
    //printf("tocken = %d nanosecond \n",fps_time_token);

    if (VideoOUT_ENABLE_SPEED_LIMIT)
    {
        fps_time_dead = fps_time_period - fps_time_token;
        // printf("dead = %d \n",fps_time_dead);
        if (fps_time_dead > 0 && fps_time_dead < 1000000000)
        {
            //std::this_thread::sleep_for(std::chrono::microseconds(fps_time_dead));
            //
            //printf("%d \n",fps_time_dead);
            /*while ((fps_time_last - fps_time_start) < fps_time_period)
            {
                GetTimeNanoSeconds(&fps_time_last);
            }*/

            // SDL_Delay(fps_time_dead /  1000000);// delay in milliseconds to releave the cpu
            usleep(fps_time_dead /  1000);
            // left over nanoseconds, just delay a little more bit to get the extra resolution
            GetTimeNanoSeconds(&fps_time_current);
            while ((fps_time_current - fps_time_start_time) < fps_time_period)
            {
                GetTimeNanoSeconds(&fps_time_current);
                if ((fps_time_current - fps_time_start_time) >= fps_time_period)
                    break;
                if ((fps_time_current - fps_time_start_time) < 0)
                    break;
            }
        }
    }

    if (VideoOUT_SHOW_FPS)
    {
        // update time token again
        GetTimeNanoSeconds(&fps_time_finish_time);
        fps_time_token = fps_time_finish_time - fps_time_start_time;
        if (fps_time_token>0)
        {
            // printf("fps %f  \n", (1000000000 / (double)fps_time_token));
            fps_avg += (1000000000 / (double)fps_time_token);
        }

        fps_count++;

        // Set FPS at one second time
        long valll=0;
        GetTimeMicrosecondsSeconds(&valll);
        if ( (valll - timer_time_start) >=1000)
        {
            //timer_time_start = SDL_GetTicks64();
            GetTimeMicrosecondsSeconds(&timer_time_start);

            vid_fps=0;
            if (fps_count > 0)
                vid_fps = fps_avg / fps_count;

            fps_avg = fps_count = 0;

            sprintf(fps_text, "FPS: %.2f", vid_fps);
            //VideoOUT_WriteFps(fps_text, 0x00FF00);

            char name[200];
            if (NES_IsGameLoaded)
                sprintf(name,"%s %s - %s - %s",APP_TITLE,APP_VERSION, fps_text,NES_GameLoadedName );
            else
                sprintf(name,"%s %s - %s", APP_TITLE,APP_VERSION, fps_text);

            SDL_SetWindowTitle(window,name);
        }
    }

    GetTimeNanoSeconds(&fps_time_start_time);
}
// Setup target fps. This should be set after initialize and/or after running a system to specify the target fps that the system should run at.
void VideoOUT_SetupTargetFPS(double target_fps)
{
    //fps_time_period = (Uint32)((1.0 / target_fps));
    //fps_time_period*=1000;

    double p = (1.0 / target_fps);
    p *=1000000000;
    fps_time_period= p;
    //printf("Frame period %d nanoseconds \n", fps_time_period);
}

void VideoOUT_SetupEmuFrameClock(void(*EmuFrameFunction)())
{
    emu_clock_frame=EmuFrameFunction;
}
void VideoOUT_SubmitFrame(int * buffer)
{

}
void VideoOUT_CheckEvents()
{
    SDLInputUpdate();
    // HANDLE INPUT
    while (SDL_PollEvent(&event) != 0)
    {
        //User requests quit
        if (event.type == SDL_QUIT)
        {
            VideoOUT_ON = 0;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.clicks >= 2)
            {
                ToggleFullscreen();
            }
        }
        else if (event.type == SDL_KEYDOWN)
        {
            //Select surfaces based on key press
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                if((SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED)== SDL_WINDOW_MAXIMIZED)
                {
                    SDL_RestoreWindow(window);
                }
                else if((SDL_GetWindowFlags(window) & SDL_WINDOWEVENT_MINIMIZED)== SDL_WINDOWEVENT_MINIMIZED)
                {
                    SDL_RestoreWindow(window);
                }
                else
                    VideoOUT_ON = 0;
            }
            if (event.key.keysym.sym == SDLK_F1)
            {
                VideoOUT_SHOW_FPS = !VideoOUT_SHOW_FPS;

                char name[200];
                if (VideoOUT_SHOW_FPS)
                {
                    if (NES_IsGameLoaded)
                        sprintf(name,"%s %s - %s - %s",APP_TITLE,APP_VERSION, fps_text, NES_GameLoadedName);
                    else
                        sprintf(name,"%s %s - %s",APP_TITLE,APP_VERSION, fps_text);

                }
                else
                {
                    if (NES_IsGameLoaded)
                        sprintf(name,"%s %s - %s",APP_TITLE,APP_VERSION,NES_GameLoadedName);
                    else
                        sprintf(name,"%s %s",APP_TITLE,APP_VERSION);
                }
                SDL_SetWindowTitle(window,name);


            }
            if (event.key.keysym.sym == SDLK_F3)
            {
                NESSoftReset();
            }
            if (event.key.keysym.sym == SDLK_F4)
            {
                NESHardReset();
            }
            if (event.key.keysym.sym == SDLK_F10)
            {
                VideoOUT_ENABLE_SPEED_LIMIT = !VideoOUT_ENABLE_SPEED_LIMIT;
            }
            if (event.key.keysym.sym == SDLK_F11)
            {
                VideoOUT_ENABLE_ASPECT_RATIO_KEEP = !VideoOUT_ENABLE_ASPECT_RATIO_KEEP;
                VideoOUT_CalculateTargetRect();
            }
            if (event.key.keysym.sym == SDLK_TAB)
            {
                ToggleFullscreen();
            }
        }
        else if (event.type == SDL_WINDOWEVENT)
        {
            VideoOUT_CalculateTargetRect();
        }
    }


}
void VideoOUT_CalculateTargetRect()
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);

    //targetPixelsRect = new SDL_Rect();

    if (VideoOUT_ENABLE_ASPECT_RATIO_KEEP && renderPixelsRect != NULL)
    {
        // Calculate aspect ratio occurding to NTSC standard resolution which Agile Nes uses
        VideoOUT_GetRatioStretchRectangle(640, 480, window_w, window_h,
                                          &targetPixelsRect->x,
                                          &targetPixelsRect->y,
                                          &targetPixelsRect->w,
                                          &targetPixelsRect->h);
    }
    else
    {
        // As it is !
        targetPixelsRect->x = 0;
        targetPixelsRect->y = 0;
        targetPixelsRect->h = window_h;
        targetPixelsRect->w = window_w;
    }
}
void VideoOUT_SetRenderRectangle(int x, int y, int w, int h)
{
// Setup render recta
    //renderPixelsRect = new SDL_Rect();
    renderPixelsRect = (SDL_Rect* )malloc(sizeof(SDL_Rect));
    renderPixelsRect->x = x;
    renderPixelsRect->y = y;
    renderPixelsRect->h = h;
    renderPixelsRect->w = w;
}
void VideoOUT_GetRenderRectangle(int *x, int *y, int *w, int *h)
{
    if (renderPixelsRect == NULL)
        return;
    *x = renderPixelsRect->x;
    *y = renderPixelsRect->y;
    *h = renderPixelsRect->h;
    *w = renderPixelsRect->w;
}
void VideoOUT_GetRatioStretchRectangle(int orgWidth, int orgHeight, int maxWidth, int maxHeight, int *out_x, int *out_y, int *out_w, int *out_h)
{
    float hRatio = orgHeight / maxHeight;
    float wRatio = orgWidth / maxWidth;
    int touchTargetFromOutside = 0;
    if ((wRatio > hRatio) ^ touchTargetFromOutside)
    {
        (*out_w) = maxWidth;
        (*out_h) = (orgHeight * maxWidth) / orgWidth;
    }
    else
    {
        (*out_h) = maxHeight;
        (*out_w) = (orgWidth * maxHeight) / orgHeight;
    }

    (*out_x) = (maxWidth - (*out_w)) / 2;
    (*out_y) = (maxHeight - (*out_h)) / 2;
}
