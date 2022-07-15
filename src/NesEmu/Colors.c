// This file is part of ANES (Agile Nes)
//
// A Nintendo Entertainment System / Family Computer (Nes/Famicom)
// Emulator written in C.
//
// Copyright © Alaa Ibrahim Hadid 2021
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

#include "Colors.h"

// Color sequences
unsigned char color_temp_blue__sequence[] = { 7, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 1, 1, 1 };
unsigned char color_temp_red___sequence[] = { 7, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 1, 1 };
unsigned char color_temp_green_sequence[] = { 7, 1, 1, 1, 1, 2, 3, 4, 5, 6, 7, 6, 5, 1, 1, 1 };

float Min(float a, float b)
{
	return a <= b ? a : b;
}

float Max(float a, float b)
{
	return a >= b ? a : b;
}
// Convert an RGB value into an HLS value.
void RgbToHls(unsigned int r_in, unsigned int g_in,unsigned int b_in, double* h, double* l, double* s)
{
    float r = (r_in / 255.0f);
	float g = (g_in / 255.0f);
	float b = (b_in / 255.0f);

	float min = Min(Min(r, g), b);
	float max = Max(Max(r, g), b);
	float delta = max - min;

	(*l) = (max + min) / 2;

	if (delta == 0)
	{
		(*h) = 0;
		(*s) = 0.0f;
	}
	else
	{
		(*s) = ((*l) <= 0.5) ? (delta / (max + min)) : (delta / (2 - max - min));

		float hue;

		if (r == max)
		{
			hue = ((g - b) / 6) / delta;
		}
		else if (g == max)
		{
			hue = (1.0f / 3) + ((b - r) / 6) / delta;
		}
		else
		{
			hue = (2.0f / 3) + ((r - g) / 6) / delta;
		}

		if (hue < 0)
			hue += 1;
		if (hue > 1)
			hue -= 1;

		(*h)= (int)(hue * 360);
	}

}
double QqhToRgb(double q1, double q2, double hue)
{
    if (hue > 360) hue -= 360;
    else if (hue < 0) hue += 360;

    if (hue < 60) return q1 + (q2 - q1) * hue / 60;
    if (hue < 180) return q2;
    if (hue < 240) return q1 + (q2 - q1) * (240 - hue) / 60;
    return q1;
}
// Convert an HLS value into an RGB value.
void HlsToRgb(double h, double l, double s,  int* r, int* g, int* b)
{
    double p2;
    if (l <= 0.5)
        p2 = l * (1 + s);
    else
    p2 = l + s - l * s;

    double p1 = 2 * l - p2;
    double double_r;double double_g; double double_b;
    if (s == 0)
    {
        double_r = l;
        double_g = l;
        double_b = l;
    }
    else
    {
        double_r = QqhToRgb(p1, p2, h + 120);
        double_g = QqhToRgb(p1, p2, h);
        double_b = QqhToRgb(p1, p2, h - 120);
    }

    // Convert RGB to the 0 to 255 range.
    (*r) = (int)(double_r * 255.0) & 0xFF;
    (*g) = (int)(double_g * 255.0) & 0xFF;
    (*b) = (int)(double_b * 255.0) & 0xFF;
}


int MakeRGBColor(unsigned int pixel_data, int system_index)
{
    // Implements the color decoding as written here <https://github.com/alaahadid/Nes-Docs/blob/main/Color%20Decoding%20In%20PPU.txt>.
    // It is better to be stored in color indexes for better performance

    // The expected pixel should be
    // bit ...
    // 15 14 13 12  11 10 9 8   7 6 5 4  3 2 1 0
    //                 ´  | |   | | | |  | | | |
    //                    | |   | | | |  +-+-+-+---- Color sequence value
    //                    | |   | | +-+------------- Color level or strength
    //                    | |   | +----------------- Gray scale bit
    //                    +-+---+------------------- Emphasizes bits


    int val = pixel_data & 0xF;// or sequence step
    int level = (pixel_data & 0x30) << 2;
    char gray = (pixel_data & 0x40) != 0;
    int emphasis = (pixel_data & 0x380) >> 7;

    // GRAY SCALE !!
    if (gray)
    {
        val = 0;
    }

    if (val >= 14)
        level = 0;

    int blue = ((color_temp_blue__sequence[val] << 3) | level);
    int red = ((color_temp_red___sequence[val] << 3) | level);
    int green = ((color_temp_green_sequence[val] << 3) | level);

    char e_red = 0;
    char e_green = 0;
    char e_blue = 0;

    // EMPHASIZE !!
    if (system_index == 0)
        e_red = (emphasis & 0x1) != 0;
    else
        e_green = (emphasis & 0x1) != 0;

    if (system_index == 0)
        e_green = (emphasis & 0x2) != 0;
    else
        e_red = (emphasis & 0x2) != 0;

    e_blue = (emphasis & 0x4) != 0;

    if (e_blue)
    {
        blue |= 0x7;
    }
    if (e_red)
    {
        red |= 0x7;
    }
    if (e_green)
    {
        green |= 0x7;
    }

    // We are using FCC NTSC Standard (this is not needed since it is only for transforming colors from
    // nes into tv, here there is no tv and we have nes rgb pure colors)
    // Convert RGB to YIQ
    // 1 We need to convert r,g,b from 0-255 value into 0-1 (double)
    /* double red_one = (double)red / 255.0;
     double green_one = (double)green / 255.0;
     double blue_one = (double)blue / 255.0;

     // 2 Apply the matrix to get YIQ
     double Y = (0.30 * red_one) + (0.59 * green_one) + (0.11 * blue_one);
     double I = (0.599 * red_one) + (-0.2773 * green_one) + (-0.3217 * blue_one);
     double Q = (0.213 * red_one) + (-0.5251 * green_one) + (0.3121 * blue_one);

     // 3 Convert back from YIQ into RGB
     double red_new = Y + (0.9469 * I) + (0.6236 * Q);
     double green_new = Y + (-0.2748 * I) + (-0.6357 * Q);
     double blue_new = Y + (-1.1 * I) + (1.7 * Q);

     // 4 Return from 0-1 value into 0-255 value
     red = (int)(red_new * 255);
     green = (int)(green_new * 255);
     blue = (int)(blue_new * 255);*/

    double l = 0;
    double h = 0;
    double s = 0;

    RgbToHls(red, green, blue, &h, &l, &s);

    // modify
    h -= 15;// In real nes, HUE is adjusted by 15 degree
    double l_diff = 1 - l;
    l += (l_diff * Color_Light_Add) / 1000.0;

    double s_diff = 1 - s;
    s += (s_diff * Color_Saturation_Add) / 1000.0;

    HlsToRgb(h, l, s, &red, &green, &blue);

    // return color R8G8B8 in format
    return ((red & 0xFF) << 16) | ((green & 0xFF) << 8) | (blue & 0xFF);
}


void GenerrateColors(unsigned int * colors, int system_index)
{
    for (unsigned int i = 0; i < 1024; i++)
    {
        colors[i] = MakeRGBColor(i, system_index) | (0xFF << 24);
    }
}
