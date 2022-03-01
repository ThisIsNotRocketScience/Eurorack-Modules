#include "Wobbler2.h"
#include <Math.h>
#include "../EurorackShared/EURORACKSHARED.H"
extern "C"
{
#include "../libfixmath/libfixmath/fix16.c"
#include "../libfixmath/libfixmath/fix16_trig.c"
#include "../libfixmath/libfixmath/fix16_sqrt.c"
}

#ifndef __max
#define __max(a,b) ((a)>(b)?(a):(b))
#define __min(a,b) ((a)<(b)?(a):(b))
#endif


#define WOBBLER_SPEEDRATIOCOUNT 11
#define SpeedRatio(x,y) ((x*65536)/y)
int32_t Wobbler_SpeedRatioSet[WOBBLER_SPEEDRATIOCOUNT] = { SpeedRatio(1,8),SpeedRatio(1,6),SpeedRatio(1,4),SpeedRatio(1,3), SpeedRatio(1,2),SpeedRatio(1,1),SpeedRatio(2,1),SpeedRatio(3,1),SpeedRatio(4,1),SpeedRatio(6,1),SpeedRatio(8,1) };


	unsigned long LERP9bit(unsigned long *V, int fade)
	{
		unsigned char frac = (fade << 3) & 0xff;
		//if (frac && (frac < 255)) frac += 1;
		int I = fade >> 5;
		return ((V[I] >> 8) *(255 - frac) + (V[I + 1] >> 8) * frac);
	}

	int32_t imul(int32_t a, int32_t b)
	{
		int64_t R = (int64_t)a * (int64_t)b;
		return (int32_t)(R >> (WOBBLER_FIXBITS));
	}

	int32_t idiv(int32_t a, int32_t b)
	{
		int64_t R = (int64_t)a;
		R <<= WOBBLER_FIXBITS;
		R /= (int64_t)b;
		return (int32_t)R;
	}

	void Wobbler2_RandomSeed(Wobbler2_RandomGen *R, unsigned int seed)
	{
		R->RandomMemory = (long)seed;
	}

	void Wobbler2_SyncPulse(Wobbler2_LFO_t *LFO)
	{
		int Delta = LFO->timesincesync;
		LFO->timesincesync = 0;
		if (Delta < 10) return;

		if (Delta > 30000)
		{
			LFO->syncindex = 0;
		}
		else
		{
			LFO->syncindex++;
		}
		if (LFO->syncindex > 2)
		{
			LFO->synctime[0] = LFO->synctime[1];
			LFO->synctime[1] = LFO->synctime[2];
			LFO->syncindex = 2;

			LFO->SyncedPeriodTime = (LFO->synctime[0] + LFO->synctime[1] + LFO->synctime[2]) / 3;
			LFO->SyncDP = 0xffffffff / LFO->SyncedPeriodTime;
			LFO->extsync = 1;
		}
		LFO->synctime[LFO->syncindex] = Delta;
	}

	int Wobbler2_Rand(Wobbler2_RandomGen *R)
	{
		return (((R->RandomMemory = R->RandomMemory * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	void Wobbler2_Init(Wobbler2_LFO_t *LFO)
	{
		LFO->Output = 0;
		LFO->OutputPhased = 0;
		LFO->fastmode = 0;
		LFO->OutputMin = 0;
		LFO->OutputPhasedMin = 0;

		LFO->OutputMax = 0;
		LFO->OutputPhasedMax = 0;

		LFO->Phase1 = 0;
		LFO->SlomoPhase1 = 0;
		LFO->SNH.segindex = 0;
		LFO->SNH.lastseg1 = 8;
		LFO->SNH.lastseg2 = 8;
		LFO->SNH.triggerhappened = 0;
		LFO->PhasedShift = 0;
		LFO->Gate[0] = 0;
		LFO->Gate[1] = 0;

		//LFO->EnvelopeVal = 0;
		LFO->PhasedCountdown = 0;
		//LFO->EnvelopeState = Wobbler2_IDLE;
		
		Wobbler2_FancyEnv_Init(&LFO->FancyEnv);
		
		LFO->Speed = 10;
		LFO->LastPhasing = 0;
		LFO->syncindex = 0;
		LFO->SNH.F1.high = LFO->SNH.F1.mid = LFO->SNH.F1.low = 0;
		LFO->SNH.F2.high = LFO->SNH.F2.mid = LFO->SNH.F2.low = 0;
		LFO->SNH.F3.high = LFO->SNH.F3.mid = LFO->SNH.F3.low = 0;
		LFO->SNH.F4.high = LFO->SNH.F4.mid = LFO->SNH.F4.low = 0;

#ifdef INTPENDULUM
		Wobbler2_InitIntPendulum(&LFO->Pendulum, LFO);
#else
		Wobbler2_InitPendulum(&LFO->Pendulum, LFO);
#endif
		Wobbler2_RandomSeed(&LFO->SNH.random, 0);
		for (int i = 0; i < 32; i++)
		{
			LFO->SNH.segbuffer[i] = 0;
		}

		LFO->CalibNormal = 0;
		LFO->CalibPhased = 6;
	}

	void Wobbler2_SwitchMode(Wobbler2_LFO_t *LFO)
	{
		if (LFO->fastmode)
		{
			Wobbler2_Init(LFO);
			LFO->fastmode = 0;

		}
		else
		{
			Wobbler2_Init(LFO);
			LFO->fastmode = 1;
		}
	}	

	void Wobbler2_Trigger(Wobbler2_LFO_t *LFO, unsigned char N, struct Wobbler2_Params *Params)
	{
		if (N == 0)
		{
			LFO->TriggerLed = 255;
			LFO->Phase1 = 0;
			LFO->SlomoPhase1 = 0;
			LFO->PhasedShift = 0;
			LFO->SNH.triggerhappened = 1;
			Wobbler2_StartTwang(LFO);
#ifdef INTPENDULUM
			Wobbler2_InitIntPendulum(&LFO->Pendulum, LFO);
#else
			Wobbler2_InitPendulum(&LFO->Pendulum, LFO);
#endif
		}
	}

	void Wobbler2_LoadSettings(Wobbler2_Settings *settings, Wobbler2_Params *params)
	{
		params->CalibNormal =0 ;
		params->CalibPhased = 0;
		params->T = 0;
		settings->SlowSpeedMult = 0;
	}

	void Wobbler2_ValidateParams(Wobbler2_Params *params)
	{

	}

	unsigned long Wobbler2_LFORange(int32_t V, int32_t SR)
	{
		return  1 + ((V*SR * 64) >> 8);
		//	return (unsigned long)(64 * pow((int32_t)((SR * 6) / 64.0), pow((int32_t)V, 0.54f)));
	}

	unsigned long Wobbler2_LFORange2(int32_t V, int32_t SR)
	{
		return  1 + V*V * 8;
		//	return (unsigned long)(64 * pow((int32_t)((SR * 6) / 64.0), pow((int32_t)V, 0.54f)));
	}

	unsigned long Wobbler2_LFORange3(int32_t V, int32_t SR)
	{
		return  1 + ((V*SR) >> 13);
	}

	void Wobbler2_StartTwang(Wobbler2_LFO_t *LFO)
	{
		Wobbler2_FancyEnv_Trigger(&LFO->FancyEnv);
	}

#define SNHTABCURVECOUNT 128
#define VAL(x) (uint32_t)((x*1024))
	uint32_t SNHTAB[SNHTABCURVECOUNT] = { VAL(0), VAL(0.25), VAL(0.5), VAL(0.75), VAL(1) };
	const uint32_t Cmapping[] = {
		0x03F0D370, 0x04113BF3, 0x0432AF01, 0x04553521, 0x0478D733, 0x049D9E4A, 0x04C393DB, 0x04EAC196, 0x05133184, 0x053CEE01, 0x056801B9, 0x059477BD, 0x05C25B5F, 0x05F1B862, 0x06229AEB, 0x06550F7A,
		0x068922FE, 0x06BEE2C9, 0x06F65C99, 0x072F9E9E, 0x076AB787, 0x07A7B67C, 0x07E6AB09, 0x0827A55A, 0x086AB609, 0x08AFEE3C, 0x08F75FB8, 0x09411CBC, 0x098D3826, 0x09DBC57E, 0x0A2CD8D7, 0x0A8086EF,
		0x0AD6E539, 0x0B3009CF, 0x0B8C0B83, 0x0BEB01D8, 0x0C4D052E, 0x0CB22E85, 0x0D1A97CD, 0x0D865BD1, 0x0DF595FE, 0x0E6862FF, 0x0EDEE00D, 0x0F592B8A, 0x0FD764CC, 0x1059AC2A, 0x10E022D7, 0x116AEB71,
		0x11FA2945, 0x128E0135, 0x132698FD, 0x13C417AB, 0x1466A591, 0x150E6C51, 0x15BB96BC, 0x166E5159, 0x1726C9F1, 0x17E52F8A, 0x18A9B2E6, 0x19748653, 0x1A45DDDA, 0x1B1DEEE9, 0x1BFCF0FA, 0x1CE31D14,
		0x1DD0AE1F, 0x1EC5E0BA, 0x1FC2F404, 0x20C8285F, 0x21D5C0FA, 0x22EC02BB, 0x240B34E4, 0x2533A0E4, 0x266592AA, 0x27A1587E, 0x28E74328, 0x2A37A668, 0x2B92D7DC, 0x2CF93096, 0x2E6B0C74, 0x2FE8C9D7,
		0x3172CA8D, 0x330973D6, 0x34AD2D72, 0x365E6291, 0x381D82C2, 0x39EAFFC4, 0x3BC75008, 0x3DB2EDBC, 0x3FAE5631, 0x41BA0BB5, 0x43D69409, 0x4604799B, 0x48444B8B, 0x4A969D09, 0x4CFC05F3, 0x4F75237A,
		0x5202981D, 0x54A50A1A, 0x575D26E2, 0x5A2BA094, 0x5D112EEE, 0x600E8FF1, 0x6324873B, 0x6653DF4B, 0x699D68E2, 0x6D01FA5D, 0x7082737D, 0x741FB99F, 0x77DAB864, 0x7BB4656B, 0x7FADBC91, 0x83C7C1D7,
		0x8803829B, 0x8C62145D, 0x90E4955A, 0x958C2C93, 0x9A5A0B05, 0x9F4F6C4F, 0xA46D94D1, 0xA9B5D389, 0xAF298355, 0xB4CA09B5, 0xBA98D626, 0xC09765E7, 0xC6C742B7, 0xCD2A0055, 0xD3C142BD, 0xDA8EB7F0
	};


	void Wobbler2_CalcSampleHoldFilter(uint16_t Mod, uint32_t DP, Wobbler2_SVF_Coeffs *Coeff)
	{
		if (Mod < 50)
		{
			Coeff->wet = (Mod * 255) / 50;
			Coeff->dry = 255 - Coeff->wet;
		}
		else
		{
			Coeff->wet = 255;
			Coeff->dry = 0;
		}
		Mod = __max(25, Mod);
		uint32_t freq = 0xffffffff / DP;
		Mod = 255 - Mod;
		Mod = ((Mod * 245) / 255);
	
		uint32_t m = LERP((int32_t*)Cmapping, SNHTABCURVECOUNT - 1, Mod);

		uint16_t res = ((m >> 16));

		Coeff->cutoff = res;
	}
#define HI16(x) (x>>16)
#define LO16(x) (x&65535)

	void Wobbler2_SVF(Wobbler2_SVFstruct *f, int32_t coeff, uint32_t inp)
	{
		signed short const tMid = HI16(f->mid);

		uint16_t sC = coeff;
		unsigned short Max = (0xf200 - sC);
		unsigned long fR = 0 * Max;
		
		uint16_t sR = ~HI16(fR);
		uint16_t scaler = 0xffff;
		
		scaler = 0xffff / 20 + HI16(coeff * (0xe000));

		f->high = (inp << 10) - f->low - sR * tMid;
		f->mid += (sC * HI16(f->high));
		f->low += ((HI16(sC * scaler))* tMid);		
	}

	void Wobbler2_SampleHold(Wobbler2_LFO_SNH_t *sh, Wobbler2_LFO_t *lfo, uint32_t phase, Wobbler2_SVF_Coeffs *coeff, uint32_t phase2)
	{
		int newseg = ((phase >> 29)) & 7;
		int const QUANT = 2048;
		if (newseg != sh->lastseg1 || sh->triggerhappened>0)
		{
			sh->lastseg1 = newseg;
			sh->segindex = (sh->segindex + 1) % 32;
			sh->segbuffer[sh->segindex] = ((Wobbler2_Rand(&sh->random)) >> 1);
			sh->lastval1 = sh->segbuffer[sh->segindex];
			sh->lastval3 = ((sh->lastval1 / QUANT) *QUANT * 4096) / 3580;
		}

		int newseg2 = ((((phase2) >> 29))) & 7;
		if (newseg2 != sh->lastseg2 || sh->triggerhappened>0)
		{
			sh->lastseg2 = newseg2;
			int segidx2 = (32 + sh->segindex - (lfo->Phasing >> 9)) % 32;
			sh->lastval2 = sh->segbuffer[segidx2];
			sh->lastval4 = ((sh->lastval2 / QUANT)*QUANT * 4096) / 3580;
		}

		sh->triggerhappened = 0;

		Wobbler2_SVF(&sh->F1, coeff->cutoff, sh->lastval1);
		Wobbler2_SVF(&sh->F2, coeff->cutoff, sh->lastval2);
		Wobbler2_SVF(&sh->F3, coeff->cutoff, sh->lastval3);
		Wobbler2_SVF(&sh->F4, coeff->cutoff, sh->lastval4);
		sh->normalout = ((sh->F1.low >> 10) * coeff->wet + sh->lastval1 * coeff->dry) >> 8;
		sh->delayedout = ((sh->F2.low >> 10) * coeff->wet + sh->lastval2 * coeff->dry) >> 8;
		sh->normalout_quantized = ((sh->F3.low >> 10) * coeff->wet + sh->lastval3 * coeff->dry) >> 8;
		sh->phasedout_quantized = ((sh->F4.low >> 10) * coeff->wet + sh->lastval4 * coeff->dry) >> 8;
	}

	int Wobbler2_Twang(Wobbler2_LFO_t *LFO, uint32_t phase)
	{
		return (Sine(phase ) >> 16) * (LFO->FancyEnv.currentcurved);
	}

#include "FreqLerp.h"

	unsigned long Wobbler2_MakeFreq(int input)
	{
		return LERP9bit(FreqLerp, input);
	}

	uint16_t Wobbler2_SkipThe1Percent(uint16_t input)
	{
		if (input > 0xffff / 40)
		{
			uint32_t r = input - (0xffff / 40);
			r *= 0xffff;
			r /= (0xffff - (0xffff / 20));
			if (r > 0xffff) return 0xffff;
			return r;
		}
		else
		{
			return 0;
		}
	}

	int Wobbler2_Get(Wobbler2_LFO_t *LFO, Wobbler2_Params *Params)
	{
		LFO->timesincesync++;
#ifdef INTPENDULUM
		Wobbler2_UpdateIntPendulumSettings(&LFO->Pendulum, LFO);
#else
		Wobbler2_UpdatePendulumSettings(&LFO->Pendulum, LFO);
#endif
		if (LFO->timesincesync > 30000 || LFO->syncindex == 0)
		{
			LFO->extsync = 0;
		}

		if (LFO->Gate[0] > 0) { LFO->Gate[0]--; }
		if (LFO->Gate[1] > 0) { LFO->Gate[1]--; }

		if (LFO->TriggerLed > 0) LFO->TriggerLed--;
		Wobbler2_FancyEnv_Update(&LFO->FancyEnv, LFO->Mod);

		uint32_t DP = 0;
		if (LFO->extsync)
		{
			SteppedResult_t SpeedGrade;
			Wobbler2_GetSteppedResult(LFO->SpeedOrig, WOBBLER_SPEEDRATIOCOUNT-1, &SpeedGrade);
			uint32_t DPorig = Wobbler2_MakeFreq(LFO->Speed);// Wobbler2_LFORange2(LFO->Speed << 2, 0);;
			int32_t interpint = WobGetInterpolatedResultInt(Wobbler_SpeedRatioSet, &SpeedGrade);
			int64_t interm = LFO->SyncDP;
			interm *= interpint;
			interm >>= 16;
			DP = interm;// ((LFO->SyncDP >> 16) * interpint);
			uint32_t DPdiff = DPorig - DP;
		}
		else
		{
			DP = Wobbler2_MakeFreq(LFO->Speed);// Wobbler2_LFORange2(LFO->Speed << 2, 0);;
		}

		LFO->Phase1 += DP;
		LFO->SlomoPhase1 += DP / 64;
		uint32_t DP2 = LFO->Phasing * 0x100000;
		uint32_t DP3 = Wobbler2_MakeFreq(LFO->Phasing >> 3);
		//DP2 <<= 24;
		LFO->Phase2 = LFO->Phase1 + DP2;
		LFO->SlomoPhase2 = LFO->SlomoPhase1 + DP2;
		LFO->Phase2Rev = LFO->Phase1 - DP2;
		

		if (LFO->Phase1 < LFO->OldPhase1)
		{
			LFO->Gate[1] = Wobbler2_GATECOUNTDOWN;			
		}
		LFO->OldPhase1 = LFO->Phase1;

		Wobbler2_CalculateCompensation(&LFO->CompensationVals, LFO->Mod >> 8);

		// slomo mode
		LFO->OutputsNormal[0] = FillBasicShapes(LFO->SlomoPhase1, LFO->Mod >> 8, &LFO->SlomoBasicShapesA, &LFO->CompensationVals);
		LFO->OutputsPhased[0] = FillBasicShapes(LFO->SlomoPhase2, LFO->Mod >> 8, &LFO->SlomoBasicShapesB, &LFO->CompensationVals);

		// normal mode
		LFO->OutputsNormal[1] = FillBasicShapes(LFO->Phase1, LFO->Mod >> 8, &LFO->BasicShapesA, &LFO->CompensationVals);
		LFO->OutputsPhased[1] = FillBasicShapes(LFO->Phase2, LFO->Mod >> 8, &LFO->BasicShapesB, &LFO->CompensationVals);

		// self-phasing mode
		uint32_t  newdp3mul = fix16_add(fix16_from_int(1), LFO->Phasing * ((0xffff * 16) / 4096));
		uint32_t  newdp3b = fix16_mul(DP, newdp3mul);

		LFO->PhasedShift += newdp3b - DP;
		//LFO->PhasedShift += DP;
		//LFO->PhasedShift = fix16_mul(LFO->OutputsNormal[1], (LFO->Phasing<<10));


		LFO->OutputsNormal[2] = (BasicShapes(LFO->Phase1 + LFO->PhasedShift, LFO->Mod >> 8, &LFO->CompensationVals) + LFO->OutputsNormal[1]) / 2;
		LFO->OutputsPhased[2] = (BasicShapes(LFO->Phase2 + LFO->PhasedShift, LFO->Mod >> 8, &LFO->CompensationVals) + LFO->OutputsPhased[1]) / 2;

		// twang
		LFO->OutputsNormal[3] = Wobbler2_Twang(LFO, LFO->Phase1);
		LFO->OutputsPhased[3] = Wobbler2_Twang(LFO, LFO->Phase2);

		// pendulum precalc
#ifdef INTPENDULUM
		Wobbler2_DoublePendulumInt(&LFO->Pendulum, LFO->BasicShapesA.Sine);
#else
		Wobbler2_DoublePendulum(&LFO->Pendulum, 0.05f, LFO->BasicShapesA.Sine);
#endif
		// pendulum
		LFO->OutputsNormal[4] = (LFO->Pendulum.As >> 16) * (LFO->FancyEnv.currentcurved);// + 0x80000000;
		LFO->OutputsPhased[4] = (LFO->Pendulum.Bs >> 16) * (LFO->FancyEnv.currentcurved);// + 0x80000000;

		// sample/hold precalc
		Wobbler2_CalcSampleHoldFilter(LFO->Mod >> 8, DP, &LFO->SVFCoeffs);
		Wobbler2_SampleHold(&LFO->SNH, LFO, LFO->Phase1, &LFO->SVFCoeffs, LFO->Phase2Rev);

		// sample/hold 
		LFO->OutputsNormal[5] = (int32_t)(LFO->SNH.normalout << 16) - (1 << 29);
		LFO->OutputsPhased[5] = (int32_t)(LFO->SNH.delayedout << 16) - (1 << 29);

		// sample/hold quantized
		LFO->OutputsNormal[6] = (int32_t)(LFO->SNH.normalout_quantized << 16) - (1 << 29);
		LFO->OutputsPhased[6] = (int32_t)(LFO->SNH.phasedout_quantized << 16) - (1 << 29);

		// calculate phased phasereset gate
		if ((LFO->BasicShapesB.Sine > 0) && (LFO->OldPhase2 <= 0)) { LFO->Gate[0] = Wobbler2_GATECOUNTDOWN; }
		LFO->OldPhase2 = LFO->BasicShapesB.Sine;

		Wobbler2_GetSteppedResult(LFO->Shape, 6, &LFO->ShapeStepped);

		// interpolate between al the shapes
		LFO->Output = WobGetInterpolatedResultInt(LFO->OutputsNormal, &LFO->ShapeStepped) / (0xffff * 4);
		LFO->OutputPhased = WobGetInterpolatedResultInt(LFO->OutputsPhased, &LFO->ShapeStepped) / (0xffff * 4);
		
		// multiply outputs by level knobs
		LFO->OutputPreCalib = (LFO->Output * LFO->Amount1) / (int)(1 << 14);
		LFO->OutputPhasedPreCalib = (LFO->OutputPhased * LFO->Amount2) / (int)(1 << 14);
		
		LFO->OutputPreCalib += 2048;
		LFO->OutputPhasedPreCalib += 2048;
		LFO->Output = LFO->OutputPreCalib + LFO->CalibNormal;
		LFO->OutputPhased = LFO->OutputPhasedPreCalib + LFO->CalibPhased;

		if (LFO->Output > 4095) LFO->Output = 4095; else if (LFO->Output < 0) LFO->Output = 0;
		if (LFO->OutputPhased > 4095) LFO->OutputPhased = 4095; else if (LFO->OutputPhased < 0) LFO->OutputPhased = 0;

		if (LFO->OutputPreCalib > 4095) LFO->OutputPreCalib = 4095; else if (LFO->OutputPreCalib < 0) LFO->OutputPreCalib = 0;
		if (LFO->OutputPhasedPreCalib > 4095) LFO->OutputPhasedPreCalib = 4095; else if (LFO->OutputPhasedPreCalib < 0) LFO->OutputPhasedPreCalib = 0;

		Wobbler2_DoLeds(LFO);
		LFO->LastPhasing = LFO->Phasing;

		if (LFO->Output>LFO->OutputMax) LFO->OutputMax = LFO->Output; else if (LFO->Output<LFO->OutputMin) LFO->OutputMin = LFO->Output;
		if (LFO->OutputPhased>LFO->OutputPhasedMax) LFO->OutputPhasedMax = LFO->OutputPhased;else if (LFO->OutputPhased<LFO->OutputPhasedMin) LFO->OutputPhasedMin = LFO->OutputPhased;

		return LFO->Output;
	}

#include "BasicShapeCompensation.h"

	void Wobbler2_CalculateCompensation(ShapeCompensationVals_t *Comp, int mod)
	{
		Comp->min = LERP((int32_t*)BasicShapeLow, SHAPECOMPENSATIONCOUNT - 1, mod);
		Comp->mul = LERP((int32_t*)BasicShapeMult, SHAPECOMPENSATIONCOUNT - 1, mod);
	}

	void Wobbler2_DoLeds(Wobbler2_LFO_t *LFO)
	{
		LFO->T++;

		for (int i = 0; i < 9; i++)
		{
			LFO->Led[0][i] = 0;
			LFO->Led[1][i] = 0;
		}

		int32_t LedIdxB = (LFO->OutputPreCalib * 8);
		int iLedIdxB = LedIdxB >> 12;
		int IdxB = ((LedIdxB - (iLedIdxB << 12))) >> 4;

		LFO->Led[0][8 - ((iLedIdxB + 9) % 9)] = 255 - IdxB;
		LFO->Led[0][8 - ((iLedIdxB + 10) % 9)] = IdxB;

		int32_t LedIdxA = (LFO->OutputPhasedPreCalib * 8);
		int iLedIdxA = LedIdxA >> 12;
		int IdxA = ((LedIdxA - (iLedIdxA << 12))) >> 4;

		LFO->Led[1][8 - ((iLedIdxA + 9) % 9)] = 255 - IdxA;
		LFO->Led[1][8 - ((iLedIdxA + 10) % 9)] = IdxA;

		for (int i = 0; i < 5; i++)
		{
			LFO->ModeLed[i] = 0;
		};
		int idx = LFO->ShapeStepped.index;
		if (idx > 0 && idx < 5)
		{
			idx--;
			LFO->ModeLed[idx] = 255 - LFO->ShapeStepped.fractional;

			if (idx < 4)
			{
				LFO->ModeLed[idx + 1] = LFO->ShapeStepped.fractional;
			}
		}
		else
		{
			if (idx == 0)
			{
				LFO->ModeLed[0] = ((LFO->T / 300) % 2 == 0) ? 255 : (128 + LFO->ShapeStepped.fractional / 2);
			}
			else
			{
				if (idx < 6)
				{
					LFO->ModeLed[4] = ((LFO->T / 300) % 2 == 0) ? 255 : (255 - LFO->ShapeStepped.fractional / 2);
				}
				else
				{
					LFO->ModeLed[4] = ((LFO->T / 300) % 2 == 0) ? 255 : 128;
				}
			}
		}
	}

	int32_t WobGetInterpolatedResultInt(int32_t *table, SteppedResult_t *inp)
	{
		unsigned char F = inp->fractional;
		if (F == 0) return table[inp->index];
		unsigned char IF = ~inp->fractional;
		return ((table[inp->index] / 256) * IF) + ((table[inp->index + 1] / 256) * F);
	}

	void Wobbler2_GetSteppedResult(uint16_t param, uint8_t steps, SteppedResult_t *out)
	{
		//max(floor(x + 0.25), (x + 0.25 - floor(x + 0.25)) * 2 + floor(x + 0.25) - 1)

		uint32_t X = (param * steps) / 256;
		X += 64;

		int FloorX = X & 0xffffff00;
		int Aside = FloorX;
		int Bside = (X - (FloorX)) * 2 + (FloorX)-256;
		int M = (Aside > Bside) ? Aside : Bside;

		out->index = M >> 8;
		out->fractional = M & 0xff;
	}

	void Wobbler2_FancyEnv_Init(Wobbler2_FancyEnv_t* env)
	{
		env->current = WOBBLER2_FANCYENV_IDLE;
		env->endlevel = 0;
		env->state = 0;
	};

	void Wobbler2_FancyEnv_Trigger(Wobbler2_FancyEnv_t* env)
	{
		env->state = WOBBLER2_FANCYENV_ATTACK;
	};

#define FIX(x)((int32_t)((x)*65536.0f))


	int32_t Wobbler2_EnvTransferFunc(int32_t S)
	{
		//	return fix16_sadd(fix16_sdiv(FIX(1), fix16_sub(S, FIX(1))), fix16_sdiv(FIX(1), fix16_add(S, FIX(1))));
		return fix16_mul(S, fix16_mul(S, S));
	};

	int32_t Wobbler2_FancyEnv_Update(Wobbler2_FancyEnv_t *env, int32_t param)
	{
		uint32_t A = 0;
		uint32_t R = Wobbler2_LFORange3(128 << 8, WOBBLERSAMPLERATE);
		env->endlevel = 0;
		if ((param >> 8) < 128)
		{
			const int minR = (int)(0.055f * (float)WOBBLERSAMPLERATE);
			R = minR + (Wobbler2_LFORange3(param, WOBBLERSAMPLERATE)) * 2;
		}
		else
		{
			A = 1 + (Wobbler2_LFORange3(param - (128 << 8), WOBBLERSAMPLERATE));
			if ((param >> 8) > (255 - 10))
			{
				env->endlevel = ((param >> 8) - (255 - 10))* ((1 << 24) / (10));
			}
		}


		switch (env->state)
		{
			case WOBBLER2_FANCYENV_IDLE: env->currentcurved = 0; return env->current;
			case WOBBLER2_FANCYENV_ATTACK:
			{
				int32_t attacklowerbound = FIX(0.8);
				int32_t attackupperbound = FIX(0.002);

				int32_t attackhighval = Wobbler2_EnvTransferFunc(attacklowerbound);
				int32_t attacklowval = Wobbler2_EnvTransferFunc(attackupperbound);
				int32_t attackbaseline = -attacklowval;
				int32_t attackrange = fix16_sdiv(FIX(1.0), fix16_ssub(attackhighval, attacklowval));

				if (A == 0)
				{
					env->state = WOBBLER2_FANCYENV_DECAY;
					env->current = 1 << 24;
				}
				else
				{
					env->current += ((1 << 24) - 1) / A;
					if (env->current >= 1 << 24) { env->current = 1 << 24; env->state = WOBBLER2_FANCYENV_DECAY; };
				}

				int32_t scaled = fix16_add(fix16_mul(fix16_ssub(FIX(1.0), env->current >> 8), fix16_ssub(attackupperbound, attacklowerbound)), attacklowerbound);
				env->currentcurved = fix16_mul(fix16_add(Wobbler2_EnvTransferFunc(scaled), attackbaseline), attackrange);

				//	env->currentcurved = env->current >> 8;
				return env->current;
			}
			case WOBBLER2_FANCYENV_DECAY:
			{
				env->current -= ((1 << 24) - 1) / R;

				if (env->current <= env->endlevel) {
					env->current = env->endlevel;
					if (env->current == 0) env->state = WOBBLER2_FANCYENV_IDLE;
				};

				int32_t decaylowerbound = FIX(0.9);
				int32_t decayupperbound = FIX(-0.016);

				if (param < 0x8000)
				{
					decaylowerbound = fix16_sub(FIX(0.9f), fix16_smul(param * 2, FIX(0.9f - 0.19f)));
					decayupperbound = -fix16_smul(param * 2, FIX(0.19));
				}
				else
				{
					decaylowerbound = fix16_sub(FIX(0.9f), fix16_smul(0x8000 * 2, FIX(0.9f - 0.19f)));
					decayupperbound = fix16_sub(-fix16_smul(0x8000 * 2, FIX(0.19)), fix16_smul(FIX(0.9), (param - 0x8000) * 2));
				}

				int32_t decayhighval = Wobbler2_EnvTransferFunc(decaylowerbound);
				int32_t decaylowval = Wobbler2_EnvTransferFunc(decayupperbound);
				int32_t decaybaseline = -decaylowval;
				int32_t decayrange = fix16_sdiv(FIX(1.0), fix16_ssub(decayhighval, decaylowval));


				int32_t scaled = fix16_add(fix16_mul(fix16_ssub(FIX(1.0), env->current >> 8), fix16_ssub(decayupperbound, decaylowerbound)), decaylowerbound);

				env->currentcurved = fix16_mul(fix16_add(Wobbler2_EnvTransferFunc(scaled), decaybaseline), decayrange);

				return env->current;
			}

		}
			return 0;
	}

