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

typedef struct
{
    double K;// represents k constant
    double K_1;// repesenet k - 1 value
    double y;// represent y or current filtered data
    double y_1;// represent y-1
    double x;// represent x of current input data
    double x_1;// represent x-1

} SoundFilterInfo;

void SoundFilterReset(SoundFilterInfo *inf);
double SoundFilterGetK(double dt, double fc);
void SoundLowPassDoFiltering(double *sample, double *filtered, SoundFilterInfo *inf);
void SoundHighPassDoFiltering(double *sample, double *filtered, SoundFilterInfo *inf);
