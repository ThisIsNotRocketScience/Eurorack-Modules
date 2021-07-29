#pragma once


#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "EurorackShared/EurorackShared.h"
#include "Wobbler2Code/Wobbler2.h"


class Wobbler2Drum
{

public:

	int32_t sintab[2048];
	int Phase;
	int Shape;
	int Freq;
	int Mod;
	
	int M1;
	int M2;
#define TABBITS 11
#define TABSHIFT (32-TABBITS)
#define WOBTABLEN (1 << TABBITS)
#define WOBTABMASK (WOBTABLEN-1)

	Wobbler2Drum()
	{
		AmpEnv = 0;
		DAmpEnv = 0;
		for (int i = 0; i < WOBTABLEN; i++)
		{
			sintab[i] = (int)(sinf((i * 6.283f) /(float)WOBTABLEN) * 32767.0f);
		}
	}
	int32_t AmpEnv;
	uint32_t DAmpEnv;
	uint32_t OscPhase;
	void Trigger(bool ON = true)
	{ 
		if (ON)
		{
			OscPhase = 0;
			AmpEnv = 0x7fffffff;
			DAmpEnv = 0x011111;
			Freq = 0xffffffff / 441;
			Mod = 0x1ff;
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
		OscPhase += Freq + ((AmpEnv * Mod)>>16);
		
		DR[0] = sintab[OscPhase >> TABSHIFT];
		DR[1] = sintab[(OscPhase >> (TABSHIFT-1))&WOBTABMASK];
		DR[2] = 0; // block
		DR[3] = 0; // ploink
		DR[4] = 0; // snare
		DR[5] = 0; // hat
		DR[6] = 0; // clap


		SteppedResult_t ShapeStepped;
		Wobbler2_GetSteppedResult(Shape, 6, &ShapeStepped);


		// interpolate between al the shapes

		uint32_t O1 = WobGetInterpolatedResultInt(DR, &ShapeStepped);// / (0xffff * 4);
		L = ((AmpEnv >> 16) * O1) >> 16;
		R = ((AmpEnv >> 16) * O1) >> 16;
	}
};
