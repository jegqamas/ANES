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

#include "CPU.h"
#include "Memory.h"

// Initialize  the addressing mode and the instructions arrays
void (*cpu_addressings[])() =
{
    //         0x0,     0x1,     0x2,     0x3,     0x4,     0x5,     0x6,     0x7,     0x8,     0x9,     0xA,     0xB,     0xC,     0xD,     0xE,     0xF //////
    /*0x0*/Imp____, IndX_R_, ImA____, IndX_W_, Zpg_R__, Zpg_R__, Zpg_RW_, Zpg_W__, ImA____, Imm____, ImA____, Imm____, Abs_R__, Abs_R__, Abs_RW_, Abs_W__,// 0x0
    /*0x1*/Imp____, IndY_R_, Imp____, IndY_W_, ZpgX_R_, ZpgX_R_, ZpgX_RW, ZpgX_W_, ImA____, AbsY_R_, ImA____, AbsY_W_, AbsX_R_, AbsX_R_, AbsX_RW, AbsX_W_,// 0x1
    /*0x2*/Imp____, IndX_R_, ImA____, IndX_W_, Zpg_R__, Zpg_R__, Zpg_RW_, Zpg_W__, ImA____, Imm____, ImA____, Imm____, Abs_R__, Abs_R__, Abs_RW_, Abs_W__,// 0x2
    /*0x3*/Imp____, IndY_R_, Imp____, IndY_W_, ZpgX_R_, ZpgX_R_, ZpgX_RW, ZpgX_W_, ImA____, AbsY_R_, ImA____, AbsY_W_, AbsX_R_, AbsX_R_, AbsX_RW, AbsX_W_,// 0x3
    /*0x4*/ImA____, IndX_R_, ImA____, IndX_W_, Zpg_R__, Zpg_R__, Zpg_RW_, Zpg_W__, ImA____, Imm____, ImA____, Imm____, Abs_W__, Abs_R__, Abs_RW_, Abs_W__,// 0x4
    /*0x5*/Imp____, IndY_R_, Imp____, IndY_W_, ZpgX_R_, ZpgX_R_, ZpgX_RW, ZpgX_W_, ImA____, AbsY_R_, ImA____, AbsY_W_, AbsX_R_, AbsX_R_, AbsX_RW, AbsX_W_,// 0x5
    /*0x6*/ImA____, IndX_R_, ImA____, IndX_W_, Zpg_R__, Zpg_R__, Zpg_RW_, Zpg_W__, ImA____, Imm____, ImA____, Imm____, Imp____, Abs_R__, Abs_RW_, Abs_W__,// 0x6
    /*0x7*/Imp____, IndY_R_, Imp____, IndY_W_, ZpgX_R_, ZpgX_R_, ZpgX_RW, ZpgX_W_, ImA____, AbsY_R_, ImA____, AbsY_W_, AbsX_R_, AbsX_R_, AbsX_RW, AbsX_W_,// 0x7
    /*0x8*/Imm____, IndX_W_, Imm____, IndX_W_, Zpg_W__, Zpg_W__, Zpg_W__, Zpg_W__, ImA____, Imm____, ImA____, Imm____, Abs_W__, Abs_W__, Abs_W__, Abs_W__,// 0x8
    /*0x9*/Imp____, IndY_W_, Imp____, IndY_W_, ZpgX_W_, ZpgX_W_, ZpgY_W_, ZpgY_W_, ImA____, AbsY_W_, ImA____, AbsY_W_, Abs_W__, AbsX_W_, Abs_W__, AbsY_W_,// 0x9
    /*0xA*/Imm____, IndX_R_, Imm____, IndX_R_, Zpg_R__, Zpg_R__, Zpg_R__, Zpg_R__, ImA____, Imm____, ImA____, Imm____, Abs_R__, Abs_R__, Abs_R__, Abs_R__,// 0xA
    /*0xB*/Imp____, IndY_R_, Imp____, IndY_R_, ZpgX_R_, ZpgX_R_, ZpgY_R_, ZpgY_R_, ImA____, AbsY_R_, ImA____, AbsY_R_, AbsX_R_, AbsX_R_, AbsY_R_, AbsY_R_,// 0xB
    /*0xC*/Imm____, IndX_R_, Imm____, IndX_R_, Zpg_R__, Zpg_R__, Zpg_RW_, Zpg_R__, ImA____, Imm____, ImA____, Imm____, Abs_R__, Abs_R__, Abs_RW_, Abs_R__,// 0xC
    /*0xD*/Imp____, IndY_R_, Imp____, IndY_RW, ZpgX_R_, ZpgX_R_, ZpgX_RW, ZpgX_RW, ImA____, AbsY_R_, ImA____, AbsY_RW, AbsX_R_, AbsX_R_, AbsX_RW, AbsX_RW,// 0xD
    /*0xE*/Imm____, IndX_R_, Imm____, IndX_W_, Zpg_R__, Zpg_R__, Zpg_RW_, Zpg_W__, ImA____, Imm____, ImA____, Imm____, Abs_R__, Abs_R__, Abs_RW_, Abs_W__,// 0xE
    /*0xF*/Imp____, IndY_R_, Imp____, IndY_W_, ZpgX_R_, ZpgX_R_, ZpgX_RW, ZpgX_W_, ImA____, AbsY_R_, ImA____, AbsY_W_, AbsX_R_, AbsX_R_, AbsX_RW, AbsX_W_,// 0xF
};

void (*cpu_instructions[])() =
{
    //0x0,   0x1,   0x2,   0x3,   0x4,   0x5,   0x6,   0x7,   0x8,   0x9,   0xA,   0xB,   0xC,   0xD,   0xE,   0xF   //////
    /*0x0*/BRK__, ORA__, NOP__, SLO__, NOP__, ORA__, ASL_M, SLO__, PHP__, ORA__, ASL_A, ANC__, NOP__, ORA__, ASL_M, SLO__,// 0x0
    /*0x1*/BPL__, ORA__, NOP__, SLO__, NOP__, ORA__, ASL_M, SLO__, CLC__, ORA__, NOP__, SLO__, NOP__, ORA__, ASL_M, SLO__,// 0x1
    /*0x2*/JSR__, AND__, NOP__, RLA__, BIT__, AND__, ROL_M, RLA__, PLP__, AND__, ROL_A, ANC__, BIT__, AND__, ROL_M, RLA__,// 0x2
    /*0x3*/BMI__, AND__, NOP__, RLA__, NOP__, AND__, ROL_M, RLA__, SEC__, AND__, NOP__, RLA__, NOP__, AND__, ROL_M, RLA__,// 0x3
    /*0x4*/RTI__, EOR__, NOP__, SRE__, NOP__, EOR__, LSR_M, SRE__, PHA__, EOR__, LSR_A, ALR__, JMP__, EOR__, LSR_M, SRE__,// 0x4
    /*0x5*/BVC__, EOR__, NOP__, SRE__, NOP__, EOR__, LSR_M, SRE__, CLI__, EOR__, NOP__, SRE__, NOP__, EOR__, LSR_M, SRE__,// 0x5
    /*0x6*/RTS__, ADC__, NOP__, RRA__, NOP__, ADC__, ROR_M, RRA__, PLA__, ADC__, ROR_A, ARR__, JMP_I, ADC__, ROR_M, RRA__,// 0x6
    /*0x7*/BVS__, ADC__, NOP__, RRA__, NOP__, ADC__, ROR_M, RRA__, SEI__, ADC__, NOP__, RRA__, NOP__, ADC__, ROR_M, RRA__,// 0x7
    /*0x8*/NOP__, STA__, NOP__, SAX__, STY__, STA__, STX__, SAX__, DEY__, NOP__, TXA__, XAA__, STY__, STA__, STX__, SAX__,// 0x8
    /*0x9*/BCC__, STA__, NOP__, AHX__, STY__, STA__, STX__, SAX__, TYA__, STA__, TXS__, XAS__, SHY__, STA__, SHX__, AHX__,// 0x9
    /*0xA*/LDY__, LDA__, LDX__, LAX__, LDY__, LDA__, LDX__, LAX__, TAY__, LDA__, TAX__, LAX__, LDY__, LDA__, LDX__, LAX__,// 0xA
    /*0xB*/BCS__, LDA__, NOP__, LAX__, LDY__, LDA__, LDX__, LAX__, CLV__, LDA__, TSX__, LAR__, LDY__, LDA__, LDX__, LAX__,// 0xB
    /*0xC*/CPY__, CMP__, NOP__, DCP__, CPY__, CMP__, DEC__, DCP__, INY__, CMP__, DEX__, AXS__, CPY__, CMP__, DEC__, DCP__,// 0xC
    /*0xD*/BNE__, CMP__, NOP__, DCP__, NOP__, CMP__, DEC__, DCP__, CLD__, CMP__, NOP__, DCP__, NOP__, CMP__, DEC__, DCP__,// 0xD
    /*0xE*/CPX__, SBC__, NOP__, ISC__, CPX__, SBC__, INC__, ISC__, INX__, SBC__, NOP__, SBC__, CPX__, SBC__, INC__, ISC__,// 0xE
    /*0xF*/BEQ__, SBC__, NOP__, ISC__, NOP__, SBC__, INC__, ISC__, SED__, SBC__, NOP__, ISC__, NOP__, SBC__, INC__, ISC__,// 0xF
};

union CPURegister
{
    unsigned short v;
    struct
    {
        unsigned char l;
        unsigned char h;
    };
};

union CPURegister cpu_reg_pc;
union CPURegister cpu_reg_sp;
union CPURegister cpu_reg_ea;

unsigned char cpu_reg_a;
unsigned char cpu_reg_x;
unsigned char cpu_reg_y;

unsigned char cpu_m;
unsigned char cpu_opcode;
// Using temp values increase performance by avoiding memory allocation.
unsigned char cpu_byte_temp;
int cpu_int_temp;
int cpu_int_temp1;
unsigned char cpu_dummy;
int cpu_bool_tmp;
union CPURegister temp_add;

void CPUHardReset()
{
// registers
    cpu_reg_a = 0x00;
    cpu_reg_x = 0x00;
    cpu_reg_y = 0x00;

    cpu_reg_sp.l = 0xFD;
    cpu_reg_sp.h = 0x01;

    unsigned short rst = 0xFFFC;
    MEM_CPURead(&rst, &cpu_reg_pc.l);
    rst++;
    MEM_CPURead(&rst, &cpu_reg_pc.h);

    unsigned char p_rst=0;
    set_register_p(&p_rst);

    cpu_flag_i = 1;
    cpu_reg_ea.v = 0;
    cpu_opcode = 0;
    //interrupts
    CPU_IRQ_PIN = 0;
    CPU_NMI_PIN = 0;
    cpu_suspend_nmi = 0;
    cpu_suspend_irq = 0;
}
void CPUSoftReset()
{
    cpu_flag_i = 1;
    cpu_reg_sp.v -= 3;

    unsigned short add = 0xFFFC;
    MEM_CPURead(&add, &cpu_reg_pc.l);
    add++;
    MEM_CPURead(&add, &cpu_reg_pc.h);
}
void CPUShutDown()
{
// registers
    cpu_reg_a = 0x00;
    cpu_reg_x = 0x00;
    cpu_reg_y = 0x00;

    cpu_reg_sp.l = 0;
    cpu_reg_sp.h = 0;

    unsigned char p_rst=0;
    set_register_p(&p_rst);
    cpu_flag_i = 0;
    cpu_reg_ea.v = 0;
    cpu_opcode = 0;
    //interrupts
    CPU_IRQ_PIN = 0;
    CPU_NMI_PIN = 0;
    cpu_suspend_nmi = 0;
    cpu_suspend_irq = 0;
}

// Clock cpu one instruction.
void CPUClock()
{
    // First clock is to fetch opcode
    MEM_CPURead(&cpu_reg_pc.v, &cpu_opcode);
    cpu_reg_pc.v++;

    //printf("ANES: PC VAL = %d, OPCODE = %d \n", cpu_reg_pc.v,cpu_opcode);

    cpu_addressings[cpu_opcode]();
    cpu_instructions[cpu_opcode]();
    // Handle interrupts...
    if (CPU_IRQ_PIN || CPU_NMI_PIN)
    {
        MEM_CPURead(&cpu_reg_pc.v, &cpu_dummy);
        MEM_CPURead(&cpu_reg_pc.v, &cpu_dummy);
        Interrupt();
    }
}

unsigned char register_p()
{
    return (
               (cpu_flag_n ? 0x80 : 0) |
               (cpu_flag_v ? 0x40 : 0) |
               (cpu_flag_d ? 0x08 : 0) |
               (cpu_flag_i ? 0x04 : 0) |
               (cpu_flag_z ? 0x02 : 0) |
               (cpu_flag_c ? 0x01 : 0) | 0x20);
}
void set_register_p(unsigned char *val)
{
    cpu_flag_n = ((*val) & 0x80) != 0;
    cpu_flag_v = ((*val)  & 0x40) != 0;
    cpu_flag_d = ((*val)  & 0x08) != 0;
    cpu_flag_i = ((*val)  & 0x04) != 0;
    cpu_flag_z = ((*val)  & 0x02) != 0;
    cpu_flag_c = ((*val)  & 0x01) != 0;
}
unsigned char register_pb()
{
    return (
               (cpu_flag_n ? 0x80 : 0) |
               (cpu_flag_v ? 0x40 : 0) |
               (cpu_flag_d ? 0x08 : 0) |
               (cpu_flag_i ? 0x04 : 0) |
               (cpu_flag_z ? 0x02 : 0) |
               (cpu_flag_c ? 0x01 : 0) | 0x30);
}

// Addressing modes implementations
void Imp____()
{
    // No addressing mode ...
}
void IndX_R_()
{
    temp_add.h = 0;// the zero page boundary crossing is not handled.
    MEM_CPURead(&cpu_reg_pc.v, &temp_add.l);
    cpu_reg_pc.v++;// CLock 1
    MEM_CPURead(&temp_add.v,&cpu_dummy);// Clock 2
    temp_add.l += cpu_reg_x;

    MEM_CPURead(&temp_add.v, &cpu_reg_ea.l);// Clock 3
    temp_add.l++;

    MEM_CPURead(&temp_add.v, &cpu_reg_ea.h);// Clock 4

    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);
}
void IndX_W_()
{
    temp_add.h = 0;// the zero page boundary crossing is not handled.
    MEM_CPURead(&cpu_reg_pc.v, &temp_add.l);
    cpu_reg_pc.v++;// CLock 1
    MEM_CPURead(&temp_add.v, &cpu_dummy);// Clock 2
    temp_add.l += cpu_reg_x;

    MEM_CPURead(&temp_add.v, &cpu_reg_ea.l);// Clock 3
    temp_add.l++;

    MEM_CPURead(&temp_add.v, &cpu_reg_ea.h);// Clock 4
}
void IndX_RW()
{
    temp_add.h = 0;// the zero page boundary crossing is not handled.
    MEM_CPURead(&cpu_reg_pc.v, &temp_add.l);
    cpu_reg_pc.v++;// CLock 1
    MEM_CPURead(&temp_add.v, &cpu_dummy);// Clock 2
    temp_add.l += cpu_reg_x;

    MEM_CPURead(&temp_add.v, &cpu_reg_ea.l);// Clock 3
    temp_add.l++;

    MEM_CPURead(&temp_add.v, &cpu_reg_ea.h);// Clock 4

    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);
}
void IndY_R_()
{
    temp_add.h = 0;// the zero page boundary crossing is not handled.
    MEM_CPURead(&cpu_reg_pc.v, &temp_add.l);
    cpu_reg_pc.v++;// CLock 1
    MEM_CPURead(&temp_add.v, &cpu_reg_ea.l);// Clock 3
    temp_add.l++;// Clock 2
    MEM_CPURead(&temp_add.v, &cpu_reg_ea.h);// Clock 4

    cpu_reg_ea.l += cpu_reg_y;

    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);
    if (cpu_reg_ea.l < cpu_reg_y)
    {
        cpu_reg_ea.h++;
        MEM_CPURead(&cpu_reg_ea.v, &cpu_m);
    }
}
void IndY_W_()
{
    temp_add.h = 0;// the zero page boundary crossing is not handled.
    MEM_CPURead(&cpu_reg_pc.v, &temp_add.l);
    cpu_reg_pc.v++;// CLock 1

    MEM_CPURead(&temp_add.v, &cpu_reg_ea.l);
    temp_add.l++;// Clock 2

    MEM_CPURead(&temp_add.v, &cpu_reg_ea.h);// Clock 2
    cpu_reg_ea.l += cpu_reg_y;

    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 3
    if (cpu_reg_ea.l < cpu_reg_y)
        cpu_reg_ea.h++;
}
void IndY_RW()
{
    temp_add.h = 0;// the zero page boundary crossing is not handled.
    MEM_CPURead(&cpu_reg_pc.v, &temp_add.l);
    cpu_reg_pc.v++;// CLock 1
    MEM_CPURead(&temp_add.v, &cpu_reg_ea.l);
    temp_add.l++;// Clock 2
    MEM_CPURead(&temp_add.v, &cpu_reg_ea.h);// Clock 2

    cpu_reg_ea.l += cpu_reg_y;

    MEM_CPURead(&cpu_reg_ea.v, &cpu_dummy);// Clock 3
    if (cpu_reg_ea.l < cpu_reg_y)
        cpu_reg_ea.h++;

    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 3
}
void Zpg_R__()
{
    cpu_reg_ea.h = 0;
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 3
}
void Zpg_W__()
{
    cpu_reg_ea.h = 0;
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
}
void Zpg_RW_()
{
    cpu_reg_ea.h = 0;
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 3
}
void ZpgX_R_()
{
    cpu_reg_ea.h = 0;
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_ea.v, &cpu_dummy);// Clock 2
    cpu_reg_ea.l += cpu_reg_x;
    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 3
}
void ZpgX_W_()
{
    cpu_reg_ea.h = 0;
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_ea.v, &cpu_dummy);// Clock 2
    cpu_reg_ea.l += cpu_reg_x;
}
void ZpgX_RW()
{
    cpu_reg_ea.h = 0;
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_ea.v, &cpu_dummy);// Clock 2
    cpu_reg_ea.l += cpu_reg_x;
    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 3
}
void ZpgY_R_()
{
    cpu_reg_ea.h = 0;
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_ea.v, &cpu_dummy);// Clock 2
    cpu_reg_ea.l += cpu_reg_y;
    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 3
}
void ZpgY_W_()
{
    cpu_reg_ea.h = 0;
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_ea.v, &cpu_dummy);// Clock 2
    cpu_reg_ea.l += cpu_reg_y;
}
void ZpgY_RW()
{
    cpu_reg_ea.h = 0;
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_ea.v, &cpu_dummy);// Clock 2
    cpu_reg_ea.l += cpu_reg_y;
    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 3
}
void Imm____()
{
    MEM_CPURead(&cpu_reg_pc.v, &cpu_m);
    cpu_reg_pc.v++;// Clock 1
}
void ImA____()
{
    MEM_CPURead(&cpu_reg_pc.v, &cpu_dummy);
}
void Abs_R__()
{
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.h);
    cpu_reg_pc.v++;// Clock 2
    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 3
}
void Abs_W__()
{
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.h);
    cpu_reg_pc.v++;// Clock 2
}
void Abs_RW_()
{
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.h);
    cpu_reg_pc.v++;// Clock 2
    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 3
}
void AbsX_R_()
{
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.h);
    cpu_reg_pc.v++;// Clock 2

    cpu_reg_ea.l += cpu_reg_x;

    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 3
    if (cpu_reg_ea.l < cpu_reg_x)
    {
        cpu_reg_ea.h++;
        MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 4
    }
}
void AbsX_W_()
{
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.h);
    cpu_reg_pc.v++;// Clock 2

    cpu_reg_ea.l += cpu_reg_x;

    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 4
    if (cpu_reg_ea.l < cpu_reg_x)
        cpu_reg_ea.h++;
}
void AbsX_RW()
{
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.h);
    cpu_reg_pc.v++;// Clock 2

    cpu_reg_ea.l += cpu_reg_x;

    MEM_CPURead(&cpu_reg_ea.v, &cpu_dummy);// Clock 3
    if (cpu_reg_ea.l < cpu_reg_x)
        cpu_reg_ea.h++;

    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 4
}
void AbsY_R_()
{
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.h);
    cpu_reg_pc.v++;// Clock 2

    cpu_reg_ea.l += cpu_reg_y;

    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 4
    if (cpu_reg_ea.l < cpu_reg_y)
    {
        cpu_reg_ea.h++;
        MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 4
    }
}
void AbsY_W_()
{
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.h);
    cpu_reg_pc.v++;// Clock 2

    cpu_reg_ea.l += cpu_reg_y;

    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 4
    if (cpu_reg_ea.l < cpu_reg_y)
        cpu_reg_ea.h++;
}
void AbsY_RW()
{
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;// Clock 1
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.h);
    cpu_reg_pc.v++;// Clock 2

    cpu_reg_ea.l += cpu_reg_y;

    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 4
    if (cpu_reg_ea.l < cpu_reg_y)
        cpu_reg_ea.h++;

    MEM_CPURead(&cpu_reg_ea.v, &cpu_m);// Clock 4
}

// Instructions
void Interrupt()
{
    Push(&cpu_reg_pc.h);
    Push(&cpu_reg_pc.l);
    cpu_dummy = cpu_opcode == 0 ? register_pb() : register_p();
    Push(&cpu_dummy);

    // pins are detected during φ2 of previous cycle (before push about 2 ppu cycles)
    if (CPU_NMI_PIN)
        temp_add.v  = 0xFFFA;
    else
        temp_add.v  = 0xFFFE;

    // THEORY:
    // Once the vector requested, the interrupts are suspended and cleared
    // by setting the I flag and clearing the nmi detect flag. Also, the nmi
    // detection get suspended for 2 cycles while pulling PC, irq still can
    // be detected but will not be taken since I is set.
    cpu_suspend_nmi = 1;
    cpu_flag_i = 1;
    CPU_NMI_PIN = 0;

    MEM_CPURead(&temp_add.v, &cpu_reg_pc.l);
    temp_add.v++;
    MEM_CPURead(&temp_add.v, &cpu_reg_pc.h);

    cpu_suspend_nmi = 0;
}
void Branch(char condition)
{
    MEM_CPURead(&cpu_reg_pc.v, &cpu_byte_temp);
    cpu_reg_pc.v++;

    if (condition)
    {
        cpu_suspend_irq = 1;
        MEM_CPURead(&cpu_reg_pc.v, &cpu_dummy);
        cpu_reg_pc.l += cpu_byte_temp;
        cpu_suspend_irq = 0;
        if (cpu_byte_temp >= 0x80)
        {
            if (cpu_reg_pc.l >= cpu_byte_temp)
            {
                MEM_CPURead(&cpu_reg_pc.v, &cpu_dummy);
                cpu_reg_pc.h--;
            }
        }
        else
        {
            if (cpu_reg_pc.l < cpu_byte_temp)
            {
                MEM_CPURead(&cpu_reg_pc.v, &cpu_dummy);
                cpu_reg_pc.h++;
            }
        }
    }
}
void Push(unsigned char *val)
{
// Pass val as it it because it is alMEM_CPUReady a pointer
    MEM_CPUWrite(&cpu_reg_sp.v, val);
    cpu_reg_sp.l--;
}
void Pull(unsigned char *val)
{
    cpu_reg_sp.l++;
    // Pass val as it it because it is alMEM_CPUReady a pointer
    MEM_CPURead(&cpu_reg_sp.v, val);
}

void ADC__()
{
    cpu_int_temp = (cpu_reg_a + cpu_m + (cpu_flag_c ? 1 : 0));

    cpu_flag_v = ((cpu_int_temp ^ cpu_reg_a) & (cpu_int_temp ^ cpu_m) & 0x80) != 0;
    cpu_flag_n = (cpu_int_temp & 0x80) != 0;
    cpu_flag_z = (cpu_int_temp & 0xFF) == 0;
    cpu_flag_c = (cpu_int_temp >> 0x8) != 0;

    cpu_reg_a = (cpu_int_temp & 0xFF);
}
void AHX__()
{
    cpu_byte_temp = (cpu_reg_a & cpu_reg_x) & 7;
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_byte_temp);
}
void ALR__()
{
    cpu_reg_a &= cpu_m;

    cpu_flag_c = (cpu_reg_a & 0x01) != 0;

    cpu_reg_a >>= 1;

    cpu_flag_n = (cpu_reg_a & 0x80) != 0;
    cpu_flag_z = cpu_reg_a == 0;
}
void ANC__()
{
    cpu_reg_a &= cpu_m;
    cpu_flag_n = (cpu_reg_a & 0x80) != 0;
    cpu_flag_z = cpu_reg_a == 0;
    cpu_flag_c = (cpu_reg_a & 0x80) != 0;
}
void AND__()
{
    cpu_reg_a &= cpu_m;
    cpu_flag_n = (cpu_reg_a & 0x80) == 0x80;
    cpu_flag_z = (cpu_reg_a == 0);
}
void ARR__()
{
    cpu_reg_a = ((cpu_m & cpu_reg_a) >> 1) | (cpu_flag_c ? 0x80 : 0x00);

    cpu_flag_z = (cpu_reg_a & 0xFF) == 0;
    cpu_flag_n = (cpu_reg_a & 0x80) != 0;
    cpu_flag_c = (cpu_reg_a & 0x40) != 0;
    cpu_flag_v = (((cpu_reg_a << 1) ^ cpu_reg_a) & 0x40) != 0;
}
void AXS__()
{
    cpu_int_temp = (cpu_reg_a & cpu_reg_x) - cpu_m;

    cpu_flag_n = (cpu_int_temp & 0x80) != 0;
    cpu_flag_z = (cpu_int_temp & 0xFF) == 0;
    cpu_flag_c = (~cpu_int_temp >> 8) != 0;

    cpu_reg_x = cpu_int_temp & 0xFF;
}
void ASL_M()
{
    cpu_flag_c = (cpu_m & 0x80) == 0x80;
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_m);

    cpu_m = (cpu_m << 1) & 0xFE;

    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_m);

    cpu_flag_n = (cpu_m & 0x80) == 0x80;
    cpu_flag_z = (cpu_m == 0);
}
void ASL_A()
{
    cpu_flag_c = (cpu_reg_a & 0x80) == 0x80;

    cpu_reg_a = (cpu_reg_a << 1) & 0xFE;

    cpu_flag_n = (cpu_reg_a & 0x80) == 0x80;
    cpu_flag_z = (cpu_reg_a == 0);
}
void BCC__()
{
    Branch(!cpu_flag_c);
}
void BCS__()
{
    Branch(cpu_flag_c);
}
void BEQ__()
{
    Branch(cpu_flag_z);
}
void BIT__()
{
    cpu_flag_n = (cpu_m & 0x80) != 0;
    cpu_flag_v = (cpu_m & 0x40) != 0;
    cpu_flag_z = (cpu_m & cpu_reg_a) == 0;
}
void BRK__()
{
    MEM_CPURead( &cpu_reg_pc.v, &cpu_dummy);
    cpu_reg_pc.v++;
    Interrupt();
}
void BPL__()
{
    Branch(!cpu_flag_n);
}
void BNE__()
{
    Branch(!cpu_flag_z);
}
void BMI__()
{
    Branch(cpu_flag_n);
}
void BVC__()
{
    Branch(!cpu_flag_v);
}

void BVS__()
{
    Branch(cpu_flag_v);
}
void SED__()
{
    cpu_flag_d = 1;
}
void CLC__()
{
    cpu_flag_c = 0;
}
void CLD__()
{
    cpu_flag_d = 0;
}
void CLV__()
{
    cpu_flag_v = 0;
}
void CMP__()
{
    cpu_int_temp = cpu_reg_a - cpu_m;
    cpu_flag_n = (cpu_int_temp & 0x80) == 0x80;
    cpu_flag_c = (cpu_reg_a >= cpu_m);
    cpu_flag_z = (cpu_int_temp == 0);
}
void CPX__()
{
    cpu_int_temp = cpu_reg_x - cpu_m;
    cpu_flag_n = (cpu_int_temp & 0x80) == 0x80;
    cpu_flag_c = (cpu_reg_x >= cpu_m);
    cpu_flag_z = (cpu_int_temp == 0);
}
void CPY__()
{
    cpu_int_temp = cpu_reg_y - cpu_m;
    cpu_flag_n = (cpu_int_temp & 0x80) == 0x80;
    cpu_flag_c = (cpu_reg_y >= cpu_m);
    cpu_flag_z = (cpu_int_temp == 0);
}
void CLI__()
{
    cpu_flag_i = 0;
}
void DCP__()
{
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_m);

    cpu_m--;
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_m);

    cpu_int_temp = cpu_reg_a - cpu_m;

    cpu_flag_n = (cpu_int_temp & 0x80) != 0;
    cpu_flag_z = cpu_int_temp == 0;
    cpu_flag_c = (~cpu_int_temp >> 8) != 0;
}
void DEC__()
{
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_m);
    cpu_m--;
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_m);
    cpu_flag_n = (cpu_m & 0x80) == 0x80;
    cpu_flag_z = (cpu_m == 0);
}
void DEY__()
{
    cpu_reg_y--;
    cpu_flag_z = (cpu_reg_y == 0);
    cpu_flag_n = (cpu_reg_y & 0x80) == 0x80;
}
void DEX__()
{
    cpu_reg_x--;
    cpu_flag_z = (cpu_reg_x == 0);
    cpu_flag_n = (cpu_reg_x & 0x80) == 0x80;
}
void EOR__()
{
    cpu_reg_a ^= cpu_m;
    cpu_flag_n = (cpu_reg_a & 0x80) == 0x80;
    cpu_flag_z = (cpu_reg_a == 0);
}
void INC__()
{
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_m);
    cpu_m++;
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_m);
    cpu_flag_n = (cpu_m & 0x80) == 0x80;
    cpu_flag_z = (cpu_m == 0);
}
void INX__()
{
    cpu_reg_x++;
    cpu_flag_z = (cpu_reg_x == 0);
    cpu_flag_n = (cpu_reg_x & 0x80) == 0x80;
}
void INY__()
{
    cpu_reg_y++;
    cpu_flag_n = (cpu_reg_y & 0x80) == 0x80;
    cpu_flag_z = (cpu_reg_y == 0);
}
void ISC__()
{
    MEM_CPURead(&cpu_reg_ea.v, &cpu_byte_temp);

    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_byte_temp);

    cpu_byte_temp++;

    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_byte_temp);

    cpu_int_temp = cpu_byte_temp ^ 0xFF;
    cpu_int_temp1 = (cpu_reg_a + cpu_int_temp + (cpu_flag_c ? 1 : 0));

    cpu_flag_n = (cpu_int_temp1 & 0x80) != 0;
    cpu_flag_v = ((cpu_int_temp1 ^ cpu_reg_a) & (cpu_int_temp1 ^ cpu_int_temp) & 0x80) != 0;
    cpu_flag_z = (cpu_int_temp1 & 0xFF) == 0;
    cpu_flag_c = (cpu_int_temp1 >> 0x8) != 0;
    cpu_reg_a = cpu_int_temp1 & 0xFF;
}
void JMP__()
{
    cpu_reg_pc.v = cpu_reg_ea.v;
}
void JMP_I()
{
    // Fetch pointer
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.h);

    MEM_CPURead(&cpu_reg_ea.v, &cpu_reg_pc.l);
    cpu_reg_ea.l++; // only increment the low byte, causing the "JMP ($nnnn)" bug
    MEM_CPURead(&cpu_reg_ea.v, &cpu_reg_pc.h);
}
void JSR__()
{
    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.l);
    cpu_reg_pc.v++;

    // Store EAL at SP, see http://users.telenet.be/kim1-6502/6502/proman.html (see the JSR part)
    MEM_CPUWrite(&cpu_reg_sp.v, &cpu_reg_ea.l);

    Push(&cpu_reg_pc.h);
    Push(&cpu_reg_pc.l);

    MEM_CPURead(&cpu_reg_pc.v, &cpu_reg_ea.h);
    cpu_reg_pc.v = cpu_reg_ea.v;

}
void LAR__()
{
    cpu_reg_sp.l &= cpu_m;
    cpu_reg_a = cpu_reg_sp.l;
    cpu_reg_x = cpu_reg_sp.l;

    cpu_flag_n = (cpu_reg_sp.l & 0x80) != 0;
    cpu_flag_z = (cpu_reg_sp.l & 0xFF) == 0;
}
void LAX__()
{
    cpu_reg_x = cpu_reg_a = cpu_m;

    cpu_flag_n = (cpu_reg_x & 0x80) != 0;
    cpu_flag_z = (cpu_reg_x & 0xFF) == 0;
}
void LDA__()
{
    cpu_reg_a = cpu_m;
    cpu_flag_n = (cpu_reg_a & 0x80) == 0x80;
    cpu_flag_z = (cpu_reg_a == 0);
}
void LDX__()
{
    cpu_reg_x = cpu_m;
    cpu_flag_n = (cpu_reg_x & 0x80) == 0x80;
    cpu_flag_z = (cpu_reg_x == 0);
}
void LDY__()
{
    cpu_reg_y = cpu_m;
    cpu_flag_n = (cpu_reg_y & 0x80) == 0x80;
    cpu_flag_z = (cpu_reg_y == 0);
}
void LSR_A()
{
    cpu_flag_c = (cpu_reg_a & 1) == 1;
    cpu_reg_a >>= 1;
    cpu_flag_z = (cpu_reg_a == 0);
    cpu_flag_n = (cpu_reg_a & 0x80) != 0;
}
void LSR_M()
{
    cpu_flag_c = (cpu_m & 1) == 1;
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_m);
    cpu_m >>= 1;

    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_m);
    cpu_flag_z = (cpu_m == 0);
    cpu_flag_n = (cpu_m & 0x80) != 0;
}
void NOP__()
{
    // Do nothing.
}
void ORA__()
{
    cpu_reg_a |= cpu_m;
    cpu_flag_n = (cpu_reg_a & 0x80) == 0x80;
    cpu_flag_z = (cpu_reg_a == 0);
}
void PHA__()
{
    Push(&cpu_reg_a);
}
void PHP__()
{
    cpu_dummy = register_pb();
    Push(&cpu_dummy);
}
void PLA__()
{
    MEM_CPURead(&cpu_reg_sp.v, &cpu_dummy);
    Pull(&cpu_reg_a);
    cpu_flag_n = (cpu_reg_a & 0x80) == 0x80;
    cpu_flag_z = (cpu_reg_a == 0);
}
void PLP__()
{
    MEM_CPURead(&cpu_reg_sp.v, &cpu_dummy);
    Pull(&cpu_dummy);
    set_register_p(&cpu_dummy);
}
void RLA__()
{
    MEM_CPURead(&cpu_reg_ea.v, &cpu_byte_temp);

    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_byte_temp);

    cpu_dummy = (cpu_byte_temp << 1) | (cpu_flag_c ? 0x01 : 0x00);

    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_dummy);

    cpu_flag_n = (cpu_dummy & 0x80) != 0;
    cpu_flag_z = (cpu_dummy & 0xFF) == 0;
    cpu_flag_c = (cpu_byte_temp & 0x80) != 0;

    cpu_reg_a &= cpu_dummy;
    cpu_flag_n = (cpu_reg_a & 0x80) != 0;
    cpu_flag_z = (cpu_reg_a & 0xFF) == 0;
}
void ROL_A()
{
    cpu_byte_temp = (cpu_reg_a << 1) | (cpu_flag_c ? 0x01 : 0x00);

    cpu_flag_n = (cpu_byte_temp & 0x80) != 0;
    cpu_flag_z = (cpu_byte_temp & 0xFF) == 0;
    cpu_flag_c = (cpu_reg_a & 0x80) != 0;

    cpu_reg_a = cpu_byte_temp;
}
void ROL_M()
{
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_m);
    cpu_byte_temp = (cpu_m << 1) | (cpu_flag_c ? 0x01 : 0x00);

    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_byte_temp);
    cpu_flag_n = (cpu_byte_temp & 0x80) != 0;
    cpu_flag_z = (cpu_byte_temp & 0xFF) == 0;
    cpu_flag_c = (cpu_m & 0x80) != 0;
}
void ROR_A()
{
    cpu_byte_temp = (cpu_reg_a >> 1) | (cpu_flag_c ? 0x80 : 0x00);

    cpu_flag_n = (cpu_byte_temp & 0x80) != 0;
    cpu_flag_z = (cpu_byte_temp & 0xFF) == 0;
    cpu_flag_c = (cpu_reg_a & 0x01) != 0;

    cpu_reg_a = cpu_byte_temp;
}
void ROR_M()
{
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_m);

    cpu_byte_temp = (cpu_m >> 1) | (cpu_flag_c ? 0x80 : 0x00);
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_byte_temp);

    cpu_flag_n = (cpu_byte_temp & 0x80) != 0;
    cpu_flag_z = (cpu_byte_temp & 0xFF) == 0;
    cpu_flag_c = (cpu_m & 0x01) != 0;
}
void RRA__()
{
    MEM_CPURead(&cpu_reg_ea.v, &cpu_byte_temp);

    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_byte_temp);

    cpu_dummy = (cpu_byte_temp >> 1) | (cpu_flag_c ? 0x80 : 0x00);

    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_dummy);

    cpu_flag_n = (cpu_dummy & 0x80) != 0;
    cpu_flag_z = (cpu_dummy & 0xFF) == 0;
    cpu_flag_c = (cpu_byte_temp & 0x01) != 0;

    cpu_byte_temp = cpu_dummy;
    cpu_int_temp = (cpu_reg_a + cpu_byte_temp + (cpu_flag_c ? 1 : 0));

    cpu_flag_n = (cpu_int_temp & 0x80) != 0;
    cpu_flag_v = ((cpu_int_temp ^ cpu_reg_a) & (cpu_int_temp ^ cpu_byte_temp) & 0x80) != 0;
    cpu_flag_z = (cpu_int_temp & 0xFF) == 0;
    cpu_flag_c = (cpu_int_temp >> 0x8) != 0;
    cpu_reg_a = cpu_int_temp & 0xFF;
}
void RTI__()
{
    MEM_CPURead(&cpu_reg_sp.v, &cpu_dummy);

    Pull(&cpu_dummy);
    set_register_p(&cpu_dummy);

    Pull(&cpu_reg_pc.l);
    Pull(&cpu_reg_pc.h);
}
void RTS__()
{
    MEM_CPURead(&cpu_reg_sp.v, &cpu_dummy);
    Pull(&cpu_reg_pc.l);
    Pull(&cpu_reg_pc.h);

    cpu_reg_pc.v++;

    MEM_CPURead(&cpu_reg_pc.v, &cpu_dummy);
}
void SAX__()
{
    cpu_dummy = (cpu_reg_x & cpu_reg_a);
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_dummy);
}
void SBC__()
{
    cpu_m ^= 0xFF;
    cpu_int_temp = (cpu_reg_a + cpu_m + (cpu_flag_c ? 1 : 0));

    cpu_flag_n = (cpu_int_temp & 0x80) != 0;
    cpu_flag_v = ((cpu_int_temp ^ cpu_reg_a) & (cpu_int_temp ^ cpu_m) & 0x80) != 0;
    cpu_flag_z = (cpu_int_temp & 0xFF) == 0;
    cpu_flag_c = (cpu_int_temp >> 0x8) != 0;
    cpu_reg_a = cpu_int_temp & 0xFF;
}
void SEC__()
{
    cpu_flag_c = 1;
}
void SEI__()
{
    cpu_flag_i = 1;
}
void SHX__()
{
    cpu_byte_temp = cpu_reg_x & (cpu_reg_ea.h + 1);

    MEM_CPURead(&cpu_reg_ea.v, &cpu_dummy);
    cpu_reg_ea.l += cpu_reg_y;

    if (cpu_reg_ea.l < cpu_reg_y)
        cpu_reg_ea.h = cpu_byte_temp;

    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_byte_temp);
}
void SHY__()
{
    cpu_byte_temp = cpu_reg_y & (cpu_reg_ea.h + 1);

    MEM_CPURead(&cpu_reg_ea.v, &cpu_dummy);
    cpu_reg_ea.l += cpu_reg_x;

    if (cpu_reg_ea.l < cpu_reg_x)
        cpu_reg_ea.h = cpu_byte_temp;
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_byte_temp);
}
void SLO__()
{
    MEM_CPURead(&cpu_reg_ea.v, &cpu_byte_temp);

    cpu_flag_c = (cpu_byte_temp & 0x80) != 0;

    MEM_CPUWrite (&cpu_reg_ea.v, &cpu_byte_temp);

    cpu_byte_temp <<= 1;

    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_byte_temp);

    cpu_flag_n = (cpu_byte_temp & 0x80) != 0;
    cpu_flag_z = (cpu_byte_temp & 0xFF) == 0;

    cpu_reg_a |= cpu_byte_temp;
    cpu_flag_n = (cpu_reg_a & 0x80) != 0;
    cpu_flag_z = (cpu_reg_a & 0xFF) == 0;
}
void SRE__()
{
    MEM_CPURead(&cpu_reg_ea.v, &cpu_byte_temp);

    cpu_flag_c = (cpu_byte_temp & 0x01) != 0;

    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_byte_temp);

    cpu_byte_temp >>= 1;

    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_byte_temp);

    cpu_flag_n = (cpu_byte_temp & 0x80) != 0;
    cpu_flag_z = (cpu_byte_temp & 0xFF) == 0;

    cpu_reg_a ^= cpu_byte_temp;
    cpu_flag_n = (cpu_reg_a & 0x80) != 0;
    cpu_flag_z = (cpu_reg_a & 0xFF) == 0;
}
void STA__()
{
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_reg_a);
}
void STX__()
{
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_reg_x);
}
void STY__()
{
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_reg_y);
}
void TAX__()
{
    cpu_reg_x = cpu_reg_a;
    cpu_flag_n = (cpu_reg_x & 0x80) == 0x80;
    cpu_flag_z = (cpu_reg_x == 0);
}
void TAY__()
{
    cpu_reg_y = cpu_reg_a;
    cpu_flag_n = (cpu_reg_y & 0x80) == 0x80;
    cpu_flag_z = (cpu_reg_y == 0);
}
void TSX__()
{
    cpu_reg_x = cpu_reg_sp.l;
    cpu_flag_n = (cpu_reg_x & 0x80) != 0;
    cpu_flag_z = cpu_reg_x == 0;
}
void TXA__()
{
    cpu_reg_a = cpu_reg_x;
    cpu_flag_n = (cpu_reg_a & 0x80) == 0x80;
    cpu_flag_z = (cpu_reg_a == 0);
}
void TXS__()
{
    cpu_reg_sp.l = cpu_reg_x;
}
void TYA__()
{
    cpu_reg_a = cpu_reg_y;
    cpu_flag_n = (cpu_reg_a & 0x80) == 0x80;
    cpu_flag_z = (cpu_reg_a == 0);
}
void XAA__()
{
    cpu_reg_a = cpu_reg_x & cpu_m;
    cpu_flag_n = (cpu_reg_a & 0x80) != 0;
    cpu_flag_z = (cpu_reg_a & 0xFF) == 0;
}
void XAS__()
{
    cpu_reg_sp.l = cpu_reg_a & cpu_reg_x /*& ((dummyVal >> 8) + 1)*/;
    MEM_CPUWrite(&cpu_reg_ea.v, &cpu_reg_sp.l);
}
