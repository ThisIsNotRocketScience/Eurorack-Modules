#pragma once


#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "EurorackShared/EurorackShared.h"

class Wobbler2Drum
{

public:

	int Phase;
	int Shape;
	int Freq;
	int Mod;
	
	int M1;
	int M2;
	Wobbler2Drum()
	{
		AmpEnv = 0;
		DAmpEnv = 0;
	}
	int32_t AmpEnv;
	uint32_t DAmpEnv;
	uint32_t OscPhase;
	void Trigger(bool ON = true)
	{ 
		if (ON)
		{
			OscPhase = 0;
			AmpEnv = 0xffffff;
			DAmpEnv = 0x1111;
			Freq = 0x001000;
			Mod = 0xffff;
		}
	}



	void Get(int32_t &L, int32_t &R)
	{		
		AmpEnv -= DAmpEnv;
		if (AmpEnv < 0)
		{
			DAmpEnv = 0;
			AmpEnv = 0;
		}
		int32_t DR[7] = { 0 };
		OscPhase += Freq + AmpEnv * Mod;
		float P = (float)(OscPhase * 6.283f) / (float)(1 << 21);
		DR[0] =sinf(P)*32768.0f; // bassdrum
		DR[1] = 0; // tom
		DR[2] = 0; // block
		DR[3] = 0; // ploink
		DR[4] = 0; // snare
		DR[5] = 0; // hat
		DR[6] = 0; // clap

		L = ((AmpEnv>>8) * DR[0])>>16;
		R = ((AmpEnv >> 8) * DR[0]) >> 16;
	}
};