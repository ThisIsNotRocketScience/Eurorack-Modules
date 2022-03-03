#include <stdint.h>
#include "EurorackShared.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define SVFHI16(x) (x>>16)
#define SVFLO16(x) (x&65535)
	void ResetSVF(struct EURORACK_SVF *filt)
	{
		filt->lo = 0;
		filt->mid = 0;
		filt->hi = 0;
	}

	void SetSVF(struct EURORACK_SVF *filt, uint16_t cut, uint16_t res)
	{

		filt->Cutoff = cut << 8;
		filt->Resonance = res << 8;
		//unsigned short R = (unsigned short)((res << 8) + (64 << 9));
		//unsigned short Max = (0xf200 - filt->Cutoff);
		//u/nsigned long fR = R * Max;
		//filt->Resonance = ~HI16(fR);

		//filt->Resonance = ~(res << 9);

	};

	void ProcessSVF(struct EURORACK_SVF *filt, uint32_t RR)
	{
		signed short const tMid = SVFHI16(filt->mid);

		filt->hi = (RR << 12) - filt->lo - filt->Resonance * tMid;
		filt->mid += filt->Cutoff * SVFHI16(filt->hi);
		filt->lo += filt->Cutoff * tMid;
	}


	/// A sine approximation via a fourth-order cosine approx.
	/// @param x   angle (with 2^15 units/circle)
	/// @return     Sine value (Q12)
	int32_t isin_S4(int32_t x)
	{
		int c, y;
		static const int qN = 13, qA = 12, B = 19900, C = 3516;

		c = x << (30 - qN);              // Semi-circle info into carry.
		x -= 1 << qN;                 // sine -> cosine calc

		x = x << (31 - qN);              // Mask with PI
		x = x >> (31 - qN);              // Note: SIGNED shift! (to qN)
		x = x*x >> (2 * qN - 14);          // x=x^2 To Q14

		y = B - (x*C >> 14);           // B - x^2*C
		y = (1 << qA) - (x*y >> 16);       // A - x^2*(B-x^2*C)

		return c >= 0 ? y : -y;
	}


	int32_t Sine(uint32_t phase)
	{
		int32_t P = phase >> 17;
		return isin_S4(P) << 17;

	}

	int32_t Cosine(uint32_t phase)
	{
		phase += 0x2000000;
		int32_t P = phase >> 17;		
		return isin_S4(P) << 17;
	}


	int32_t SawTooth(uint32_t phase)
	{
		return (*(int32_t*)&phase) >> 2;
	}

	int32_t Pulse(uint32_t phase)
	{
		if (phase & 0x80000000)
		{
			return INT32_MIN >> 2;
		}
		else
		{
			return INT32_MAX >> 2;
		}
	}

	int32_t Triangle(uint32_t phase)
	{
		if (phase & 0x80000000)
		{
			return (~(*(int32_t*)&(phase)) - 0x40000000) >> 1;
		}
		else
		{
			return ((*(int32_t*)&(phase)) - 0x40000000) >> 1;
		}
	}

	int32_t LERP(int32_t *V, int total, int fade)
	{
		int T = fade * total;
		unsigned char frac = T & 0xff;
		if (frac && (frac < 255)) frac += 1;
		int I = T >> 8;
		return ((V[I] >> 8) *(255 - frac) + (V[I + 1] >> 8) * frac);
	}

	int32_t LERP16(int32_t *V, int total, int fade)
	{
		int T = fade * total;
		unsigned short frac = T & 0xffff;
		if (frac && (frac < 0xffff)) frac += 1;
		int I = T >> 16;

		int64_t tempOut;
		tempOut = ((int64_t)V[I] * ((0x10000 - frac)));
		tempOut += ((int64_t)V[I+1]* frac);
		tempOut >>= 16;
		return (int32_t)tempOut;


//		int32_t term1 = ((V[I] ) * ((0xffff - frac)>>3))>>13;
	//	int32_t term2 = ((V[I+1] ) * (( frac)>>3))>>13;

//		return term1 + term2;
	}

	uint32_t ULERP(uint32_t *V, int total, int fade)
	{
		int T = fade * total;
		unsigned char frac = T & 0xff;
		if (frac && (frac < 255)) frac += 1;
		int I = T >> 8;
		return ((V[I] >> 8) *(255 - frac) + (V[I + 1] >> 8) * frac);
		

//		return ((V[I] >> 8) *(255 - frac) + (V[I + 1] >> 8) * frac);
	}
	
	uint32_t ULERP16(uint32_t *V, int total, int fade)
	{


		int T = fade * total;
		unsigned short frac = T & 0xffff;
		if (frac && (frac < 0xffff)) frac += 1;
		int I = T >> 16;

		uint64_t tempOut;
		tempOut = ((int64_t)V[I] * ((0xffff - frac)));
		tempOut += ((int64_t)V[I + 1] * frac);
		tempOut >>= 16;
		return (uint32_t)tempOut;


		uint32_t term1 = ((V[I] >> 3) * ((0xffff - frac) >> 3)) >> 10;
		uint32_t term2 = ((V[I + 1] >> 3) * ((frac) >> 3)) >> 10;

		return term1 + term2;
	}

	int32_t FillBasicShapes(uint32_t phase, int mod, Shapes_t *Shapes, ShapeCompensationVals_t *Comp)
	{		
		Shapes->Sine = Sine(phase);
		Shapes->Saw = SawTooth(phase);
		Shapes->Tri = Triangle(phase);
		Shapes->Pulse = Pulse(phase);		
		int32_t PreComp = LERP(&Shapes->Sine, 3, mod);		
		int32_t PostComp32 = ((PreComp - Comp->min) / (Comp->mul >> 20))<<10;
		return PostComp32;
	}

	int32_t BasicShapes(uint32_t phase, int mod, ShapeCompensationVals_t *Comp)
	{
		Shapes_t Shapes;
		return FillBasicShapes(phase, mod, &Shapes, Comp);		
	}

	
	int32_t UncompensatedBasicShapes(uint32_t phase, int mod)
	{
		Shapes_t Shapes;
		Shapes.Sine = Sine(phase);
		Shapes.Saw = SawTooth(phase);
		Shapes.Tri = Triangle(phase);
		Shapes.Pulse = Pulse(phase);
		int32_t PreComp = LERP(&Shapes.Sine, 3, mod);

		return PreComp;
	}


#ifdef __cplusplus
}
#endif
