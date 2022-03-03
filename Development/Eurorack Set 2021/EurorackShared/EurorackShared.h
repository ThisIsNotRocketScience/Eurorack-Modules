#ifndef EURORACKCALIBRATION
#define EURORACKCALIBRATION

#include <stdint.h>

#define EURORACK_MAX_ADC 6
#define EURORACK_MAX_DAC 2

struct EuroRack_ADC_Calibration
{
	int16_t offset;
	int16_t scale;
};

struct EuroRack_DAC_Calibration
{
	int16_t volt_3;
	int16_t volt_1;
};

struct EuroRack_Calibration
{
	struct EuroRack_ADC_Calibration ADC[6];
	struct EuroRack_DAC_Calibration DAC[2];
};


struct denoise_state_t
{
	int counter;
	int down;
	unsigned char pressed:4;
	unsigned char released:4;
	int longpressed;
	int lastcounter;
};

struct EURORACK_SVF
{
	uint16_t Cutoff;
	uint32_t Resonance;
	int32_t lo;
	int32_t mid;
	int32_t hi;
};

typedef struct Shapes_t
{
	int32_t Sine;
	int32_t Saw;
	int32_t Tri;
	int32_t Pulse;
} Shapes_t;

typedef struct ShapeCompensationVals_t
{
	int32_t min;
	int32_t mul;
} ShapeCompensationVals_t;

typedef struct SteppedResult_t
{
	uint8_t index;
	uint8_t fractional;
} SteppedResult_t;

typedef struct SteppedResult16_t
{
	uint8_t index;
	uint16_t fractional;
} SteppedResult16_t;

#define COMPENSATION_NOMIN 0
	#define COMPENSATION_NOMUL 0xffffffff;

#ifdef __cplusplus
extern "C"
{
#endif


	void EuroRack_InitCalibration();
	uint8_t CalibratedADC(int adcchannel, uint32_t Input);
	uint16_t CalibratedDAC(int dacchannel, int32_t Input);
	void ChangeDACCalibration(int dacchannel, int low, int high);
	int EuroRack_ValidateCalibration();

	void denoise(int sw_down, struct denoise_state_t *state);
	int islongpress(struct denoise_state_t *state);
	int pressed(struct denoise_state_t *state);

	/// A sine approximation via a fourth-order cosine approx.
	/// @param x   angle (with 2^15 units/circle)
	/// @return     Sine value (Q12)
	int32_t isin_S4(int32_t x);
	int32_t LERP(int32_t *V, int total, int fade);
	uint32_t ULERP(uint32_t *V, int total, int fade);


	int32_t LERP16(int32_t *V, int total, int fade);
	uint32_t ULERP16(uint32_t *V, int total, int fade);

	int32_t Sine(uint32_t phase);
	int32_t Cosine(uint32_t phase);
	int32_t SawTooth(uint32_t phase);
	int32_t Pulse(uint32_t phase);
	int32_t Triangle(uint32_t phase);
	void Wobbler2_CalculateCompensation(ShapeCompensationVals_t *Comp, int mod);
	int32_t BasicShapes(uint32_t phase, int mod, ShapeCompensationVals_t *Comp);
	int32_t UncompensatedBasicShapes(uint32_t phase, int mod);
	int32_t FillBasicShapes(uint32_t phase, int mod, Shapes_t *Shapes, ShapeCompensationVals_t *Comp);

	void ResetSVF(struct EURORACK_SVF *filt);
	void SetSVF(struct EURORACK_SVF *filt, uint16_t cut, uint16_t res);
	void ProcessSVF(struct EURORACK_SVF *filt, uint32_t RR);
	void GetSteppedResult(uint16_t param, uint8_t steps, SteppedResult_t *out);
	void GetSteppedResult16(uint32_t param, uint8_t steps, SteppedResult16_t *out);
	uint32_t StartCounting();
	uint32_t EndCount(uint32_t start);

#ifdef __cplusplus
}
#endif

/* DWT (Data Watchpoint and Trace) registers, only exists on ARM Cortex with a DWT unit */
#define KIN1_DWT_CONTROL             (*((volatile uint32_t*)0xE0001000))
/*!< DWT Control register */
#define KIN1_DWT_CYCCNTENA_BIT       (1UL<<0)
/*!< CYCCNTENA bit in DWT_CONTROL register */
#define KIN1_DWT_CYCCNT              (*((volatile uint32_t*)0xE0001004))
/*!< DWT Cycle Counter register */
#define KIN1_DEMCR                   (*((volatile uint32_t*)0xE000EDFC))
/*!< DEMCR: Debug Exception and Monitor Control Register */
#define KIN1_TRCENA_BIT              (1UL<<24)
/*!< Trace enable bit in DEMCR register */

#define KIN1_InitCycleCounter() \
  KIN1_DEMCR |= KIN1_TRCENA_BIT
/*!< TRCENA: Enable trace and debug block DEMCR (Debug Exception and Monitor Control Register */

#define KIN1_ResetCycleCounter() \
  KIN1_DWT_CYCCNT = 0
/*!< Reset cycle counter */

#define KIN1_EnableCycleCounter() \
  KIN1_DWT_CONTROL |= KIN1_DWT_CYCCNTENA_BIT
/*!< Enable cycle counter */

#define KIN1_DisableCycleCounter() \
  KIN1_DWT_CONTROL &= ~KIN1_DWT_CYCCNTENA_BIT
/*!< Disable cycle counter */

#define KIN1_GetCycleCounter() \
  KIN1_DWT_CYCCNT
/*!< Read cycle counter register */


#endif
