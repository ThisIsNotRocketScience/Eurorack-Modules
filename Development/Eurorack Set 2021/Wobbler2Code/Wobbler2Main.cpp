#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/adc.h"
#include "../EurorackShared/Math.c"
#include "Wobbler2Drum.h"
#define DAC_SCK 0
#define DAC_CS 1
#define DAC_DAT 2

#define FASTMODE_BUT 5
#define SR_2_DAT 9
#define SR_2_CLK 10
#define SR_2_LATCH 11

#define SR_1_DAT 12
#define SR_1_CLK 13
#define SR_1_LATCH 14

#define RESET_PATTERN_BUT 20
#define RESET_PATTERN 21
#define SYNC_LFO 22
#define MUX_A 24
#define MUX_B 25

#define ADC_MUX0 ADC0
#define ADC_MUX1 ADC1
#define ADC_FREQ ADC2
#define ADC_SHAPE ADC3

uint16_t COMBINE(int32_t A, int32_t B)
{
    int32_t R = A + B - 32767;
    if (R > 0xffff)
        R = 0xffff;
    else if (R < 0)
        R = 0;
    return (uint16_t)R;
}

#define KNOB_MOD muxed[0][0]
#define KNOB_FREQ muxed[0][1]
#define KNOB_SHAPE muxed[0][2]
#define KNOB_PHASE muxed[0][3]

#define ADC_PHASE muxed[1][0]
#define ADC_MOD muxed[1][1]
#define KNOB_PHASED muxed[1][2]
#define KNOB_NORMAL muxed[1][3]
#include "Wobbler2.h"

Wobbler2_LFO_t TheWobbler;
Wobbler2_Params TheParams;
Wobbler2Drum TheDrums;
const uint32_t Cmapping[] = {
    0x03F0D370, 0x04113BF3, 0x0432AF01, 0x04553521, 0x0478D733, 0x049D9E4A, 0x04C393DB, 0x04EAC196, 0x05133184, 0x053CEE01, 0x056801B9, 0x059477BD, 0x05C25B5F, 0x05F1B862, 0x06229AEB, 0x06550F7A,
    0x068922FE, 0x06BEE2C9, 0x06F65C99, 0x072F9E9E, 0x076AB787, 0x07A7B67C, 0x07E6AB09, 0x0827A55A, 0x086AB609, 0x08AFEE3C, 0x08F75FB8, 0x09411CBC, 0x098D3826, 0x09DBC57E, 0x0A2CD8D7, 0x0A8086EF,
    0x0AD6E539, 0x0B3009CF, 0x0B8C0B83, 0x0BEB01D8, 0x0C4D052E, 0x0CB22E85, 0x0D1A97CD, 0x0D865BD1, 0x0DF595FE, 0x0E6862FF, 0x0EDEE00D, 0x0F592B8A, 0x0FD764CC, 0x1059AC2A, 0x10E022D7, 0x116AEB71,
    0x11FA2945, 0x128E0135, 0x132698FD, 0x13C417AB, 0x1466A591, 0x150E6C51, 0x15BB96BC, 0x166E5159, 0x1726C9F1, 0x17E52F8A, 0x18A9B2E6, 0x19748653, 0x1A45DDDA, 0x1B1DEEE9, 0x1BFCF0FA, 0x1CE31D14,
    0x1DD0AE1F, 0x1EC5E0BA, 0x1FC2F404, 0x20C8285F, 0x21D5C0FA, 0x22EC02BB, 0x240B34E4, 0x2533A0E4, 0x266592AA, 0x27A1587E, 0x28E74328, 0x2A37A668, 0x2B92D7DC, 0x2CF93096, 0x2E6B0C74, 0x2FE8C9D7,
    0x3172CA8D, 0x330973D6, 0x34AD2D72, 0x365E6291, 0x381D82C2, 0x39EAFFC4, 0x3BC75008, 0x3DB2EDBC, 0x3FAE5631, 0x41BA0BB5, 0x43D69409, 0x4604799B, 0x48444B8B, 0x4A969D09, 0x4CFC05F3, 0x4F75237A,
    0x5202981D, 0x54A50A1A, 0x575D26E2, 0x5A2BA094, 0x5D112EEE, 0x600E8FF1, 0x6324873B, 0x6653DF4B, 0x699D68E2, 0x6D01FA5D, 0x7082737D, 0x741FB99F, 0x77DAB864, 0x7BB4656B, 0x7FADBC91, 0x83C7C1D7,
    0x8803829B, 0x8C62145D, 0x90E4955A, 0x958C2C93, 0x9A5A0B05, 0x9F4F6C4F, 0xA46D94D1, 0xA9B5D389, 0xAF298355, 0xB4CA09B5, 0xBA98D626, 0xC09765E7, 0xC6C742B7, 0xCD2A0055, 0xD3C142BD, 0xDA8EB7F0};

enum
{
    LO_A_1,
    LO_A_2,
    LO_A_3,
    LO_A_4,
    LO_A_5,
    LO_A_6,
    LO_A_7,
    LO_A_8,
    LO_NC0,
    LO_NC1,
    LO_NC2,
    LO_LFOCYCLESTART,
    LO_A_0,
    LO_B_0,
    LO_TRIGGERBUTTONLED,
    LO_LFOCYCLEMID,
    LO_B_1,
    LO_B_2,
    LO_B_3,
    LO_B_4,
    LO_B_5,
    LO_B_6,
    LO_B_7,
    LO_B_8,
    LO_NC3,
    LO_OPT4,
    LO_OPT0,
    LO_OPT1,
    LO_OPT3,
    LO_OPT2,
    LO_NC4,
    LO_NC5,

    LO_NC6,

    LO_OPT3b,
    LO_OPT4b,
    LO_OPT2b,
    LO_OPT1b,
    LO_OPT0b,
    LO_FASTMODELED,
    LO_NC7
};
typedef struct debounce_t
{
    int timeon;
    int timeoff;
} debounce_t;

enum
{
    UNCERTAIN,
    PRESSED,
    RELEASED,
    DOWN,
    UP
};

extern int debounce(int gpio, debounce_t *b);
#define DEBOUNCETIME 10
int debounce(int gpio, debounce_t *b)
{
    if (gpio_get(gpio) == 0)
    {
        if (b->timeoff > 0)
        {
            b->timeoff = 0;
        }
        if (b->timeon < DEBOUNCETIME)
        {
            b->timeon++;
            if (b->timeon == DEBOUNCETIME)
                return PRESSED;
        }
        else
        {
            return DOWN;
        }

        return UNCERTAIN;
    }
    else
    {
        if (b->timeon > 0)
        {
            b->timeon = 0;
        }
        if (b->timeoff < DEBOUNCETIME)
        {
            b->timeoff++;
            if (b->timeoff == DEBOUNCETIME)
                return RELEASED;
        }
        else
        {
            return UP;
        }
        return UNCERTAIN;
    }
};

debounce_t Trigger_b;
debounce_t Trigger_Jack;
debounce_t FastMode_b;

#define DAC_DATA_SET gpio_put(DAC_DAT, 1)
#define DAC_DATA_CLR gpio_put(DAC_DAT, 0)
#define DAC_CLK_SET gpio_put(DAC_SCK, 1)
#define DAC_CLK_CLR gpio_put(DAC_SCK, 0)
#define DAC_CLK_SET gpio_put(DAC_SCK, 1)
#define DAC_CLK_CLR gpio_put(DAC_SCK, 0)

void DAC_Shift(uint8_t b)
{
    if ((b & 0x80) == 0x80)
        DAC_DATA_SET;
    else
        DAC_DATA_CLR;
    DAC_CLK_SET;
    DAC_CLK_CLR;
    if ((b & 0x40) == 0x40)
        DAC_DATA_SET;
    else
        DAC_DATA_CLR;
    DAC_CLK_SET;
    DAC_CLK_CLR;
    if ((b & 0x20) == 0x20)
        DAC_DATA_SET;
    else
        DAC_DATA_CLR;
    DAC_CLK_SET;
    DAC_CLK_CLR;
    if ((b & 0x10) == 0x10)
        DAC_DATA_SET;
    else
        DAC_DATA_CLR;
    DAC_CLK_SET;
    DAC_CLK_CLR;
    if ((b & 0x08) == 0x08)
        DAC_DATA_SET;
    else
        DAC_DATA_CLR;
    DAC_CLK_SET;
    DAC_CLK_CLR;
    if ((b & 0x04) == 0x04)
        DAC_DATA_SET;
    else
        DAC_DATA_CLR;
    DAC_CLK_SET;
    DAC_CLK_CLR;
    if ((b & 0x02) == 0x02)
        DAC_DATA_SET;
    else
        DAC_DATA_CLR;
    DAC_CLK_SET;
    DAC_CLK_CLR;
    if ((b & 0x01) == 0x01)
        DAC_DATA_SET;
    else
        DAC_DATA_CLR;
    DAC_CLK_SET;
    DAC_CLK_CLR;
}

int DACSENDDONE = 0;

void DACB_Write(int channel, int value)
{
    gpio_put(DAC_CS, 0);
    const int shutdown1 = 0;
    const int gain1 = 0;
    const int shutdown2 = 0;
    const int gain2 = 0;
    unsigned char coms[2];

    unsigned int command;
    if (channel == 1)
    {
        command = 0x0000;
        command |= shutdown1 ? 0x0000 : 0x1000;
        command |= gain1 ? 0x0000 : 0x2000;
        command |= (value & 0x0FFF);
        coms[0] = command >> 8;
        coms[1] = command & 0xff;
        DACSENDDONE = 0;
    }
    else
    {
        command = 0x8000;
        command |= shutdown2 ? 0x0000 : 0x1000;
        command |= gain2 ? 0x0000 : 0x2000;
        command |= (value & 0x0FFF);
        coms[0] = command >> 8;
        coms[1] = command & 0xff;
        DACSENDDONE = 0;
    }
    DAC_Shift(coms[0]);
    DAC_Shift(coms[1]);
    gpio_put(DAC_CS, 1);
}

// half-millisecond timer -> update each dacchannel in turn
int LinearOut = 0;
int CurvedOut = 0;
int timer_count = 0;
int tickssincecommit = 0;
int t = 0;
int LEDS[5 * 8] = {0};

void ShiftBytes(int clk, int dat, int latch, int *b, int n, int cmp = 0)
{
    for (int i = n - 1; i >= 0; i--)
    {
        gpio_put(dat, b[i] > cmp ? 1 : 0);
        gpio_put(clk, 1);
        gpio_put(clk, 0);
    }
    gpio_put(latch, 1);
    gpio_put(latch, 0);
}

uint8_t pwm = 0;
void ShiftOut()
{
    pwm += 11;
    ShiftBytes(SR_1_CLK, SR_1_DAT, SR_1_LATCH, LEDS, 24, pwm);
    ShiftBytes(SR_2_CLK, SR_2_DAT, SR_2_LATCH, LEDS + 24, 16, pwm);
}
int runner = 0;
void doTimer()
{

    int TriggerButton = debounce(RESET_PATTERN_BUT, &Trigger_b);
    int TriggerJack = debounce(RESET_PATTERN, &Trigger_Jack);
    int FastModeButton = debounce(FASTMODE_BUT, &FastMode_b);
    if (TheWobbler.fastmode)
    {
        runner = runner + 1;

        if (TriggerButton == PRESSED || TriggerJack == PRESSED)
        {
            TheDrums.Trigger(true);
        }
        if (FastModeButton == PRESSED)
        {
            Wobbler2_SwitchMode(&TheWobbler);
        }
    }
    else
    {
        runner = runner + 100;
        if (TriggerButton == PRESSED)
        {

            Wobbler2_Trigger(&TheWobbler, 0, &TheParams);
        }
        if (FastModeButton == PRESSED)
        {
            Wobbler2_SwitchMode(&TheWobbler);
        }
    }
    timer_count++;

    if (runner >= 100)
    {
        runner = 0;
        for (int i = 0; i < 40; i++)
            LEDS[i] = 0;
        LEDS[LO_A_0] = TheWobbler.Led[0][0];
        LEDS[LO_A_1] = TheWobbler.Led[0][1];
        LEDS[LO_A_2] = TheWobbler.Led[0][2];
        LEDS[LO_A_3] = TheWobbler.Led[0][3];
        LEDS[LO_A_4] = TheWobbler.Led[0][4];
        LEDS[LO_A_5] = TheWobbler.Led[0][5];
        LEDS[LO_A_6] = TheWobbler.Led[0][6];
        LEDS[LO_A_7] = TheWobbler.Led[0][7];
        LEDS[LO_A_8] = TheWobbler.Led[0][8];

        LEDS[LO_B_0] = TheWobbler.Led[1][0];
        LEDS[LO_B_1] = TheWobbler.Led[1][1];
        LEDS[LO_B_2] = TheWobbler.Led[1][2];
        LEDS[LO_B_3] = TheWobbler.Led[1][3];
        LEDS[LO_B_4] = TheWobbler.Led[1][4];
        LEDS[LO_B_5] = TheWobbler.Led[1][5];
        LEDS[LO_B_6] = TheWobbler.Led[1][6];
        LEDS[LO_B_7] = TheWobbler.Led[1][7];
        LEDS[LO_B_8] = TheWobbler.Led[1][8];
        LEDS[LO_TRIGGERBUTTONLED] = TheWobbler.TriggerLed;
        LEDS[LO_LFOCYCLEMID] = TheWobbler.Gate[0];
        LEDS[LO_LFOCYCLESTART] = TheWobbler.Gate[1];

        if (TheWobbler.fastmode)
        {
            LEDS[LO_OPT0b] = TheWobbler.ModeLed[0];
            LEDS[LO_OPT1b] = TheWobbler.ModeLed[1];
            LEDS[LO_OPT2b] = TheWobbler.ModeLed[2];
            LEDS[LO_OPT3b] = TheWobbler.ModeLed[3];
            LEDS[LO_OPT4b] = TheWobbler.ModeLed[4];
        }
        else
        {
            LEDS[LO_OPT0] = TheWobbler.ModeLed[0];
            LEDS[LO_OPT1] = TheWobbler.ModeLed[1];
            LEDS[LO_OPT2] = TheWobbler.ModeLed[2];
            LEDS[LO_OPT3] = TheWobbler.ModeLed[3];
            LEDS[LO_OPT4] = TheWobbler.ModeLed[4];
        }
        ShiftOut();
    }

    if (timer_count & 1)
        return;
    tickssincecommit++;
    t++;
    switch (t % 2)
    {
    case 0:
    {
        if (TheWobbler.fastmode)
        {
            int32_t DrumL, DrumR;
            TheDrums.Get(DrumL, DrumR);
            LinearOut = (DrumL / (65536 / 4096)) + 2048;
            CurvedOut = (DrumR / (65536 / 4096)) + 2048;
        }
        else
        {
            LinearOut = Wobbler2_Get(&TheWobbler, &TheParams);
            LinearOut = (int)(TheWobbler.Output);
            CurvedOut = (int)(TheWobbler.OutputPhased);
        }

        // CYCLESTART_PutVal(CYCLESTART_DeviceData, !(TheWobbler.Gate[1]>0));
        // CYCLEMID_PutVal(CYCLEMID_DeviceData,   !(TheWobbler.Gate[0]>0));

        DACB_Write(1, CurvedOut);
    }
    break;
    case 1:
    {
        DACB_Write(0, LinearOut);
    }

    break;
    }
}

int main()
{
    adc_init();
    adc_gpio_init(26);
    int muxed[2][4] = {0};
    Wobbler2_Init(&TheWobbler);
    int curmux = 0;

    gpio_init(RESET_PATTERN_BUT);
    gpio_dir(RESET_PATTERN_BUT, GPIO_IN);
    gpio_pull_up(RESET_PATTERN_BUT);

    gpio_init(RESET_PATTERN);
    gpio_dir(RESET_PATTERN, GPIO_IN);
    gpio_pull_up(RESET_PATTERN);

    gpio_init(FASTMODE_BUT);
    gpio_dir(FASTMODE_BUT, GPIO_IN);
    gpio_pull_up(FASTMODE_BUT);
    gpio_init(MUX_A);
    gpio_dir(MUX_A, GPIO_OUT);
    gpio_init(MUX_B);
    gpio_dir(MUX_B, GPIO_OUT);

    gpio_init(DAC_CS);
    gpio_dir(DAC_CS, GPIO_OUT);
    gpio_init(DAC_SCK);
    gpio_dir(DAC_SCK, GPIO_OUT);
    gpio_init(DAC_DAT);
    gpio_dir(DAC_DAT, GPIO_OUT);

    gpio_init(SR_1_CLK);
    gpio_dir(SR_1_CLK, GPIO_OUT);
    gpio_init(SR_1_DAT);
    gpio_dir(SR_1_DAT, GPIO_OUT);
    gpio_init(SR_1_LATCH);
    gpio_dir(SR_1_LATCH, GPIO_OUT);

    gpio_init(SR_2_CLK);
    gpio_dir(SR_2_CLK, GPIO_OUT);
    gpio_init(SR_2_DAT);
    gpio_dir(SR_2_DAT, GPIO_OUT);
    gpio_init(SR_2_LATCH);
    gpio_dir(SR_2_LATCH, GPIO_OUT);
    int i = 0;
    while (true)
    {

        curmux = (curmux + 1) % 4;
        if ((curmux & 1) == 1)
            gpio_put(MUX_A, 1);
        else
            gpio_put(MUX_A, 0);
        if ((curmux & 2) == 2)
            gpio_put(MUX_B, 1);
        else
            gpio_put(MUX_B, 0);
        adc_input_select(0);
        muxed[0][curmux] = adc_read() * 16;
        adc_input_select(1);
        muxed[1][curmux] = adc_read() * 16;
        adc_input_select(2);
        int A2 = adc_read() * 16;
        adc_input_select(3);
        int A3 = adc_read() * 16;

        doTimer();
        if (TheWobbler.fastmode)
        {
            //        sleep_us(500);
            TheDrums.Mod = KNOB_MOD;
            TheDrums.Shape = KNOB_SHAPE;

            TheDrums.Freq = COMBINE(A2, KNOB_FREQ);
            TheDrums.Phase = COMBINE(A3, KNOB_PHASE);
        }
        else
        {
            sleep_us(500);

            //   PrescaleADC(adcchannels,convadcchannels,ADC_Count);
            TheWobbler.Mod = KNOB_MOD;
            TheWobbler.Shape = KNOB_SHAPE;
            uint16_t phasepre = KNOB_PHASE;
            TheWobbler.Phasing = phasepre >> 4;

            TheWobbler.Speed = ((KNOB_FREQ) >> 7);
            TheWobbler.SpeedOrig = ((KNOB_FREQ));

            TheWobbler.Amount1 = KNOB_NORMAL;
            TheWobbler.Amount2 = KNOB_PHASED;
        }
    }
    return 0;
}





// For optimal use of DMA bandwidth we would use an autopull threshold of 32,
// but we are using a threshold of 8 here (consume 1 byte from each FIFO entry
// and discard the remainder) to make things easier for software on the other side

static inline void spi_on_pio_init(PIO pio, uint sm, uint offset, uint data_pin, uint clk_pin, float clk_div) {
    pio_gpio_init(pio, data_pin);
    pio_gpio_init(pio, clk_pin);
    pio_sm_set_consecutive_pindirs(pio, sm, data_pin, 1, true);
    pio_sm_set_consecutive_pindirs(pio, sm, clk_pin, 1, true);
    pio_sm_config c = st7789_lcd_program_get_default_config(offset);
    sm_config_set_sideset_pins(&c, clk_pin);
    sm_config_set_out_pins(&c, data_pin, 1);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
    sm_config_set_clkdiv(&c, clk_div);
    sm_config_set_out_shift(&c, false, true, 8);
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

// Making use of the narrow store replication behaviour on RP2040 to get the
// data left-justified (as we are using shift-to-left to get MSB-first serial)

static inline void spi_on_pio_put(PIO pio, uint sm, uint8_t x) {
    while (pio_sm_is_tx_fifo_full(pio, sm))
        ;
    *(volatile uint8_t*)&pio->txf[sm] = x;
}

// SM is done when it stalls on an empty FIFO

static inline void spi_on_pio_wait_idle(PIO pio, uint sm) {
    uint32_t sm_stall_mask = 1u << (sm + PIO_FDEBUG_TXSTALL_LSB);
    pio->fdebug = sm_stall_mask;
    while (!(pio->fdebug & sm_stall_mask))
        ;
}
