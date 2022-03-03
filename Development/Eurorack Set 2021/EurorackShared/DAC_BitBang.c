#include "Cpu.h"

#define DAC_CS 5 // on port A
#define DAC_DATA 7 // on port A

#define DAC_CLOCK 0 // on port B

#define NOP __asm volatile ("nop");


void CS_SET() {GPIOA_PDOR |= (1<<DAC_CS);NOP;NOP;NOP;NOP;};
void CS_CLR() {GPIOA_PDOR &= ~(1<<DAC_CS);NOP;NOP;NOP;NOP;NOP;};
void CLOCK_SET() {GPIOB_PDOR |= (1<<DAC_CLOCK);NOP;NOP;NOP;NOP;};
void CLOCK_CLR() {GPIOB_PDOR &= ~(1<<DAC_CLOCK);NOP;NOP;NOP;NOP;};

void DATA_SET()
{
	GPIOA_PDOR |= (1<<DAC_DATA);
	NOP;NOP;NOP;NOP;
};
void DATA_CLR() {GPIOA_PDOR &= ~(1<<DAC_DATA);NOP;NOP;NOP;NOP;};

void DACBITBANG_InitHardware()
{
	PORTA_PCR5 = PORT_PCR_MUX(0x01);
	PORTA_PCR7 = PORT_PCR_MUX(0x01);

	PORTB_PCR0 = PORT_PCR_MUX(0x01);

	GPIOA_PDDR |= (1 << DAC_CS) | (1 << DAC_DATA);
	GPIOB_PDDR |= (1 << DAC_CLOCK);
}

void SendByte(uint8_t val)
{
	for(int i =7;i>=0;i--)
	{
	CLOCK_CLR(); if ((val & (1<<i)) == (1<<i)){DATA_SET();} else {DATA_CLR();}CLOCK_SET();
	}
}

void DACBITBANG_Update(uint16_t val1, uint16_t val2)
{

	const int shutdown1 = 0;
	const int gain1 = 0;
	const int shutdown2 = 0;
	const int gain2 = 0;

	unsigned int command;

	
	command = 0x0000;
	command |= shutdown1 ? 0x0000 : 0x1000;
	command |= gain1 ? 0x0000 : 0x2000;
	command |= (val1 & 0x0FFF);
	CS_CLR();
	SendByte(command >> 8);
	SendByte(command & 0xff);
	CS_SET();

	command = 0x8000;
	command |= shutdown2 ? 0x0000 : 0x1000;
	command |= gain2 ? 0x0000 : 0x2000;
	command |= (val2 & 0x0FFF);
	CS_CLR();
	SendByte(command >> 8);
	SendByte(command & 0xff);
	CS_SET();
}
