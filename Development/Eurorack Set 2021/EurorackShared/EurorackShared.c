#include "EurorackShared.h"
#ifndef BIGFISH
#ifndef WIN32
#ifndef SKIPDAC
#include "DAC.c"
#endif
#include "Eeprom.c"
#include "LEDS.c"
#endif
#endif

struct EuroRack_Calibration MasterCalibration;

#define DAC_VOLT_UNCALIBRATED(x) ((int32_t)((409600 * ((int32_t)x)) / (int32_t)(512)))
int  ValidateDAC(struct EuroRack_DAC_Calibration *dac);

uint16_t  CalibratedDAC(int dacchannel, int32_t Input)
{
	//return Input;
	ValidateDAC(&MasterCalibration.DAC[dacchannel]);

	int32_t raw_1 = DAC_VOLT_UNCALIBRATED(1);
	int32_t raw_3 = DAC_VOLT_UNCALIBRATED(3);
	int32_t cal_1 = MasterCalibration.DAC[dacchannel].volt_1;
	int32_t cal_3 = MasterCalibration.DAC[dacchannel].volt_3;

	int32_t raw_D = raw_3 - raw_1;
	int32_t cal_D = cal_3 - cal_1;

	int32_t r1 = Input - raw_1;
	int32_t precomp = (r1  * cal_D ) / (raw_D) ;
	int32_t postcomp = precomp  + cal_1;
	if (postcomp < 0 ) postcomp  = 0;
	return postcomp;
	//return ((Input + MasterCalibration.DAC[dacchannel].offset) *MasterCalibration.DAC[dacchannel].scale) >> 16;
}

void ChangeDACCalibration(int dacchannel, int low, int high)
{
	MasterCalibration.DAC[dacchannel].volt_1 = DAC_VOLT_UNCALIBRATED(1) + ((low - 128)/4);
	MasterCalibration.DAC[dacchannel].volt_3 = DAC_VOLT_UNCALIBRATED(3) + ((high - 128)/4);
}

void InitDAC(struct EuroRack_DAC_Calibration *DAC)
{
	DAC->volt_1 = DAC_VOLT_UNCALIBRATED(1);
	DAC->volt_3 = DAC_VOLT_UNCALIBRATED(3);
}

void InitADC(struct EuroRack_ADC_Calibration *ADC)
{
	ADC->offset = 0;
	ADC->scale = 0x100;
}

void GetSteppedResult(uint16_t param, uint8_t steps, SteppedResult_t *out)
{
	
	uint32_t X = (param * steps) / 256;
	X += 64;

	int FloorX = X & 0xffffff00;
	int Aside = FloorX;
	int Bside = (X - (FloorX)) * 2 + (FloorX)-256;
	int M = (Aside > Bside) ? Aside : Bside;

	out->index = M >> 8;
	out->fractional = M & 0xff;
}

void GetSteppedResult16(uint32_t param, uint8_t steps, SteppedResult16_t *out)
{

	uint32_t X = (param * steps);
	//X += 64<<8;

	//int FloorX = X & 0xffff0000;
	//int Aside = FloorX;
	//int Bside = (X - (FloorX)) * 2 + (FloorX)-256;
	//int M = (Aside > Bside) ? Aside : Bside;
	int32_t frac = X & 0xffff;
	frac -= 0x2000;
	frac *= (0xffff / 0x7fff);
	if (frac < 0) frac = 0; else if (frac > 0xffff) frac = 0xffff;

	out->index = X >> 16;
	out->fractional = frac;
}


void EuroRack_InitCalibration()
{

	for (int i = 0; i < EURORACK_MAX_ADC; i++)
	{
		InitADC(&MasterCalibration.ADC[i]);
	}

	for (int i = 0; i < EURORACK_MAX_DAC; i++)
	{
		InitDAC(&MasterCalibration.DAC[i]);
	}
}

int ValidateADC(struct EuroRack_ADC_Calibration *adc)
{
	return 0;
}

int  ValidateDAC(struct EuroRack_DAC_Calibration *dac)
{
	if (dac->volt_1 >= dac->volt_3)
	{
		InitDAC(dac);
		return 1;
	}
	return 0;
}

int EuroRack_ValidateCalibration()
{
	int invalid = 0;
	for (int i = 0; i < EURORACK_MAX_ADC; i++)
	{
		invalid += ValidateADC(&MasterCalibration.ADC[i]);
	}

	for (int i = 0; i < EURORACK_MAX_DAC; i++)
	{
		invalid += ValidateDAC(&MasterCalibration.DAC[i]);
	}
	return invalid;
}

#define LONGPRESSCYCLES 2000

int islongpress( struct denoise_state_t *state)
{
	if (state->longpressed >= LONGPRESSCYCLES && state->longpressed > -1)
	{
		state->longpressed  = -1;
		return 1;
	}
	return 0;
}

int pressed(struct denoise_state_t *state)
{
	if (state->pressed == 1)
	{
		state->pressed = 0;
		return 1;
	}
	return 0;
}

int released(struct denoise_state_t *state)
{
	if (state->released== 1)
	{
		state->released = 0;
		return 1;
	}
	return 0;
}

void denoise(int sw_down, struct denoise_state_t *state)
{
	if (sw_down)
	{
		state->counter++;
	}
	else
	{
		state->counter--;
	}

//state->pressed = 0;
//state->released = 0;

	if (state->counter < 2)
	{
		if (state->lastcounter == 2)
		{
			state->pressed = 1;
		}
		state->counter = 1;
		state->down = 1;
	}
	else
	{
		if (state->counter > 30)
		{
			if (state->lastcounter == 30)
			{
				state->released = 1;
			}
			state->counter = 31;
			state->down = 0;
		}
	}

	if (state->down > 0)
	{
		if (state->longpressed>-1) state->longpressed++;
	}
	else
	{
		state->longpressed = 0; 
	}

	state->lastcounter = state->counter;
	
	if (state->longpressed >= LONGPRESSCYCLES)
	{
		state->longpressed = LONGPRESSCYCLES;	
	}
}

void InitCounter()
{
	KIN1_InitCycleCounter();
	KIN1_ResetCycleCounter(); 
	KIN1_EnableCycleCounter(); 
}

uint32_t StartCounting()
{
	return KIN1_GetCycleCounter(); 
}

uint32_t EndCount(uint32_t start)
{
	uint32_t Ec = KIN1_GetCycleCounter();
	return Ec - start;
}
