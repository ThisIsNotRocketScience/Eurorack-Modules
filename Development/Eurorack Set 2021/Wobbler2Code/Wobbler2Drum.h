#pragma once

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "../EurorackShared/EurorackShared.h"
#include "Wobbler2.h"

extern const uint32_t Cmapping[];

typedef struct
{
	int32_t lo;
	int32_t mid;
	int32_t hi;
} intsvg_t;

const unsigned short tblDecayTime[] = {
	0x0002, 0x0003, 0x0003, 0x0004, 0x0005, 0x0005, 0x0006, 0x0007, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000C, 0x000D,
	0x000E, 0x000F, 0x0010, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0018, 0x0019, 0x001B, 0x001C, 0x001E, 0x001F, 0x0021, 0x0023,
	0x0024, 0x0026, 0x0028, 0x002A, 0x002C, 0x002E, 0x0031, 0x0033, 0x0035, 0x0038, 0x003A, 0x003D, 0x003F, 0x0042, 0x0045, 0x0048,
	0x004B, 0x004E, 0x0051, 0x0055, 0x0058, 0x005C, 0x005F, 0x0063, 0x0067, 0x006B, 0x006F, 0x0073, 0x0078, 0x007C, 0x0081, 0x0085,
	0x008A, 0x008F, 0x0095, 0x009A, 0x009F, 0x00A5, 0x00AB, 0x00B1, 0x00B7, 0x00BD, 0x00C4, 0x00CA, 0x00D1, 0x00D8, 0x00DF, 0x00E7,
	0x00EE, 0x00F6, 0x00FE, 0x0106, 0x010F, 0x0118, 0x0121, 0x012A, 0x0133, 0x013D, 0x0147, 0x0151, 0x015B, 0x0166, 0x0171, 0x017C,
	0x0187, 0x0193, 0x019F, 0x01AC, 0x01B8, 0x01C5, 0x01D3, 0x01E0, 0x01EE, 0x01FD, 0x020B, 0x021A, 0x022A, 0x0239, 0x024A, 0x025A,
	0x026B, 0x027C, 0x028E, 0x02A0, 0x02B3, 0x02C6, 0x02D9, 0x02ED, 0x0302, 0x0317, 0x032C, 0x0342, 0x0358, 0x036F, 0x0387, 0x039E};

const unsigned long tblNoteTable[] = {
	0x000C265D, 0x000CDF51, 0x000DA344, 0x000E72DE, 0x000F4ED0, 0x001037D7, 0x00112EB8, 0x00123448, 0x00134965, 0x00146EFD, 0x0015A60A, 0x0016EF96, 0x00184CBB, 0x0019BEA2, 0x001B4689, 0x001CE5BD,
	0x001E9DA1, 0x00206FAE, 0x00225D71, 0x00246891, 0x002692CB, 0x0028DDFB, 0x002B4C15, 0x002DDF2D, 0x00309976, 0x00337D45, 0x00368D12, 0x0039CB7A, 0x003D3B43, 0x0040DF5C, 0x0044BAE3, 0x0048D122,
	0x004D2597, 0x0051BBF7, 0x0056982B, 0x005BBE5B, 0x006132ED, 0x0066FA8B, 0x006D1A24, 0x007396F4, 0x007A7686, 0x0081BEB9, 0x008975C6, 0x0091A244, 0x009A4B2F, 0x00A377EE, 0x00AD3056, 0x00B77CB6,
	0x00C265DB, 0x00CDF516, 0x00DA3449, 0x00E72DE9, 0x00F4ED0D, 0x01037D73, 0x0112EB8C, 0x01234489, 0x0134965F, 0x0146EFDC, 0x015A60AD, 0x016EF96D, 0x0184CBB6, 0x019BEA2D, 0x01B46892, 0x01CE5BD2,
	0x01E9DA1A, 0x0206FAE6, 0x0225D719, 0x02468912, 0x02692CBF, 0x028DDFB9, 0x02B4C15A, 0x02DDF2DB, 0x0309976D, 0x0337D45B, 0x0368D125, 0x039CB7A5, 0x03D3B434, 0x040DF5CC, 0x044BAE33, 0x048D1225,
	0x04D2597F, 0x051BBF72, 0x056982B5, 0x05BBE5B7, 0x06132EDB, 0x066FA8B6, 0x06D1A24A, 0x07396F4B, 0x07A76868, 0x081BEB99, 0x08975C67, 0x091A244A, 0x09A4B2FE, 0x0A377EE5, 0x0AD3056A, 0x0B77CB6E,
	0x0C265DB7, 0x0CDF516D, 0x0DA34494, 0x0E72DE96, 0x0F4ED0D1, 0x1037D732, 0x112EB8CE, 0x12344894, 0x134965FD, 0x146EFDCB, 0x15A60AD5, 0x16EF96DC, 0x184CBB6F, 0x19BEA2DB, 0x1B468928, 0x1CE5BD2C,
	0x1E9DA1A3, 0x206FAE64, 0x225D719D, 0x24689129, 0x2692CBFA, 0x28DDFB96, 0x2B4C15AA, 0x2DDF2DB9, 0x309976DF, 0x337D45B6, 0x368D1251, 0x39CB7A58, 0x3D3B4347, 0x40DF5CC9, 0x44BAE33A, 0x48D12252};

enum
{
	RATTLEENV_STOP,
	RATTLEENV_HOLD,
	RATTLEENV_DECAY,
	RATTLEENV_LASTDECAY,
	RATTLEENV_REST,
	RATTLEENV_ATTACK,
};

enum
{
	DECAYENV_STOP,
	DECAYENV_HOLD,
	DECAYENV_DECAY,
	DECAYENV_ATTACK,
};

class Filter
{
#define HI16(x) (x >> 16)
#define LO16(x) (x & 65535)
public:
	intsvg_t filt;

	int32_t GetLP(int32_t input)
	{
		Pump(input);
		return filt.lo;
	}

	int32_t GetBP(int32_t input)
	{
		Pump(input);
		return filt.mid;
	}

	int32_t GetHP(int32_t input)
	{
		Pump(input);
		return filt.hi;
	}

	void SetCut(int val) // val is midi range! 0-127
	{
		unsigned long nC = Cmapping[val];

		if (nC > C)
		{
			dC = (nC - C) / 80;
			finertiacount = 80;
		}
		else
		{
			if (nC < C)
			{
				dC = (C - nC) / 80;
				dC = -dC;
				finertiacount = 80;
			}
		}
	}
	void SetRes(int val) // val is midi range! 0-127
	{
		R = (unsigned short)((val << 8) + (64 << 9));
		unsigned short Max = (0xf200 - sC);
		unsigned long fR = R * Max;
		sR = ~HI16(fR);
	}
	unsigned long C;
	signed long dC;
	unsigned short sC;
	unsigned short R;
	unsigned short sR;

	unsigned long finertiacount;

	void Pump(int32_t input)
	{
		if (finertiacount)
		{
			C += dC;
			finertiacount--;
		}
		sC = HI16(C);

		signed short const tMid = HI16(filt.mid);
		filt.hi = (input)-filt.lo - sR * tMid;
		filt.mid += sC * HI16(filt.hi);
		filt.lo += sC * tMid;
	}
};

class DecayEnv
{
public:
	int EnvTime;
	int EnvState;
	int EnvCurrent;
	int EnvDelta;
	int DecayTime;
	int HoldTime;
	int AttackTime;

	void CopyFrom(DecayEnv *D)
	{
		EnvTime = D->EnvTime;
		DecayTime = D->DecayTime;
		AttackTime = D->AttackTime;
		HoldTime = D->HoldTime;
		pShape = D->pShape;
		pDecay = D->pDecay;
	}

	DecayEnv()
	{
		EnvCurrent = 0;
		EnvDelta = 0;
		EnvTime = 0;
		AttackTime = 5;
		HoldTime = 50;
		DecayTime = 500;
		EnvState = DECAYENV_STOP;
	}

	void Trigger()
	{
		EnvState = DECAYENV_ATTACK;
		EnvTime = AttackTime;
		EnvDelta = ((1 << 24) - EnvCurrent) / AttackTime;
	}

	unsigned short pShape;
	unsigned short pDecay;
	void SetAttack(uint8_t attacktime) // 7bit midi time
	{
		AttackTime =  5 + (tblDecayTime[attacktime]);
	}
	void SetParam(unsigned short shape, unsigned short decay, int decaymult)
	{
		pShape = shape;
		pDecay = decay;
		int idx = decay >> 9;
		int v = decay & 0x1ff;
		int iv = 0x1ff - v;
		DecayTime = (tblDecayTime[idx] * v + tblDecayTime[idx + 1] * iv) * decaymult;
	}

	int32_t Get()
	{
		if (EnvState == DECAYENV_STOP)
			return 0;
		EnvCurrent += EnvDelta;
		EnvTime--;
		if (EnvTime == 0)
		{
			switch (EnvState)
			{
			case DECAYENV_ATTACK:
				EnvState = DECAYENV_HOLD;
				EnvDelta = 0;
				EnvCurrent = 1 << 24;
				EnvTime = HoldTime;
				break;
			case DECAYENV_HOLD:

				EnvState = DECAYENV_DECAY;
				EnvTime = DecayTime;
				EnvDelta = -1 * ((1 << 24) / EnvTime);

				break;
			case DECAYENV_DECAY:
				EnvState = DECAYENV_STOP;
				EnvDelta = 0;
				EnvCurrent = 0;
				break;
			default:
				EnvState = DECAYENV_STOP;
				break;
			}
		}

		uint32_t SqrEnv = EnvCurrent >> 12;
		SqrEnv *= EnvCurrent >> 12;

		return SqrEnv;
	}
};
class RattleEnv
{
public:
	int RattleCount;

	void CopyFrom(RattleEnv *R)
	{
		DecayTime = R->DecayTime;
		RestTime = R->RestTime;
		RattleCount = R->RattleCount;
		EnvTime = R->EnvTime;
	}
	int EnvTime;
	int EnvState;
	int RattleLeft;
	int EnvCurrent;
	int EnvDelta;
	int DecayTime;
	int HoldTime;
	int RestTime;
	int LastDecayMultiplier;

	void SetDecayAndRestSymmetrical(int T)
	{
		DecayTime = RestTime = T;
	};

	RattleEnv()
	{
		EnvCurrent = 0;
		EnvDelta = 0;
		RattleCount = 5;
		RattleLeft = 0;
		EnvTime = 0;
		HoldTime = 20;
		DecayTime = 500;
		RestTime = 500;
		LastDecayMultiplier = 8;
		EnvState = RATTLEENV_STOP;
	}

	void Trigger()
	{
		EnvState = RATTLEENV_ATTACK;
		EnvTime = 5;
		EnvDelta = ((1 << 24) - EnvCurrent) / EnvTime;
		RattleLeft = RattleCount;
	}

	void SetParam(unsigned short spacing, unsigned short decay)
	{
		int idx = decay >> 9;
		int v = decay & 0x1ff;
		int iv = 0x1ff - v;
		DecayTime = (tblDecayTime[idx] * v + tblDecayTime[idx + 1] * iv) >> 4;

		idx = spacing >> 9;
		v = spacing & 0x1ff;
		iv = 0x1ff - v;

		RestTime = (tblDecayTime[idx] * v + tblDecayTime[idx + 1] * iv) >> 4;
	}

	inline int32_t Get()
	{
		if (EnvState == RATTLEENV_STOP)
			return 0;
		EnvCurrent += EnvDelta;
		EnvTime--;
		if (EnvTime == 0)
		{
			switch (EnvState)
			{
			case RATTLEENV_ATTACK:
				EnvState = RATTLEENV_HOLD;
				EnvDelta = 0;
				EnvCurrent = 1 << 24;
				EnvTime = HoldTime;
				break;
			case RATTLEENV_HOLD:

				EnvState = RATTLEENV_DECAY;
				if (--RattleLeft == 0)
				{
					EnvTime = DecayTime * LastDecayMultiplier;
				}
				else
				{
					EnvTime = DecayTime;
				}
				EnvDelta = -1 * ((1 << 24) / EnvTime);

				break;
			case RATTLEENV_DECAY:
				EnvState = RATTLEENV_REST;
				EnvDelta = 0;
				EnvCurrent = 0;
				EnvTime = RestTime;
				break;
			case RATTLEENV_REST:
				if (RattleLeft > 0)
				{
					EnvState = RATTLEENV_ATTACK;
					EnvTime = 5;
					EnvDelta = ((1 << 24) - EnvCurrent) / EnvTime;
				}
				else
				{
					EnvState = RATTLEENV_STOP;
					EnvDelta = 0;
					EnvCurrent = 0;
				}
				break;
			default:
				EnvState = RATTLEENV_STOP;
				break;
			}
		}
		return EnvCurrent;
	}
};

class Wobbler2Drum
{

public:
	Filter Filt;
	int Phase;

	int Shape;
	int Freq;
	int Mod;

	int M1;
	int M2;

	SteppedResult_t ShapeStepped;

#define TABBITS 11
#define TABSHIFT (32 - TABBITS)
#define WOBTABLEN (1 << TABBITS)
#define WOBTABMASK (WOBTABLEN - 1)

	int32_t sintab[WOBTABLEN];
	RattleEnv ClapRattle;
	DecayEnv SnareNoiseAmp;
	DecayEnv BdDecay;
	DecayEnv PDecay;
	Wobbler2Drum()
	{
		Filt.SetCut(24);
		Filt.SetRes(10);

		for (int i = 0; i < WOBTABLEN; i++)
		{
			sintab[i] = (int)(sinf((i * 6.283f) / (float)WOBTABLEN) * 32767.0f);
		}
		SnareNoiseAmp.SetParam(70 << 9, 12 << 9, 1);
		BdDecay.SetParam(70 << 9, 30 << 9, 1);
		BdDecay.HoldTime = 1000;

		PDecay.SetParam(70 << 9, 12 << 9, 1);
	}

	uint32_t OscPhase2;
	uint32_t OscPhase3;
	uint32_t OscPhase4;
	uint32_t OscPhase1;
	int32_t DPhase;
	void Trigger(bool ON = true)
	{
		if (ON)
		{

			// int64_t F = (int64_t)0xffffffff / 440;
			// DPhase = F ;
			Mod = 0x1fffff;
			ClapRattle.Trigger();
			SnareNoiseAmp.Trigger();
			BdDecay.Trigger();
			PDecay.Trigger();

			OscPhase1 = OscPhase2 = OscPhase3 = OscPhase4 = 0;
		}
	}

	void SetMod(uint16_t newmod)
	{
		Mod = newmod;
		if (Mod > 0x8000)
		{
			PitchMul = Mod - 32768;
			PitchMul *= PitchMul;
		}
		else
		{
			PitchMul = Mod - 32768;
			PitchMul *= -PitchMul;
		};

		PitchMul /= 32768;
	}
	void SetFreq(uint16_t newfreq)
	{
		Filt.SetCut(Freq >> 9);
		BaseDPhase = tblNoteTable[Freq >> 9] << 1;
	}
	int32_t BaseDPhase = 0;
	int PitchMul = 1;

	void SetShape(uint16_t newshape)
	{

		// shape dependent parameters

		Shape = newshape;
		Wobbler2_GetSteppedResult(Shape, 6, &ShapeStepped);

		if (ShapeStepped.index >= 5)
			ClapRattle.SetDecayAndRestSymmetrical(300);
		else
			ClapRattle.SetDecayAndRestSymmetrical(800);
		if (Shape < 0x500)
		{
			int Idx = ((0x500-Shape) * 127) / 0x500;
			PDecay.SetAttack(Idx);
			BdDecay.SetAttack(Idx);
		}
		else
		{
			PDecay.SetAttack(0);
			BdDecay.SetAttack(0);
		}
	}
	void Get(int32_t &L, int32_t &R)
	{
		SetFreq(Freq);
		SetMod(Mod);
		SetShape(Shape);

		DPhase = BaseDPhase;

		int32_t DR[7]; // = { 0 };

		int PDec = PDecay.Get()>>9;

		int32_t neutral = 0x10000;
		neutral += (PitchMul * PDec) >> 12;
		OscPhase1 += (DPhase>>16)*neutral;
		OscPhase2 += (DPhase * 16) / 10;
		OscPhase3 += (DPhase * 227) / 50;
		OscPhase4 += (DPhase * 454) / 50;

		// x2.27   x4.54
		// f1 = 500 Hz, f2 = 220 Hz, f3 = 1000Hz.

		int ClapEnv = ClapRattle.Get() >> 8;
		int noise = Filt.GetBP(rand()) >> 16;
		int snareenv = SnareNoiseAmp.Get();
		int tombdenv = BdDecay.Get();

		int idx1 = OscPhase1 >> TABSHIFT;
		int idx2 = OscPhase2 >> TABSHIFT;
		int idx3 = OscPhase3 >> TABSHIFT;
		int idx4 = OscPhase4 >> TABSHIFT;
		int idxdbl = (OscPhase1 >> (TABSHIFT - 1)) & WOBTABMASK;

		const int32_t sinidx1 = sintab[idx1];
		const int32_t sinidxdbl = sintab[idxdbl];
		const int32_t sinidx2 = sintab[idx2];
		const int32_t sinidx3 = sintab[idx3];
		const int32_t sinidx4 = sintab[idx4];

		DR[0] = (sinidx1 * (tombdenv >> 10)) >> 16;
		DR[1] = (sinidxdbl * (tombdenv >> 10)) >> 16;
		;
		DR[2] = (((sinidx4 + sinidx3 + sinidxdbl) * (tombdenv >> 10)) >> 16);

		int ClapSqrEnv = ClapEnv >> 13;
		ClapSqrEnv *= ClapEnv >> 12;
		DR[3] = ((((tombdenv >> 8) * sinidx3 + ((0xffffff - tombdenv) >> 8) * sinidxdbl) >> 16) * (ClapSqrEnv)) >> 8; // ploink
		DR[4] = (((sinidx2 + sinidx1) * (tombdenv >> 10)) >> 16) + ((noise * (snareenv >> 10)) >> 16);
		; // snare
		DR[5] = ((noise * (snareenv >> 10)) >> 16);
		;
		;									   // hat
		DR[6] = (noise * (ClapEnv >> 8)) >> 8; // clap

		// interpolate between al the shapes

		uint32_t O1 = WobGetInterpolatedResultIntNew(DR, &ShapeStepped); // / (0xffff * 4);
		L = O1;
		R = O1;
		//		L = ( O1*65535) ;
		//		R = ( O1 * 65535) / 65536;
	}
};
