#pragma once

#define INTPENDULUM

#ifndef PENDULUMINCLUDED
#define PENDULUMINCLUDED

#ifndef INTPENDULUM
typedef struct Wobbler2_Pendulum_t
{
	int32_t A;
	int32_t B;
	float m1;
	float m2;

	float l1;
	float l2;
	float g;
	float mu;


	float _2sub1;
	float _1sub2;
	float c1sub2;
	float c1sub2SQUARED;
	float s1sub2;
	//float T1a1, T1a2, T1a, T1;
	//float T2b1, T2b, T2;
	//float T3b, T3, T4;
	//float T5a1, T5a2, T5a, T5;
	//float T6b, T6;

	float st1;
	float st2;
	float dTheta1SQUARED;
	float dTheta2SQUARED;
	float l1_x_dTheta1SQUARED;
	float l2_x_dTheta2SQUARED;

	float Theta1;
	float Theta2;
	float d2Theta1;
	float d2Theta2;
	float dTheta1;
	float dTheta2;

	float Damping;

	int32_t As;
	int32_t Bs;

} Wobbler2_Pendulum_t;
#endif

#ifdef INTPENDULUM
typedef struct ivec_t
{
	int32_t X; //7.24
	int32_t Y; //7.24
} ivec_t;


#include "../libfixmath/libfixmath/fixmath.h"


typedef struct Wobbler2_PendulumInt_t
{
	fix16_t A;
	fix16_t B;
	fix16_t m1;
	fix16_t m2;

	fix16_t l1;
	fix16_t l2;
	fix16_t g;
	fix16_t mu;


	fix16_t _2sub1;
	fix16_t _1sub2;
	fix16_t c1sub2;
	fix16_t c1sub2SQUARED;
	fix16_t s1sub2;
	//fix16_t T1a1, T1a2, T1a, T1;
	//fix16_t T2b1, T2b, T2;
	//fix16_t T3b, T3, T4;
	//fix16_t T5a1, T5a2, T5a, T5;
	//fix16_t T6b, T6;

	fix16_t st1;
	fix16_t st2;
	fix16_t dTheta1SQUARED;
	fix16_t dTheta2SQUARED;
	fix16_t l1_x_dTheta1SQUARED;
	fix16_t l2_x_dTheta2SQUARED;

	fix16_t Theta1;
	fix16_t Theta2;
	fix16_t d2Theta1;
	fix16_t d2Theta2;
	fix16_t  dTheta1;
	fix16_t dTheta2;

	fix16_t	Damping;
	
	int32_t As;
	int32_t Bs;
	int32_t dampcount;
	int32_t dampmax;
} Wobbler2_PendulumInt_t;

#ifdef SPRINGMASS
typedef struct Mass_t
{
	ivec_t Pos;
	ivec_t PrevPos;
	ivec_t Speed;
	ivec_t Force;
	int fixed;
	int32_t mass; //7.24
} Mass_t;

typedef struct Spring_t
{
	int32_t K; // 7.24

	unsigned char A;
	unsigned char B;

	int32_t RestLength; // 7.24
} Spring_t;

typedef struct SpringMassSystem_t
{
	int SpringCount;
	int MassCount;
	Spring_t Springs[4];
	Mass_t Masses[5];
} SpringMassSystem_t;

#endif
#endif

#endif
