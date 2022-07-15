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
#include "INES.h"
#include "../Memory.h"
#include <stdio.h>
#include <unistd.h>


// Load rom file header, this will set INES_FileName and INES_IsValid
// fileName: the complete rom file path
void INES_LoadFile(char *file_name)
{
// 1 check if file exist or not
    if( access( file_name, F_OK ) == 0 )
    {
        INES_FileName=file_name;
        // file exists
// 1 open the file with read-binary mode
        FILE* fp;
        char header[16];
        fp = fopen(file_name, "rb");

// 2 read header
        fread(header, 16,1, fp);

        // CHECK INES
        if (header[0] != 'N' ||
                header[1] != 'E' ||
                header[2] != 'S' ||
                header[3] != 0x1A)
        {
            fclose(fp);
            INES_IsValid=0;
            printf("ANES: ERROR: This file is not INES !\n");
            return;
        }

        INES_PRGCount = header[4];
        INES_CHRCount = header[5];
        /*
        #define MIRRORING_Horz 80 // 0x50
        #define MIRRORING_Vert 68 //0x44
        #define MIRRORING_OneScA 0 //0x00
        #define MIRRORING_OneScB 85 //0x55
        #define MIRRORING_Full 228 //0xE4
        */
        switch (header[6] & 0x9)
        {
        case 0x0:
            INES_Mirroring = 0x50;// Horz
            break;
        case 0x1:
            INES_Mirroring = 0x44;// Vert
            break;
        case 0x8:
        case 0x9:
            INES_Mirroring = 0xE4;// Full
            break;
        }

        INES_HasBattery = (header[6] & 0x2) != 0x0;
        printf("ANES: HAS BATTERY: %d\n",INES_HasBattery);

        INES_HasTrainer = (header[6] & 0x4) != 0x0;
        printf("ANES: HAS TRAINER: %d\n",INES_HasTrainer);

        if ((header[7] & 0x0F) == 0)
            INES_MapperNumber = ((header[7] & 0xF0) | ((header[6] >> 4) & 0xF));
        else
            INES_MapperNumber = ((header[6] >> 4) & 0xF);

        printf("ANES: MAPPER NUMBER: %d\n",INES_MapperNumber);

        INES_IsVSUnisystem = (header[7] & 0x01) != 0;
        printf("ANES: Is VSUnisystem: %d\n",INES_HasTrainer);
        INES_IsPlaychoice10 = (header[7] & 0x02) != 0;
        printf("ANES: Is Playchoice10: %d\n",INES_HasTrainer);

        fseek(fp, 16, SEEK_SET);
        if (INES_HasTrainer)
        {
            //trn_dump = new char[512];
            trn_dump=  (char*)malloc(512);
            INES_TrainerSizeInBytes=512;
            fread(trn_dump, 512,1, fp);
        }
        else
        {
            trn_dump = 0;
            INES_TrainerSizeInBytes=0;
        }
        INES_PRGSizeInBytes = (INES_PRGCount * 0x4000);

        prg_dump = (char*)malloc(INES_PRGSizeInBytes);
        fread(prg_dump, INES_PRGSizeInBytes,1, fp);

        printf("ANES: PRG SIZE: %d bytes\n",INES_PRGSizeInBytes);

        if (INES_CHRCount > 0)
        {
            INES_CHRSizeInBytes = (INES_CHRCount * 0x2000);
            chr_dump = (char*)malloc(INES_CHRSizeInBytes);
            fread(chr_dump, INES_CHRSizeInBytes,1, fp);
        }
        else
        {
            chr_dump = 0;
            INES_CHRSizeInBytes = 0;
        }
        printf("ANES: CHR SIZE: %d bytes\n",INES_CHRSizeInBytes);
        // Finally, close the file and set is valid
        fclose(fp);
        INES_IsValid=1;
        printf("ANES: File is loaded success !\n");
    }
    else
    {
        // file doesn't exist
        INES_IsValid=0;
    }
}

// Close and free resources
void INES_Close()
{
    free( prg_dump);
    free(chr_dump);
    free(trn_dump);
}
