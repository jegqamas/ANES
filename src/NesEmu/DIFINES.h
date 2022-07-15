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


// The game is loaded as ines format file
#define GAME_SOURCE_INES 0

#define TVFORMAT_NTSC 0
#define TVFORMAT_PALB 1
#define TVFORMAT_DENDY 2

// Mirroring value:
// 0000 0000
// ddcc bbaa
// aa: index for area 0x2000
// bb: index for area 0x2400
// cc: index for area 0x2800
// dd: index for area 0x2C00
/*
            (  D  C  B  A)
Horz:  $50  (%01 01 00 00)
Vert:  $44  (%01 00 01 00)
1ScA:  $00  (%00 00 00 00)
1ScB:  $55  (%01 01 01 01)
Full:  $E4  (%11 10 01 00)
*/
#define MIRRORING_Horz 0x50
#define MIRRORING_Vert 0x44
#define MIRRORING_OneScA 0x00
#define MIRRORING_OneScB 0x55
#define MIRRORING_Full 0xE4

/* Nes irq flags */
#define IRQ_FLAG_APU 0x1
#define IRQ_FLAG_DMC 0x2
#define IRQ_FLAG_BOARD 0x4

#define IRQ_FLAG_APU_OFF 0xFE
#define IRQ_FLAG_DMC_OFF 0xFD
#define IRQ_FLAG_BOARD_OFF 0xFB

#define NES_SCREEN_WIDTH 256
#define NES_SCREEN_HEIGHT 240

#define PRG_AREA_0000 0
#define PRG_AREA_1000 1
#define PRG_AREA_2000 2
#define PRG_AREA_3000 3
#define PRG_AREA_4000 4
#define PRG_AREA_5000 5
#define PRG_AREA_6000 6
#define PRG_AREA_7000 7
#define PRG_AREA_8000 8
#define PRG_AREA_9000 9
#define PRG_AREA_A000 10
#define PRG_AREA_B000 11
#define PRG_AREA_C000 12
#define PRG_AREA_D000 13
#define PRG_AREA_E000 14
#define PRG_AREA_F000 15

#define CHR_AREA_0000 0
#define CHR_AREA_0400 1
#define CHR_AREA_0800 2
#define CHR_AREA_0C00 3
#define CHR_AREA_1000 4
#define CHR_AREA_1400 5
#define CHR_AREA_1800 6
#define CHR_AREA_1C00 7

#define NMT_AREA_2000_NT0 0
#define NMT_AREA_2400_NT1 1
#define NMT_AREA_2800_NT2 2
#define NMT_AREA_2C00_NT3 3







