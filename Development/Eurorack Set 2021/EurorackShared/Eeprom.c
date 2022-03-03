#include "EE241.h"

unsigned char EE24_ReadByte(int address)
{
	unsigned char out = 0;
	EE241_ReadByte(address, &out);
	return out;
}


void EE24_WriteByte(unsigned short address, unsigned char value)
{
	EE241_WriteByte(address, value);
}

#ifdef OLDVERSION

#include "CI2C1.h"
#include "WAIT1.h"

extern void ShiftOut();

uint8_t hi(int address)
{
	return (uint8_t)((address) >> 8);
}

uint8_t lo(int address)
{
	return (uint8_t)((address & 0xFF));
}

uint8_t dev(int address)
{
#define address_24LC16B 0b1010

	return (uint8_t)((address_24LC16B << 3) | ((hi(address)) & 0x07));
}

volatile int i2csending = 0;
volatile int i2creceiving = 0;
volatile int i2csomething = 0;

unsigned char combuffer[2];

void EE24_WriteByte(unsigned short address, unsigned char value)
{
	//EE241_WriteByte(address, value);
	combuffer[0] = lo(address);
	combuffer[1] = value;
	i2csending = 0;

	byte devaddr = dev(address);
	int i = 0;

	CI2C1_SelectSlaveDevice(CI2C1_DeviceData, LDD_I2C_ADDRTYPE_7BITS, devaddr);
	i2csending = 1;
	CI2C1_MasterSendBlock(CI2C1_DeviceData, combuffer, 2, LDD_I2C_SEND_STOP);
	while (i2csending == 1)
	{
		CI2C1_MasterSendBlock(CI2C1_DeviceData, combuffer, 1, LDD_I2C_SEND_STOP);
		WAIT1_Waitms(1);
		ShiftOut();
	};

	i2csending = 0;
}


byte EE24_ReadByte(unsigned short address)
{
	//	byte out;
	//	EE241_ReadByte(address, &out);
	//return out;

	unsigned char com[1] = {lo(address)};
	unsigned char devaddr = dev(address);
	CI2C1_SelectSlaveDevice(CI2C1_DeviceData, LDD_I2C_ADDRTYPE_7BITS, devaddr);
	i2csending = 1;
	CI2C1_MasterSendBlock(CI2C1_DeviceData, com, 1, LDD_I2C_SEND_STOP);
	while (i2csending == 1)
	{
	}
	i2csomething = 0;
	ShiftOut();

	byte out;
	i2creceiving = 1;
	CI2C1_MasterReceiveBlock(CI2C1_DeviceData, &out, 1, LDD_I2C_SEND_STOP);
	while (i2creceiving == 1)
	{
	};
	i2csomething = 1;
	ShiftOut();

	return out;
}

void EE24_ReadBlock(unsigned short address, unsigned char *out, int len)
{
	//EE241_ReadBlock(address, out, len);
	for (int i = 0; i < len; i++)
	{
		out[i] = EE24_ReadByte(address++);
	}

	/*
	byte com[1] = {lo(address)};
	byte devaddr = dev(address);
	CI2C1_Init(CI2C1_DeviceData);
	CI2C1_SelectSlaveDevice(CI2C1_DeviceData, LDD_I2C_ADDRTYPE_7BITS, devaddr);
	i2csending = 1;
	CI2C1_MasterSendBlock(CI2C1_DeviceData, com, 1, LDD_I2C_NO_SEND_STOP);
	while (i2csending == 1) {}


	i2creceiving = 1;
	CI2C1_MasterReceiveBlock(CI2C1_DeviceData,out, len, LDD_I2C_SEND_STOP);
	while (i2creceiving == 1) {};

	 */
}


#endif

void EE24_ReadBlock(unsigned short address, unsigned char *out, int len)
{
	//EE241_ReadBlock(address, out, len);
	for (int i = 0; i < len; i++)
	{
		out[i] = EE24_ReadByte(address++);
	}
}

void EE24_WriteBlock(unsigned short address, unsigned char *data, int len)
{
	//EE241_WriteBlock(address, data, len);

	for (int i = 0; i < len; i++)
	{

		EE24_WriteByte(address++, data[i]);
		//	WAIT1_Waitms(5);
	}
}
