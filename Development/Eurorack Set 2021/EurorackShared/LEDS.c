/*
 * LEDS.c
 *
 *  Created on: 6 jan. 2017
 *      Author: Stijn
 */
#include "Cpu.h"

#define LED_LATCH 5
#define LED_CLOCK 6
#define LED_DATA 7

inline void LED_LATCH_SET() {GPIOB_PDOR |= (1<<LED_LATCH);};
inline void LED_LATCH_CLR() {GPIOB_PDOR &= ~(1<<LED_LATCH);};
inline void LED_CLOCK_SET() {GPIOB_PDOR |= (1<<LED_CLOCK);};
inline void LED_CLOCK_CLR() {GPIOB_PDOR &= ~(1<<LED_CLOCK);};
inline void LED_DATA_SET() {GPIOB_PDOR |= (1<<LED_DATA);};
inline void LED_DATA_CLR() {GPIOB_PDOR &= ~(1<<LED_DATA);};

inline void LEDS_InitHardware()
{
	PORTB_PCR5 = PORT_PCR_MUX(0x01);
	PORTB_PCR6 = PORT_PCR_MUX(0x01);
	PORTB_PCR7 = PORT_PCR_MUX(0x01);

	GPIOB_PDDR |= (1 << LED_LATCH) | (1 << LED_CLOCK) | (1 << LED_DATA);
}

uint8_t PWM = 0;

inline void LEDS_Update(uint8_t *leds, int count, uint8_t *gates, int gatecount)
{
	PWM+= 16;

	LED_LATCH_CLR();
	for (int i = 0; i < gatecount; i++)
	{
		if (gates[i] > 0)
		{
			LED_DATA_CLR();
		}
		else
		{
			LED_DATA_SET();
		}
		LED_CLOCK_CLR();
		LED_CLOCK_SET();
	}

	for (int i = 0; i < count; i++)
	{
		if (leds[i]>0 && leds[i] >= PWM)
		{
			LED_DATA_SET();
		}
		else
		{
			LED_DATA_CLR();
		}
		LED_CLOCK_CLR();
		LED_CLOCK_SET();
	}

	LED_LATCH_SET();
}
