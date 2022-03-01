#include "Wobbler2.h"

#define MAKESPEED(x) ((int32_t)((x*x*x*x) * 65535.0f/10.0f))

int32_t SpeedAdjust[16] =
{
	MAKESPEED(0 / 15.0),
	MAKESPEED(1 / 15.0),
	MAKESPEED(2 / 15.0),
	MAKESPEED(3 / 15.0),
	MAKESPEED(4 / 15.0),
	MAKESPEED(5 / 15.0),
	MAKESPEED(6 / 15.0),
	MAKESPEED(7 / 15.0),
	MAKESPEED(8 / 15.0),
	MAKESPEED(9 / 15.0),
	MAKESPEED(10 / 15.0),
	MAKESPEED(11 / 15.0),
	MAKESPEED(12 / 15.0),
	MAKESPEED(13 / 15.0),
	MAKESPEED(14 / 15.0),
	MAKESPEED(15 / 15.0)

};
#ifdef INTPENDULUM
#define MAXPENDULUMDECAYVALUE  (5 * 60 * WOBBLERSAMPLERATE * 0xffff)

void Wobbler2_InitIntPendulum(Wobbler2_PendulumInt_t *P, Wobbler2_LFO_t *W)
{
	fix16_t const NEGQUARTERPI = fix16_mul(fix16_pi, fix16_from_float(-0.25));
	P->A = 0;
	P->B = 0;
	P->Theta1 = NEGQUARTERPI;
	P->Theta2 = fix16_add(NEGQUARTERPI , fix16_mul(fix16_from_int(W->Phasing) , fix16_from_float(1.0f / 4096.0f)));
	P->d2Theta1 = 0;
	P->d2Theta2 = 0;
	P->dTheta1 = fix16_mul(fix16_from_int(W->Speed), fix16_from_float(-1.0f / ((float)0xffff)));
	P->dTheta2 = 0;

	P->dTheta1 = fix16_mod(P->dTheta1, fix16_mul(fix16_pi, fix16_from_int(2)));
	P->dTheta2 = fix16_mod(P->dTheta2, fix16_mul(fix16_pi, fix16_from_int(2)));



	P->dampcount = 0;
	P->dampmax = 1;
//	P->DecayEnvelopeCount = 1<<30;
};

#define FIX(x)((int32_t)((x)*65536.0f))

void Wobbler2_UpdateIntPendulumSettings(Wobbler2_PendulumInt_t *P, Wobbler2_LFO_t *W)
{
#define DAMPSTART 0.9995f
	P->Damping = fix16_add(FIX(DAMPSTART), fix16_mul(W->Mod , FIX((1.0f - DAMPSTART) / (float)(0xffff))));
	P->dTheta1 = fix16_add(P->dTheta1, fix16_mul(W->Phasing - W->LastPhasing, fix16_from_float(0.00001f)));
	//float spe = LERP9bit(SpeedAdjust, W->Speed)   * (1.0f / (float)65535);
	//	spe *= spe;
	P->g = fix16_mul(FIX( .2981f), fix16_add(FIX(0.001f), LERP9bit((unsigned long*)SpeedAdjust, W->Speed)));
	P->l1 = fix16_from_int(1);
	P->l2 = fix16_from_int(1);
	P->m1 = fix16_from_int(1);
	P->m2 = fix16_from_int(1);
	P->mu = fix16_from_int(2);// 1.0f + 1.0;// P->m1 / P->m2;

	//P->DecayEnvelopeAmt = ((1 << 27) - 1) / (1 + (Wobbler2_LFORange3(W->Mod, WOBBLERSAMPLERATE)));
};



void Wobbler2_DoublePendulumInt(Wobbler2_PendulumInt_t *P, int32_t feed)
{

	fix16_t const TAU = fix16_mul(fix16_pi, fix16_from_int(2));
	fix16_t const PI = fix16_pi;

	P->_2sub1 = fix16_sub(P->Theta2, P->Theta1);
	P->_1sub2 = fix16_sub(P->Theta1, P->Theta2);
	P->st1 = fix16_sin(P->Theta1);
	P->st2 = fix16_sin(P->Theta2);
	P->c1sub2 = fix16_cos(P->_1sub2);
	P->c1sub2SQUARED = fix16_mul(P->c1sub2, P->c1sub2);
	P->s1sub2 = fix16_sin(P->_1sub2);
	P->dTheta2SQUARED = fix16_mul(P->dTheta2, P->dTheta2);
	P->dTheta1SQUARED = fix16_mul(P->dTheta1, P->dTheta1);
	P->l1_x_dTheta1SQUARED = fix16_mul(P->l1 , P->dTheta1SQUARED);
	P->l2_x_dTheta2SQUARED = fix16_mul(P->l2 ,P->dTheta2SQUARED);

	fix16_t const T1a1 = fix16_mul(P->st2, P->c1sub2);
	fix16_t const T1a2 = fix16_mul(P->mu, P->st1);
	fix16_t const T1a = fix16_sub(T1a1 , T1a2);
	fix16_t const T1 = fix16_mul(P->g, T1a);
	fix16_t const T2b1 = fix16_mul(P->l1_x_dTheta1SQUARED, P->c1sub2);
	fix16_t const T2b = fix16_add(P->l2_x_dTheta2SQUARED , T2b1);
	fix16_t const T2 = fix16_mul(T2b, P->s1sub2);
	fix16_t const T3b =fix16_sub( P->mu , P->c1sub2SQUARED);
	fix16_t const T3 = fix16_mul(P->l1,T3b);
	fix16_t const T4 = fix16_mul(P->mu , fix16_mul(P->g , fix16_sub(fix16_mul(P->st1 , P->c1sub2), P->st2)));
	fix16_t const T5a2 = fix16_mul(P->l2_x_dTheta2SQUARED , P->c1sub2);
	fix16_t const T5a1 = fix16_mul(P->l1_x_dTheta1SQUARED, P->mu);
	fix16_t const T5a = fix16_add(T5a1 , T5a2);
	fix16_t const T5 = fix16_mul(T5a , P->s1sub2);
	fix16_t const T6b = fix16_sub(P->mu , P->c1sub2SQUARED);
	fix16_t const T6 = fix16_mul(P->l2, T6b);

	P->d2Theta1 = fix16_div(fix16_sub(T1 , T2) , T3);
	P->d2Theta1 += feed>>28;
	P->d2Theta2 = fix16_div(fix16_add(T4 , T5) , T6);
	
	P->dTheta1 = fix16_mul(P->dTheta1, P->Damping);
	P->dTheta2 = fix16_mul(P->dTheta2, P->Damping);

	fix16_t const DT = FIX(0.5f);
	P->dTheta1 = fix16_add(P->dTheta1,  fix16_mul(P->d2Theta1,DT));
	P->dTheta2 = fix16_add(P->dTheta2, fix16_mul(P->d2Theta2,DT));
	P->Theta1 = fix16_add(P->Theta1, fix16_mul(P->dTheta1,DT));
	P->Theta2 = fix16_add(P->Theta2, fix16_mul(P->dTheta2,DT));
	
	P->A = P->Theta1 * 0xffff;
	P->B = P->Theta2 * 0xffff;
	//P->As = P->Theta1 * ((float)(0xffff) / TAU);
	//P->Bs = P->Theta2 *  ((float)(0xffff) / TAU);
	P->Theta1 = fix16_mod(P->Theta1, PI);
	P->Theta2 = fix16_mod(P->Theta2, PI);

	P->As = fix16_mul(P->Theta1, FIX(((float)0xffff) / (3.1415f)));// *(P->DecayEnvelopeCount >> 17);
	
	P->Bs = fix16_mul(P->Theta2, FIX(((float)0xffff) / (3.1415f )));// *(P->DecayEnvelopeCount >> 17);
	
																					//P->Bs = P->Theta2 * (P->DecayEnvelopeCount >> 17);
	//P->As = P->DecayEnvelopeCount>>16;
	//P->As *= 0xffff;
	//P->Bs *= 0xffff;

};

#else

#define NEGQUARTERPI (float const)(-PI / 4.0f)
void Wobbler2_InitPendulum(Wobbler2_Pendulum_t *P, Wobbler2_LFO_t *W)
{
	P->A = 0;
	P->B = 0;
	P->Theta1 = NEGQUARTERPI;
	P->Theta2 = NEGQUARTERPI + W->Phasing * (1.0f / (float)4096);
	P->d2Theta1 = 0;
	P->d2Theta2 = 0;
	P->dTheta1 = W->Speed * (-1.0f / ((float)0xffff));;
	P->dTheta2 = 0;

	//Wobbler2_UpdatePendulumSettings(P, W);

}



void Wobbler2_UpdatePendulumSettings(Wobbler2_Pendulum_t *P, Wobbler2_LFO_t *W)
{
	P->Damping = 0.999 + (W->Mod * (0.00099999999 / (float)(0xffff)));
	P->dTheta1 += (W->Phasing - W->LastPhasing)*0.00001;
	float spe = LERP9bit(SpeedAdjust, W->Speed)   * (1.0f / (float)65535);
	
	P->g = .2981 *  (0.001 + spe);
	P->l1 = 1;
	P->l2 = 1;
	P->m1 = 1;
	P->m2 = 1;
	P->mu = 1.0f + 1.0;// P->m1 / P->m2;
}

float fsin(float P)
{
	//	P *= (float)(1 << 32);
	P *= (float)(1.0f / TAU);;
	P *= (float)0xffffffff;
	int32_t phase = (int)P;
	int32_t R = Sine(phase);
	return (float)R * (1.0 / (float)(1 << 30));
}


//P->d2Theta1 = (P->g*(st2*c1sub2 - P->mu*st1) - (P->l2*P->dTheta2*P->dTheta2 + P->l1*P->dTheta1*P->dTheta1*c1sub2)*s1sub2) / (P->l1*(P->mu - c1sub2*c1sub2));
//P->d2Theta2 = (P->mu*P->g*(st1*c1sub2 - st2) + (P->mu*P->l1*P->dTheta1*P->dTheta1 + P->l2*P->dTheta2*P->dTheta2*c1sub2)*s1sub2) / (P->l2*(P->mu - c1sub2*c1sub2));

void Wobbler2_DoublePendulum(Wobbler2_Pendulum_t *P, float const DT)
{
	//while (P->Theta1 < 0) { P->Theta1 += TAU; P->Theta2 += TAU; }
	P->_2sub1 = P->Theta2 - P->Theta1;
	P->_1sub2 = P->Theta1 - P->Theta2;
	P->st1 = fsin(P->Theta1);
	P->st2 = fsin(P->Theta2);
	P->c1sub2 = cos(P->_1sub2);
	P->c1sub2SQUARED = P->c1sub2 * P->c1sub2;
	P->s1sub2 = fsin(P->_1sub2);
	P->dTheta2SQUARED = P->dTheta2 * P->dTheta2;
	P->dTheta1SQUARED = P->dTheta1 * P->dTheta1;
	P->l1_x_dTheta1SQUARED = P->l1 * P->dTheta1SQUARED;
	P->l2_x_dTheta2SQUARED = P->l2 * P->dTheta2SQUARED;

	float const T1a1 = P->st2 * P->c1sub2;
	float const T1a2 = P->mu * P->st1;
	float const T1a = T1a1 - T1a2;
	float const T1 = P->g * T1a;
	float const T2b1 = P->l1_x_dTheta1SQUARED * P->c1sub2;
	float const T2b = P->l2_x_dTheta2SQUARED + T2b1;
	float const T2 = T2b * P->s1sub2;
	float const T3b = P->mu - P->c1sub2SQUARED;
	float const T3 = P->l1*T3b;
	float const T4 = P->mu * P->g * (P->st1 * P->c1sub2 - P->st2);
	float const T5a2 = P->l2_x_dTheta2SQUARED * P->c1sub2;
	float const T5a1 = P->l1_x_dTheta1SQUARED * P->mu;
	float const T5a = T5a1 + T5a2;
	float const T5 = T5a * P->s1sub2;
	float const T6b = P->mu - P->c1sub2SQUARED;
	float const T6 = P->l2 * T6b;

	P->d2Theta1 = (T1 - T2) / (T3);
	P->d2Theta2 = (T4 + T5) / T6;
	P->dTheta1 *= P->Damping;
	P->dTheta2 *= P->Damping;
	P->dTheta1 += P->d2Theta1*DT;
	P->dTheta2 += P->d2Theta2*DT;
	P->Theta1 += P->dTheta1*DT;
	P->Theta2 += P->dTheta2*DT;
	P->A = P->Theta1 * 0xffff;
	P->B = P->Theta2 * 0xffff;
	P->As = P->Theta1 * ((float)(0xffff) / TAU);
	P->Bs = P->Theta2 *  ((float)(0xffff) / TAU);
	while (P->Theta1 > PI) P->Theta1 -= TAU;
	while (P->Theta2 > PI) P->Theta2 -= TAU;
	while (P->Theta1 < -PI) P->Theta1 += TAU;
	while (P->Theta2 < -PI) P->Theta2 += TAU;

	P->As *= 0xffff;
	P->Bs *= 0xffff;
}
#endif
