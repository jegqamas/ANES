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

// Initialize the cpu
// writeAccess: is the method that cpu uses to write into cpu memory space
// readAccess: is the method that cpu uses to read from cpu memory space
void CPUHardReset();
void CPUSoftReset();
void CPUShutDown();

// Clock cpu one instruction.
void CPUClock();

char CPU_IRQ_PIN;
char CPU_NMI_PIN;
char cpu_suspend_nmi;
char cpu_suspend_irq;

// flags
char cpu_flag_n;
char cpu_flag_v;
char cpu_flag_d;
char cpu_flag_i;
char cpu_flag_z;
char cpu_flag_c;
unsigned char register_p();
void set_register_p(unsigned char *val);
unsigned char register_pb();// with b set

// Addressing modes
void Imp____();
void IndX_R_();
void IndX_W_();
void IndX_RW();
void IndY_R_();
void IndY_W_();
void IndY_RW();
void Zpg_R__();
void Zpg_W__();
void Zpg_RW_();
void ZpgX_R_();
void ZpgX_W_();
void ZpgX_RW();
void ZpgY_R_();
void ZpgY_W_();
void ZpgY_RW();
void Imm____();
void ImA____();
void Abs_R__();
void Abs_W__();
void Abs_RW_();
void AbsX_R_();
void AbsX_W_();
void AbsX_RW();
void AbsY_R_();
void AbsY_W_();
void AbsY_RW();

// Instructions
void Interrupt();
void Branch(char condition);
void Push(unsigned char *val);
void Pull(unsigned char *val);
void ADC__();
void AHX__();
void ALR__();
void ANC__();
void AND__();
void ARR__();
void AXS__();
void ASL_M();
void ASL_A();
void BCC__();
void BCS__();
void BEQ__();
void BIT__();
void BRK__();
void BPL__();
void BNE__();
void BMI__();
void BVC__();
void BVS__();
void SED__();
void CLC__();
void CLD__();
void CLV__();
void CMP__();
void CPX__();
void CPY__();
void CLI__();
void DCP__();
void DEC__();
void DEY__();
void DEX__();
void EOR__();
void INC__();
void INX__();
void INY__();
void ISC__();
void JMP__();
void JMP_I();
void JSR__();
void LAR__();
void LAX__();
void LDA__();
void LDX__();
void LDY__();
void LSR_A();
void LSR_M();
void NOP__();
void ORA__();
void PHA__();
void PHP__();
void PLA__();
void PLP__();
void RLA__();
void ROL_A();
void ROL_M();
void ROR_A();
void ROR_M();
void RRA__();
void RTI__();
void RTS__();
void SAX__();
void SBC__();
void SEC__();
void SEI__();
void SHX__();
void SHY__();
void SLO__();
void SRE__();
void STA__();
void STX__();
void STY__();
void TAX__();
void TAY__();
void TSX__();
void TXA__();
void TXS__();
void TYA__();
void XAA__();
void XAS__();

