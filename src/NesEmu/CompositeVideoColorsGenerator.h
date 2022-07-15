// This file is part of ANES (Agile Nes)
//
// A Nintendo Entertainment System / Family Computer (Nes/Famicom)
// Emulator written in C.
//
// Copyright � Alaa Ibrahim Hadid 2021 - 2022
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

/*float CVC_Saturation = 1.1F;// 1.2F
float CVC_Hue_tweak = +0.15f;
float CVC_Contrast = 0.8F;
float CVC_Brightness = 1.11F;//1.122F
float CVC_Gamma = 1.8F;*/

float CVC_Saturation;
float CVC_Hue_tweak;
float CVC_Contrast;
float CVC_Brightness;
float CVC_Gamma;

/// Generate composite video colors. Don't forget to adjust values first.
void GenerrateCompisteVideoColors(unsigned int * colors, int system_index);
