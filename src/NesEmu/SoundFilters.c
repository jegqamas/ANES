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

#include "SoundFilters.h"
#include <math.h>

void SoundFilterReset(SoundFilterInfo *inf)
{
    (*inf).y = (*inf).y_1 = (*inf).x = (*inf).x_1 = 0;
}
double SoundFilterGetK(double dt, double fc)
{
    double V = (2 * M_PI * dt * fc);

    return (V / (V + 1));
}
void SoundLowPassDoFiltering(double *sample, double *filtered, SoundFilterInfo *inf)
{
    (*filtered) = (inf->K * (*sample)) + (inf->K_1 * inf->y_1);
    inf->x_1 = (*sample);
    inf->y_1 =  (*filtered);
}
void SoundHighPassDoFiltering(double *sample, double *filtered, SoundFilterInfo *inf)
{
    (*filtered) = (inf->K * inf->y_1) + (inf->K * ((*sample) - inf->x_1));
    inf->x_1 = (*sample);
    inf->y_1 =  (*filtered);
}
