#include "DAC.h"
#include "SM1.h"
#include "DACSEL.h"

volatile unsigned char DACSENDDONE = 1;
void DAC_Done()
{
	DACSEL_SetVal(0);
	DACSENDDONE = 1;
}
const int shutdown1 = 0;
const int gain1 = 0;
const int shutdown2 = 0	;
const int gain2 = 0;
unsigned char coms[2];

void DAC_Write(int channel, int value)
{




	if (DACSENDDONE == 0)
	{
		return;
	}
	DACSEL_ClrVal(0); // SetVal done in interrupt handler

	unsigned int command;
	if(channel == 1)
	{
		command = 0x0000;
		command |= shutdown1 ? 0x0000 : 0x1000;
		command |= gain1 ? 0x0000 : 0x2000;
		command |= (value & 0x0FFF);
		coms[0] =  command >> 8;
		coms[1] = command &0xff;
		DACSENDDONE  = 0;
		SM1_SendBlock(SM1_DeviceData, coms, 2 );
	}
	else
	{
		command = 0x8000;
		command |= shutdown2 ? 0x0000 : 0x1000;
		command |= gain2 ? 0x0000 : 0x2000;
		command |= (value & 0x0FFF);
		coms[0] =  command >> 8;
		coms[1] = command &0xff;
		DACSENDDONE  = 0;
		SM1_SendBlock(SM1_DeviceData, coms, 2 );

	}
	//WAIT1_Wait100Cycles();

}
