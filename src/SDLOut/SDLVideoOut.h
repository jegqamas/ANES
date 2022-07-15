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

// Indicates if the video out is on (true) or off (false). Setting this to false in runtime would cause the application to exit.
int VideoOUT_ON;
// Indicates if the video out is paused (true) or not (false). Setting this to true will pause the video but not exit the loop.
int VideoOUT_PAUSED;
int VideoOUT_ENABLE_VSYNC;// enable or disable vsync. (requires video restart to take effect)
int VideoOUT_ENABLE_ASPECT_RATIO_KEEP;// enable or disable keep aspect ratio. (doesn't require video restart to take effect)
int VideoOUT_INITIALIZED;// indicates if the video provider is initialized and ready to use (NEVER SET THIS)
int VideoOUT_EMU_SIGNAL_ON;// indicates if the video currently receiving signals from emu core. Set this to off will set video to standby mode (snow rendering).
char VideoOut_ENABLE_EMU_ON_DELAY;
int VideoOut_EMU_ON_DELAY_TIMER;
int VideoOUT_IS_RENDERING;// indicates if the video provider is rendering or not. Set internally each frame. Has no effect if set externally.
int VideoOUT_SHOW_FPS;// indicates if the fps should be displayed
int VideoOUT_ENABLE_SPEED_LIMIT;

char* VideoOUT_Driver;
char* VideoOUT_Filter;

// Render information
int VideoOUT_RENDER_WIDTH;// The render width or expected pixels buffer width from the emu each frame clock. For nes res 256 x 240, this should be 256
int VideoOUT_RENDER_HEIGHT;// The render height or expected pixels buffer height from the emu each frame clock. For nes res 256 x 240, this should be 240
int VideoOUT_RES_USE_UPSCALE;// If true (1), video res RENDER_WIDTH x RENDER_HEIGHT will be upscaled into res RES_WIDTH x RES_HEIGHT. If false, res RENDER_WIDTH x RENDER_HEIGHT will be used.
int VideoOUT_RES_WIDTH;// The target res width that will be used if RES_USE_UPSCALE=1 (uses res RES_WIDTH x RES_HEIGHT)
int VideoOUT_RES_HEIGHT;// The target res height that will be used if RES_USE_UPSCALE=1 (uses res RES_WIDTH x RES_HEIGHT)
int VideoOUT_WindowSizeMultiply;

// PUBLIC METHODS//////////////////////////////
// Initialize the video provider
void VideoOUT_Initialize(int render_width, int render_height, int target_res_width, int target_res_height, int use_res_upscale);
// Run the video on thread loop.
void VideoOUT_Run();
// Run the video on thread once frame
void VideoOUT_RunThreaded();
// Setup target fps. This should be set after initialize and/or after running a system to specify the target fps that the system should run at.
void VideoOUT_SetupTargetFPS(double target_fps);

void VideoOUT_SetupEmuFrameClock(void(*EmuFrameFunction)());
void VideoOUT_SubmitFrame(int * buffer);
// Shutdown video
void VideoOUT_Shutdown();

// INTERNAL METHODS////////////////////////////
// Render video on thread.
void VideoOUT_Render();
void VideoOUT_CheckEvents();
void VideoOUT_CalculateTargetRect();
void VideoOUT_SetRenderRectangle(int x, int y, int w, int h);
void VideoOUT_GetRenderRectangle(int *x, int *y, int *w, int *h);
void VideoOUT_GetRatioStretchRectangle(int orgWidth, int orgHeight, int maxWidth, int maxHeight, int *out_x, int *out_y, int *out_w, int *out_h);
void VideoOUT_WriteFps(const char* status, int color);
