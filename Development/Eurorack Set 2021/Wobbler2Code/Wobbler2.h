#ifndef WOBBLER2
#define WOBBLER2
#define Wobbler2_GATECOUNTDOWN 20

#include <stdint.h>
#include "../EurorackShared/EURORACKSHARED.H"

typedef struct Wobbler2_Params
{
	int CalibNormal;
	int CalibPhased;
	unsigned char T;
} Wobbler2_Params;
	
typedef struct Wobbler2_Settings
{
	unsigned char SlowSpeedMult;
} Wobbler2_Settings;

#define Wobbler2_ATTACK 2
#define Wobbler2_IDLE 3
#define Wobbler2_RELEASE 4
#define WOBBLER_FIXBITS 24
#define WOBBLERSAMPLERATE 2500

typedef struct Wobbler2_RandomGen
{
	long RandomMemory;
} Wobbler2_RandomGen;

typedef struct Wobbler2_SVF_Coeffs
{
	uint16_t cutoff;
	uint16_t wet;
	uint16_t dry;

} Wobbler2_SVF_Coeffs;

typedef struct Wobbler2_SVFstruct
{
	int32_t low;
	int32_t mid;
	int32_t high;

} Wobbler2_SVFstruct;

typedef struct Wobbler2_LFO_SNH_t
{
	uint32_t countdown;
	
	uint32_t lastval1;
	uint32_t lastval2;
	uint32_t lastval3;
	uint32_t lastval4;
	uint32_t normalout;
	uint32_t delayedout;
	uint32_t normalout_quantized;
	uint32_t phasedout_quantized;

	
	uint8_t lastseg1;
	uint8_t lastseg2;
	uint8_t segindex;
	
	uint16_t segbuffer[32];
	Wobbler2_RandomGen random;	
	Wobbler2_SVFstruct F1, F2, F3, F4;

	uint8_t triggerhappened;
} Wobbler2_LFO_SNH_t;


#include "Pendulum.h"

//float GetInterpolatedResultFloat(float *table, SteppedResult_t *inp);
int32_t WobGetInterpolatedResultInt(int32_t *table, SteppedResult_t *inp);

enum
{
	WOBBLER2_FANCYENV_ATTACK,
	WOBBLER2_FANCYENV_DECAY,
	WOBBLER2_FANCYENV_IDLE
};


typedef struct Wobbler2_FancyEnv_t
{
	int32_t current;
	int32_t currentcurved;
	int32_t state;
	int32_t endlevel;
} Wobbler2_FancyEnv_t;

typedef struct Wobbler2_LFO_t
{
	uint16_t Speed;
	uint16_t Shape;
	uint16_t Mod;
	uint16_t Phasing;
	uint16_t LastPhasing;

	uint16_t SpeedOrig;
	
	int16_t Amount1;
	int16_t Amount2;

	uint32_t Phase1;
	uint32_t SlomoPhase1;
	uint32_t OldPhase1;
	uint32_t SlomoPhase2;
	uint32_t Phase2;
	uint32_t Phase2Rev;
	int32_t OldPhase2;
	uint32_t PhasedShift;

	int32_t OutputPhased;
	int32_t Output;
	int32_t OutputPhasedMin;
	int32_t OutputMin;
	int32_t OutputPhasedMax;
	int32_t OutputMax;

	int32_t OutputPhasedPreCalib;
	int32_t OutputPreCalib;
	
	unsigned char Gate[2];
	 int Led[2][9];
	unsigned char ModeLed[5];
	unsigned char TriggerLed;
	uint32_t T;
	uint32_t PhasedCountdown;
	//int32_t EnvelopeVal;
	//uint8_t EnvelopeState;
	Wobbler2_FancyEnv_t FancyEnv;
	Wobbler2_SVF_Coeffs SVFCoeffs;
	Wobbler2_LFO_SNH_t SNH;
#ifdef INTPENDULUM
	Wobbler2_PendulumInt_t Pendulum;
#else
	Wobbler2_Pendulum_t Pendulum;

#endif
	int synctime[3];
	uint32_t SyncedPeriodTime;
	uint32_t SyncDP;
	int timesincesync;
	int extsync;
	int syncindex;
#ifdef SPRINGMASS
	SpringMassSystem_t PendulumSystem;
#endif
	SteppedResult_t ShapeStepped;
	Shapes_t BasicShapesA;
	Shapes_t BasicShapesB;
	Shapes_t SlomoBasicShapesA;
	Shapes_t SlomoBasicShapesB;

	int32_t OutputsNormal[7];
	int32_t OutputsPhased[7];

	ShapeCompensationVals_t CompensationVals;

	int32_t CalibNormal;
	int32_t CalibPhased;

	int32_t fastmode;
} Wobbler2_LFO_t;



#ifdef __cplusplus
extern "C"
{
#endif

	extern void Wobbler2_GetSteppedResult(uint16_t param, uint8_t steps, SteppedResult_t *out);

	extern int Wobbler2_Get(Wobbler2_LFO_t *LFO,  Wobbler2_Params *Params);
	extern void Wobbler2_Init( Wobbler2_LFO_t *LFO);
	extern unsigned long LERP9bit(unsigned long *V, int fade);


#ifdef INTPENDULUM
	extern void Wobbler2_InitIntPendulum(Wobbler2_PendulumInt_t *P, Wobbler2_LFO_t *W);
	extern void Wobbler2_UpdateIntPendulumSettings(Wobbler2_PendulumInt_t *P, Wobbler2_LFO_t *W);
	extern void Wobbler2_DoublePendulumInt(Wobbler2_PendulumInt_t *P, int32_t feed);
#else
	extern void Wobbler2_UpdatePendulumSettings(Wobbler2_Pendulum_t *P, Wobbler2_LFO_t *W);
	extern void Wobbler2_InitPendulum( Wobbler2_Pendulum_t *Pendulum,Wobbler2_LFO_t *LFO);
	extern void Wobbler2_DoublePendulum(Wobbler2_Pendulum_t *P, float const DT, int32_t feed);
#endif
	extern void Wobbler2_Trigger(Wobbler2_LFO_t *LFO, unsigned char N, Wobbler2_Params *Params);
	extern void Wobbler2_SyncPulse(Wobbler2_LFO_t *LFO);
	extern void Wobbler2_SwitchMode(Wobbler2_LFO_t *LFO);
	extern void Wobbler2_LoadSettings( Wobbler2_Settings *settings, Wobbler2_Params *params);
	extern void Wobbler2_ValidateParams(Wobbler2_Params *params);
	extern void Wobbler2_StartTwang(Wobbler2_LFO_t *LFO);
	extern void Wobbler2_DoLeds(Wobbler2_LFO_t *LFO);
	extern unsigned long Wobbler2_MakeFreq(int input);
	extern unsigned long Wobbler2_LFORange2(int32_t V, int32_t SR);
	extern unsigned long Wobbler2_LFORange3(int32_t V, int32_t SR);
	extern void Wobbler2_FancyEnv_Init(Wobbler2_FancyEnv_t *env);
	extern void Wobbler2_FancyEnv_Trigger(Wobbler2_FancyEnv_t *env);
	extern int32_t Wobbler2_FancyEnv_Update(Wobbler2_FancyEnv_t *env, int32_t param);
	extern uint16_t Wobbler2_SkipThe1Percent(uint16_t input);
	extern int32_t LERP(int32_t *V, int total, int fade);
#ifdef __cplusplus
}
#endif

#endif
