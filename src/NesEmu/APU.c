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

#include "NES.h"
#include "APU.h"
#include "DMA.h"
#include "DIFINES.h"

#include "SC_DMC.h"
#include "SC_Noise.h"
#include "SC_Square1.h"
#include "SC_Square2.h"
#include "SC_Triangle.h"

#include "SoundFilters.h"

void(*apu_reg_update_func[0x20])();
void(*apu_board_sound_get_sample)(double *board_sample);
void(*apu_board_clock_length)();
void(*apu_board_clock_envelope)();
void(*apu_board_clock)();
void(*apu_board_clock_single)();
void(*apu_upfate_playback)();
char apu_irq_enabled;
char apu_irq_flag;
char apu_irq_do_it;

char apu_seq_mode;
int apu_ferq_f;// IRQ clock frequency
int apu_ferq_l;// Length counter clock
int apu_ferq_e;// Envelope counter clock
int apu_cycle_f;
int apu_cycle_f_t;
int apu_cycle_e;
int apu_cycle_l;
char apu_odd_l;
char apu_check_irq;
char apu_do_env;
char apu_do_length;

// DAC
double audio_playback_amplitude;
int audio_playback_peek_limit;
char audio_playback_dac_initialized;

double cpu_clock_per_frame;
double apu_target_samples_count_per_frame;

int audio_samples_count;
double tnd_table[204];
double pulse_table[32];
double channels_out[5];
double mixer_small_table[16];
double mixer_big_table[128];

// Output values
double audio_board_x;

// Output values
double audio_x_r;
unsigned char audio_y_r;
double audio_x_l;
unsigned char audio_y_l;
double audio_bp_ratio1;
double audio_bp_ratio1_timer;

double audio_x_r_av;
double audio_x_r_clk;
double audio_x_l_av;
double audio_x_l_clk;

double apu_l_v_sq1;
double apu_l_v_sq2;
double apu_l_v_nos;
double apu_l_v_trl;
double apu_l_v_dmc;

double apu_r_v_sq1;
double apu_r_v_sq2;
double apu_r_v_nos;
double apu_r_v_trl;
double apu_r_v_dmc;

int PORT0;
int PORT1;
int inputStrobe;

void APUGetPrec(double inVal, double inMax, double outMax, double * val)
{
    (*val) = (outMax * inVal) / inMax;
}
void APUApplyChannelPans()
{
    // Adjust pan for stereo nes
    double channel_sq1 = ((double)apu_channel_pan_sq1 / 100.0);

    if (channel_sq1 > 1.0)
    {
        // Adjusting right channel
        apu_l_v_sq1 = 1.0;
        apu_r_v_sq1 = 2.0 - channel_sq1;
    }
    else if (channel_sq1 == 1.0)
    {
        apu_r_v_sq1 = 1.0;
        apu_l_v_sq1 = 1.0;
    }
    else
    {
        apu_r_v_sq1 = 1.0;
        apu_l_v_sq1 = channel_sq1;
    }

    double channel_sq2 = ((double)apu_channel_pan_sq2 / 100.0);

    if (channel_sq2 > 1.0)
    {
        // Adjusting right channel
        apu_l_v_sq2 = 1.0;
        apu_r_v_sq2 = 2.0 - channel_sq2;
    }
    else if (channel_sq2 == 1.0)
    {
        apu_r_v_sq2 = 1.0;
        apu_l_v_sq2 = 1.0;
    }
    else
    {
        apu_r_v_sq2 = 1.0;
        apu_l_v_sq2 = channel_sq2;
    }

    double channel_nos = ((double)apu_channel_pan_nos / 100.0);

    if (channel_nos > 1.0)
    {
        // Adjusting right channel
        apu_l_v_nos = 1.0;
        apu_r_v_nos = 2.0 - channel_nos;
    }
    else if (channel_nos == 1.0)
    {
        apu_r_v_nos = 1.0;
        apu_l_v_nos = 1.0;
    }
    else
    {
        apu_r_v_nos = 1.0;
        apu_l_v_nos = channel_nos;
    }

    double channel_trl = ((double)apu_channel_pan_trl / 100.0);

    if (channel_trl > 1.0)
    {
        // Adjusting right channel
        apu_l_v_trl = 1.0;
        apu_r_v_trl = 2.0 - channel_trl;
    }
    else if (channel_trl == 1.0)
    {
        apu_r_v_trl = 1.0;
        apu_l_v_trl = 1.0;
    }
    else
    {
        apu_r_v_trl = 1.0;
        apu_l_v_trl = channel_trl;
    }

    double channel_dmc = ((double)apu_channel_pan_dmc / 100.0);

    if (channel_dmc > 1.0)
    {
        // Adjusting right channel
        apu_l_v_dmc = 1.0;
        apu_r_v_dmc = 2.0 - channel_dmc;
    }
    else if (channel_dmc == 1.0)
    {
        apu_r_v_dmc = 1.0;
        apu_l_v_dmc = 1.0;
    }
    else
    {
        apu_r_v_dmc = 1.0;
        apu_l_v_dmc = channel_dmc;
    }
}
void APUInitialize()
{
    audio_samples=(unsigned char **) malloc(2048* sizeof(unsigned char *));
    for (int i = 0; i < 2048; i ++)
    {
        audio_samples[i] =(unsigned char *) malloc(2 * sizeof(unsigned char));
    }

    for (int i = 0; i < 0x20; i++)
    {
        apu_reg_update_func[i] = APUBlankAccess;
    }
    apu_reg_update_func[0x00] = APUOnRegister4000;
    apu_reg_update_func[0x01] = APUOnRegister4001;
    apu_reg_update_func[0x02] = APUOnRegister4002;
    apu_reg_update_func[0x03] = APUOnRegister4003;

    apu_reg_update_func[0x04] = APUOnRegister4004;
    apu_reg_update_func[0x05] = APUOnRegister4005;
    apu_reg_update_func[0x06] = APUOnRegister4006;
    apu_reg_update_func[0x07] = APUOnRegister4007;

    apu_reg_update_func[0x08] = APUOnRegister4008;
    apu_reg_update_func[0x0A] = APUOnRegister400A;
    apu_reg_update_func[0x0B] = APUOnRegister400B;

    apu_reg_update_func[0x0C] = APUOnRegister400C;
    apu_reg_update_func[0x0E] = APUOnRegister400E;
    apu_reg_update_func[0x0F] = APUOnRegister400F;

    apu_reg_update_func[0x10] = APUOnRegister4010;
    apu_reg_update_func[0x11] = APUOnRegister4011;
    apu_reg_update_func[0x12] = APUOnRegister4012;
    apu_reg_update_func[0x13] = APUOnRegister4013;

    apu_reg_update_func[0x15] = APUOnRegister4015;
    apu_reg_update_func[0x16] = APUOnRegister4016;
    apu_reg_update_func[0x17] = APUOnRegister4017;

    audio_playback_peek_limit = 124;
    audio_playback_amplitude = 128;

    for (int n = 0; n < 32; n++)
        pulse_table[n] = 95.52 / (8128.0 / n + 100);

    for (int n = 0; n < 204; n++)
        tnd_table[n] = 163.67 / (24329.0 / n + 100);

    for (int n = 0; n < 16; n++)
        APUGetPrec(n, 0xF, 0.14, &mixer_small_table[n]);// Normalize all channels except dmc output from 0-15 into 0 - 14 % of the mix

    for (int n = 0; n < 128; n++)
        APUGetPrec(n, 0x7F, 0.44, &mixer_big_table[n]);// Normalize DMC-PCM output from 0-127 into 0 - 44 % of the mix

    if (IsStereoNes)
        apu_upfate_playback=APUUpdatePlaybackStereoNes;
    else
        apu_upfate_playback=APUUpdatePlaybackNormal;

    APUApplyChannelPans();

}
void APUSetupMapper(void (*apu_get_sample)(double *output_sample),void (*on_board_clock_length)(),void (*on_board_clock_envelope)(),void (*on_board_clock)(),void (*on_board_clock_single)())
{
    apu_board_sound_get_sample=apu_get_sample;
    apu_board_clock_length=on_board_clock_length;
    apu_board_clock_envelope=on_board_clock_envelope;
    apu_board_clock=on_board_clock;
    apu_board_clock_single=on_board_clock_single;
}
void APUHardReset(int tv_format, int audio_frequency)
{
    apu_reg_io_db = 0;
    apu_reg_io_addr = 0;
    apu_reg_access_happened = 0;
    apu_reg_access_w = 0;
    apu_seq_mode = 0;
    apu_odd_cycle = 1;
    apu_cycle_f_t = 0;
    apu_cycle_e = 4;
    apu_cycle_f = 4;
    apu_cycle_l = 4;
    apu_odd_l = 0;
    apu_check_irq = 0;
    apu_do_env = 0;
    apu_do_length = 0;
    switch (tv_format)
    {
    case TVFORMAT_NTSC:
    {
        cpu_clock_per_frame = 29780.5;// 29780.5 ?
        apu_ferq_f = 14914;
        apu_ferq_e = 3728;
        apu_ferq_l = 7456;
        break;
    }
    // TODO: define APU clock frequencies at PAL/DENDY
    case TVFORMAT_PALB:
    {
        cpu_clock_per_frame = 33247.5;// 33247.5 ?
        apu_ferq_f = 14914;
        apu_ferq_e = 3728;
        apu_ferq_l = 7456;
        break;
    }
    case TVFORMAT_DENDY:
    {
        cpu_clock_per_frame = 35464;
        apu_ferq_f = 14914;
        apu_ferq_e = 3728;
        apu_ferq_l = 7456;
        break;
    }
    }

    // Setup audio output stuff

    // Calculate ratio 2
    double r1 = (double)audio_frequency / NES_TargetFPS;
    // Ratio = cpu cycles per frames / target samples each frame
    audio_bp_ratio1 = cpu_clock_per_frame / r1;

    audio_bp_ratio1_timer = 0;

    audio_samples_count=2024;

    audio_w_pos = 0;
    audio_samples_added = 0;
    audio_timer = 0;
    audio_x_r = audio_y_r = 0;
    audio_x_l = audio_y_l = 0;

    SC_Square1HardReset();
    SC_Square2HardReset();
    NOSHardReset(tv_format);
    TRLHardReset();
    DMCHardReset(tv_format);

    apu_irq_enabled = 1;
    apu_irq_flag = 0;
}
void APUSoftReset()
{
    apu_reg_io_db = 0;
    apu_reg_io_addr = 0;
    apu_reg_access_happened = 0;
    apu_reg_access_w = 0;
    apu_seq_mode = 0;
    apu_odd_cycle = 0;
    apu_cycle_f_t = 0;
    apu_cycle_e = 4;
    apu_cycle_f = 4;
    apu_cycle_l = 4;
    apu_odd_l = 0;
    apu_check_irq = 0;
    apu_do_env = 0;
    apu_do_length = 0;
    apu_irq_enabled = 1;
    apu_irq_flag = 0;

    SC_Square1SoftReset();
    SC_Square2SoftReset();
    NOSSoftReset();
    TRLSoftReset();
    DMCSoftReset();
}

void APUClock()
{
    apu_odd_cycle = !apu_odd_cycle;

    if (apu_do_env)
        APUClockEnvelope();

    if (apu_do_length)
        APUClockDuration();

    if (apu_odd_cycle)
    {
        // IRQ
        apu_cycle_f++;
        if (apu_cycle_f >= apu_ferq_f)
        {
            apu_cycle_f = -1;
            apu_check_irq = 1;
            apu_cycle_f_t = 3;
        }
        // Envelope
        apu_cycle_e++;
        if (apu_cycle_e >= apu_ferq_e)
        {
            apu_cycle_e = -1;
            // Clock envelope and other units except when:
            // 1 the seq mode is set
            // 2 it is the time of irq check clock
            if (apu_check_irq)
            {
                if (!apu_seq_mode)
                {
                    // this is the 3rd step of mode 0, do a reset
                    apu_do_env = 1;
                }
                else
                {
                    // the next step will be the 4th step of mode 1
                    // so, shorten the step then do a reset
                    apu_cycle_e = 4;
                }
            }
            else
                apu_do_env = 1;
        }
        // Length
        apu_cycle_l++;
        if (apu_cycle_l >= apu_ferq_l)
        {
            apu_odd_l = !apu_odd_l;

            apu_cycle_l = apu_odd_l ? -2 : -1;

            // Clock duration and sweep except when:
            // 1 the seq mode is set
            // 2 it is the time of irq check clock
            if (apu_check_irq && apu_seq_mode)
            {
                apu_cycle_l = 3730;// Next step will be after 7456 - 3730 = 3726 cycles, 2 cycles shorter than e freq
                apu_odd_l = 1;
            }
            else
            {
                apu_do_length = 1;
            }
        }

        SC_Square1Clock();
        SC_Square2Clock();
        NOSClock();

        if (IsExternalSoundChannelsEnabled)
            apu_board_clock();

        if (apu_reg_access_happened)
        {
            apu_reg_access_happened = 0;
            apu_reg_update_func[apu_reg_io_addr]();
        }

    }

    // Clock internal components
    TRLClock();
    DMCClock();

    if (apu_check_irq)
    {
        if (!apu_seq_mode)
            APUCheckIRQ();
        // This is stupid ... :(
        apu_cycle_f_t--;
        if (apu_cycle_f_t == 0)
            apu_check_irq = 0;
    }
    if (IsExternalSoundChannelsEnabled)
        apu_board_clock_single();

    apu_upfate_playback();
}

void APUCPURead(unsigned short *address, unsigned char *value)
{
    apu_reg_io_addr = (*address) & 0x1F;
    apu_reg_access_happened = 1;
    apu_reg_access_w = 0;
    if ((*address) == 0x4015)
    {
        APURead4015();
    }
    else if ((*address) == 0x4016)
    {
        APURead4016();
    }
    else if ((*address) == 0x4017)
    {
        APURead4017();
    }
    (*value) = apu_reg_io_db;
}
void APUCPUWrite(unsigned short *address, unsigned char *value)
{
    apu_reg_io_addr = (*address) & 0x1F;
    apu_reg_io_db = (*value);
    apu_reg_access_w = 1;
    apu_reg_access_happened = 1;
    if ((*address) == 0x4014)
    {
        AssertOAMDMA(value);
    }
    else if ((*address) == 0x4015)
    {
        DMCWrite4015(address,value);
    }
}

void APUBlankAccess()
{

}

void APUOnRegister4015()
{
    if (apu_reg_access_w)
    {
        // do a normal write
        SC_Square1OnRegister4015();
        SC_Square2OnRegister4015();
        NOSOn4015();
        TRLOn4015();
        DMCOn4015();
    }
    else
    {
        // on reads, do the effects we know
        apu_irq_flag = 0;
        NES_IRQFlags &= IRQ_FLAG_APU_OFF;
    }
}
void APUOnRegister4016()
{
    // Only writes accepted
    if (apu_reg_access_w)
    {
        if (inputStrobe > (apu_reg_io_db & 0x01))
        {
            // TODO: setup ports
            if (IsFourPlayersConnected)
            {
                PORT0 = PLAYER3_DATA << 8 | PLAYER1_DATA | 0x01010000;
                PORT1 = PLAYER4_DATA << 8 | PLAYER2_DATA | 0x02020000;
            }
            else
            {
                PORT0 = PLAYER1_DATA | 0x01010100;
                PORT1 = PLAYER2_DATA | 0x02020200;
            }
        }
        inputStrobe = apu_reg_io_db & 0x01;
    }
}
void APUOnRegister4017()
{
    if (apu_reg_access_w)
    {
        apu_seq_mode = (apu_reg_io_db & 0x80) != 0;
        apu_irq_enabled = (apu_reg_io_db & 0x40) == 0;

        // Reset counters
        apu_cycle_e = -1;
        apu_cycle_l = -1;
        apu_cycle_f = -1;
        apu_odd_l = 0;
        // Clock immediately ?
        apu_do_length = apu_seq_mode;
        apu_do_env = apu_seq_mode;
        // Reset irq
        apu_check_irq = 0;

        if (!apu_irq_enabled)
        {
            apu_irq_flag = 0;
            NES_IRQFlags &= IRQ_FLAG_APU_OFF;
        }
    }
}

void APURead4015()
{
    apu_reg_io_db = apu_reg_io_db & 0x20;
    // Channels enable
    SC_Square1ReadRegister4015();
    SC_Square2ReadRegister4015();
    NOSRead4015();
    TRLRead4015();
    DMCRead4015();
    // IRQ
    if (apu_irq_flag)
        apu_reg_io_db = (apu_reg_io_db & 0xBF) | 0x40;
    if (apu_irq_delta_occur)
        apu_reg_io_db = (apu_reg_io_db & 0x7F) | 0x80;
}
void APURead4016()
{
    apu_reg_io_db = (PORT0 & 1);
    PORT0 >>= 1;
}
void APURead4017()
{
    apu_reg_io_db = (PORT1 & 1);
    PORT1 >>= 1;
}

void APUClockDuration()
{
    SC_Square1ClockLength();
    SC_Square2ClockLength();
    TRLClockLength();
    NOSClockLength();

    if (IsExternalSoundChannelsEnabled)
        apu_board_clock_length();

    apu_do_length = 0;
}
void APUClockEnvelope()
{
    SC_Square1ClockEnvelope();
    SC_Square2ClockEnvelope();
    NOSClockEnvelope();
    TRLClockEnvelope();

    if (IsExternalSoundChannelsEnabled)
        apu_board_clock_envelope();

    apu_do_env = 0;
}
void APUCheckIRQ()
{
    if (apu_irq_enabled)
        apu_irq_flag = 1;
    if (apu_irq_flag)
        NES_IRQFlags |= IRQ_FLAG_APU;
}

void APUUpdatePlaybackNormal()
{

    // TODO: audio downsampling
    audio_bp_ratio1_timer++;

    // Clock on playback frequency (i.e. 44100 Hz)
    if (audio_bp_ratio1_timer >= audio_bp_ratio1)
    {
        audio_bp_ratio1_timer -= audio_bp_ratio1;

        if (UseAPULookUpTable)
        {
            // use look-up table as described here: <https://wiki.nesdev.com/w/index.php?title=APU_Mixer>
            audio_x_r = pulse_table[sq1_output + sq2_output] + tnd_table[(3 * trl_output) + (2 * nos_output) + dmc_output];
        }
        else
        {
            // Use built-in mixer.
            // Use pre-caluculated arrays to get better performance, this line does as same as below.
            audio_x_r = (mixer_small_table[sq1_output] + mixer_small_table[sq2_output] + mixer_small_table[trl_output] + mixer_small_table[nos_output] + mixer_big_table[dmc_output]);// 0 - 1.0 ( 0 - 100 %)


            /*APUGetPrec(sq1_output, 0xF, 0.14, &channels_out[0]);// Normalize Square 1 output from 0-15 into 0 - 14 % of the mix
            APUGetPrec(sq2_output, 0xF, 0.14, &channels_out[1]);// Normalize Square 2 output from 0-15 into 0 - 14 % of the mix
            APUGetPrec(trl_output, 0xF, 0.14, &channels_out[2]);// Normalize Triangle output from 0-15 into 0 - 14 % of the mix
            APUGetPrec(nos_output, 0xF, 0.14, &channels_out[3]);// Normalize Noise output from 0-15 into 0 - 14 % of the mix
            APUGetPrec(dmc_output, 0x7F, 0.44, &channels_out[4]);// Normalize DMC-PCM output from 0-127 into 0 - 44 % of the mix

            audio_x = (channels_out[0] + channels_out[1] + channels_out[2] + channels_out[3] + channels_out[4]);// 0 - 1.0 ( 0 - 100 %)*/
        }

        if (IsExternalSoundChannelsEnabled)
        {
            apu_board_sound_get_sample(&audio_board_x);
// should be 0-1.0 (0 - 100 %) since expected mixed value from external sound channels should be 0 to 1.0 as well
            audio_x_r  = (audio_x_r  + audio_board_x) / 2;
        }

        // There is no possible other transform from 0-1 into bits. It works only from range 0-1 into 0-127.
        // Add 1 to make sure there is no sample 0, db fix. See <https://github.com/alaahadid/Docs/blob/main/Audio%20And%20DB.txt>
        audio_x_r  = (audio_x_r * 127) + 1;

        // Write into buffer
        if (audio_w_pos < audio_samples_count)
        {
            audio_samples[audio_w_pos][0] = audio_x_r ;
            audio_samples[audio_w_pos][1] = audio_x_r ;

            audio_w_pos++;
            audio_samples_added++;
        }

    }
}
void APUUpdatePlaybackStereoNes()
{

    // TODO: audio downsampling
    audio_bp_ratio1_timer++;

    // Clock on frequency when apu should output sound, it is  cpu speed / 4, about 447443.25 Hz
    if (audio_bp_ratio1_timer >= audio_bp_ratio1)
    {
        audio_bp_ratio1_timer -= audio_bp_ratio1;

        // Left
        if (UseAPULookUpTable)
        {
            // use look-up table as described here: <https://wiki.nesdev.com/w/index.php?title=APU_Mixer>
            audio_x_l = pulse_table[(int)(sq1_output * apu_l_v_sq1) + (int)(sq2_output*apu_l_v_sq2)] + tnd_table[(3 * (int)(trl_output*apu_l_v_trl)) + (2 * (int)(nos_output*apu_l_v_nos)) + (int)(dmc_output*apu_l_v_dmc)];
            audio_x_r = pulse_table[(int)(sq1_output * apu_r_v_sq1) + (int)(sq2_output*apu_r_v_sq2)] + tnd_table[(3 * (int)(trl_output*apu_r_v_trl)) + (2 * (int)(nos_output*apu_r_v_nos)) + (int)(dmc_output*apu_r_v_dmc)];
        }
        else
        {
            // Use built-in mixer.
            audio_x_l = (mixer_small_table[(int)(sq1_output * apu_l_v_sq1)] + mixer_small_table[(int)(sq2_output*apu_l_v_sq2)] + mixer_small_table[(int)(trl_output*apu_l_v_trl)] + mixer_small_table[(int)(nos_output*apu_l_v_nos)] + mixer_big_table[(int)(dmc_output*apu_l_v_dmc)]);// 0 - 1.0 ( 0 - 100 %)
            audio_x_r = (mixer_small_table[(int)(sq1_output * apu_r_v_sq1)] + mixer_small_table[(int)(sq2_output*apu_r_v_sq2)] + mixer_small_table[(int)(trl_output*apu_r_v_trl)] + mixer_small_table[(int)(nos_output*apu_r_v_nos)] + mixer_big_table[(int)(dmc_output*apu_r_v_dmc)]);// 0 - 1.0 ( 0 - 100 %)
        }

        if (IsExternalSoundChannelsEnabled)
        {
            apu_board_sound_get_sample(&audio_board_x);
// should be 0-1.0 (0 - 100 %) since expected mixed value from external sound channels should be 0 to 1.0 as well
            audio_x_l  = (audio_x_l  + audio_board_x) / 2;
            audio_x_r  = (audio_x_r  + audio_board_x) / 2;
        }

        // There is no possible other transform from 0-1 into bits. It works only from range 0-1 into 0-127.
        // Add 1 to make sure there is no sample 0, db fix. See <https://github.com/alaahadid/Docs/blob/main/Audio%20And%20DB.txt>
        audio_x_l  = (audio_x_l*127)+1;
        audio_x_r  = (audio_x_r*127)+1;

        // Write into buffer
        if (audio_w_pos < audio_samples_count)
        {
            audio_samples[audio_w_pos][0] = audio_x_r;
            audio_samples[audio_w_pos][1] = audio_x_l;

            audio_w_pos++;
            audio_samples_added++;
        }

    }
}
