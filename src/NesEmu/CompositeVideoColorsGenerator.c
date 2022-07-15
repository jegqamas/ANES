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

#include "CompositeVideoColorsGenerator.h"
#include <math.h>

float levels[] =
{
    0.350F, 0.518F, 0.962F, 1.550F, // Signal low
    1.094F, 1.506F, 1.962F, 1.962F  // Signal high
};
int wave(int p, int color)
{
    return ((color + p + 8) % 12 < 6) ? 1 : 0;
}
float gammafix(float f, float gamma)
{
    return (float)(f < 0.0f ? 0.0f : pow(f, 2.2f / gamma));
}
int clamp(float v)
{
    return (int)(v < 0 ? 0 : v > 255 ? 255 : v);
}
/// Make a signal color from pixel data comes from NES -> Composite video -> RGB color
int MakeRGBcolor(int pixel, int system_index)
{
    // The input value is a NES color index (with de-emphasis bits).
    // We need RGB values. Convert the index into RGB.
    // For most part, this process is described at:
    //    http://wiki.nesdev.com/w/index.php/NTSC_video

    // The expected pixel should be
    // bit ...
    // 15 14 13 12  11 10 9 8   7 6 5 4  3 2 1 0
    //                 ´  | |   | | | |  | | | |
    //                    | |   | | | |  +-+-+-+---- Color sequence value
    //                    | |   | | +-+------------- Color level or strength
    //                    | |   | +----------------- Gray scale bit
    //                    +-+---+------------------- Emphasizes bits

    // Decode the color index
    int color = (pixel & 0x0F);
    int level = color < 0xE ? (pixel >> 4) & 3 : 1;
    char gray_scale = (pixel & 0x40) != 0;

    // Gray scale forces color to be 1
    if (gray_scale)
        color = 1;

    float lo_and_hi[] =
    {
        levels[level + ((color == 0x0) ? 4 : 0)],
        levels[level + ((color <= 0xC) ? 4 : 0)]
    };
    float black = 0.518F;
    float white = 1.962F;
    float attenuation = 0.746F;

    // Calculate the luma and chroma by emulating the relevant circuits:
    float y = 0.0f, i = 0.0f, q = 0.0f;

    for (int p = 0; p < 12; p++) // 12 clock cycles per pixel.
    {
        // NES NTSC modulator (square wave between two voltage levels):
        float spot = lo_and_hi[wave(p, color)];

        // De-emphasis bits attenuate a part of the signal:
        if (system_index == 0)
        {
            if ((pixel & 0x080) != 0 && wave(p, 0xC) == 1 ||// Red
                    (pixel & 0x100) != 0 && wave(p, 0x4) == 1 ||// Green
                    (pixel & 0x200) != 0 && wave(p, 0x8) == 1)  // Blue
            {
                spot *= attenuation;
            }
        }
        else
        {
            // PAL/DENDY
            if ((pixel & 0x080) != 0 && wave(p, 0x4) == 1 ||// Green
                    (pixel & 0x100) != 0 && wave(p, 0xC) == 1 ||// Red
                    (pixel & 0x200) != 0 && wave(p, 0x8) == 1)  // Blue
            {
                spot *= attenuation;
            }
        }

        // Normalize:
        float v = (spot - black) / (white - black);

        // Ideal TV NTSC demodulator:
        // Apply contrast/brightness
        v = (v - 0.5F) * CVC_Contrast + 0.5F;
        v *= CVC_Brightness / 12.0F;

        y += v;
        i += (float)(v * cos((M_PI / 6.0) * (p + CVC_Hue_tweak)));
        q += (float)(v * sin((M_PI / 6.0) * (p + CVC_Hue_tweak)));
    }

    i *= CVC_Saturation;
    q *= CVC_Saturation;

    // Convert YIQ into RGB according to FCC-sanctioned conversion matrix.
    return
        0x10000 * clamp(255 * gammafix(y + 0.946882F * i + 0.623557F * q, CVC_Gamma)) +
        0x00100 * clamp(255 * gammafix(y - 0.274788F * i - 0.635691F * q, CVC_Gamma)) +
        0x00001 * clamp(255 * gammafix(y - 1.108545F * i + 1.709007F * q, CVC_Gamma));
}

/// Generate composite video colors. Don't forget to adjust values first.
void GenerrateCompisteVideoColors(unsigned int * colors, int system_index)
{
    for (int i = 0; i < 1024; i++)
    {
        colors[i] = MakeRGBcolor(i, system_index) | (0xFF << 24);
    }
}


