#pragma once

#pragma once

#include <stdint.h>
#include <stdio.h>

class LedState
{
public:
	int Brigthness;
};
class GateState
{
public:
	void Set();
	int state;
};
enum
{
	Button_Unknown,
	Button_Up,
	Button_Down,
	Button_Pressed,
	Button_Released
};

class ButtonState
{
public:
	int ID;
	ButtonState()
	{
		static int thecount  =0;
		ID = thecount++;
		state = 0;
		statecount = 0;
		laststate = 0;
	}

	int statecount;
	int state;
	int laststate;
	int Update()
	{
		if (state != laststate)
		{
			laststate = state;
			statecount = 0;
//			printf("button %d statechange %d\n", ID, state);
		}
		else
		{
			statecount++;
			if (statecount > 11) statecount = 11;
			if (statecount == 10)
			{

				return state ? Button_Pressed : Button_Released;
			}
		}
		return statecount == 11 ? (state ? Button_Down : Button_Up) : Button_Unknown;
	}
};
enum
{
	LooperLed_1_1,
	LooperLed_2_2,
	LooperLed_3_4,
	LooperLed_4_8,
	LooperLed_5_16,
	LooperLed_6_32,
	LooperLed_7_loop,
	LooperLed_8_reset,
	LooperLed_Rec,
	LooperLed_Start,
	LooperLed_Arm,
	__LooperLed_Count
};

enum
{
	ModuleLed_Clock,
	ModuleLed_Reset,
	ModuleLed_Shift,
	__ModuleLed_Count
};

enum
{
	LooperButton_1_1,
	LooperButton_2_2,
	LooperButton_3_4,
	LooperButton_4_8,
	LooperButton_5_16,
	LooperButton_6_32,
	LooperButton_7_loop,
	LooperButton_8_reset,
	LooperButton_Start,
	LooperButton_Arm,
	LooperButton_ArmGate,
	LooperButton_StartGate,
	__LooperButton_Count
};

enum
{
	ModuleButton_Shift,
	ModuleButton_Clock,
	ModuleButton_Reset,

	__ModuleButton_Count
};

#define FADETIME 30

class Looper
{
public:
	bool ResetOnReset;
	bool AutoLoop;
	LedState Leds[__LooperLed_Count];
	int16_t Balance;
	ButtonState Buttons[__LooperButton_Count];
	
	float* Buffers[2];

	int recordingbuffer;
	int playingbuffer;
	int playpos;
	int recordpos;
	float inlevel; 
	float outlevel;

	int recordedsamplesinbuffer;
	int looplength;
	int32_t  recordingbufferlength;
	int32_t  playbufferlength;
	
	int playing;
	int playvolume;
	bool armed;
	bool recording;
	int BeatLength;
	int startbeat;

	void SetBuffers(float *b1, float *b2)
	{
		Buffers[0] = b1;//new int32_t[256 * 1024];
		Buffers[1] = b2;//new int32_t[256 * 1024];
		for(int i =0 ;i<256*1024;i++) Buffers[0][i] = 0;
		for(int i =0 ;i<256*1024;i++) Buffers[1][i] = 0;
	}

	Looper()
	{
		startbeat = 0;
		BeatLength = 8;
		TicksPerBeat = 24;
		SamplesPerBeat = (44100 * 60) / 140;

		playbufferlength = SamplesPerBeat * BeatLength;
		AutoLoop = true;
		ResetOnReset = false;
		fixdc = 0;
		inlevel = 1.0f;
		armed = false;
		outlevel = 1.0f;
		playing = true;
		FadeOutStart = playpos - FADETIME;
		sampletimerec = 0;
		for (int i = 0; i < 10; i++) sampletimebetweenclocks[i] = SamplesPerBeat;
		recordingbufferlength = SamplesPerBeat * BeatLength;
		samplecountsinceclock = 0;
		JumpBeat = -1;
		overdub = false;
		ledT = 0;

	}

	int FadeOutStart;
	int CurrentTick;
	int TicksPerBeat;
	int SamplesPerBeat;

	void Reset()
	{
		CurrentTick = -1;
		if (playing)
		{
			if (ResetOnReset)
			{
				FadeOutStart = playpos;
				playpos = 0;
				
			}
		}
	}
	int sampletimebetweenclocks[10];
	int sampletimerec;
	int samplecountsinceclock;
	int RecordingTick;
bool overdub;
	void Clock()
	{
		if (recording)
		{
			RecordingTick++;
			if (RecordingTick== BeatLength)
			{

			}
		}
		sampletimerec = (sampletimerec + 1) % 10;
		sampletimebetweenclocks[sampletimerec] = samplecountsinceclock;
		samplecountsinceclock = 0;

		for (int i = 0; i < 10; i++) SamplesPerBeat += sampletimebetweenclocks[i];
		SamplesPerBeat /= 10;

		CurrentTick++;
		if (armed && (CurrentTick % BeatLength == 0))
		{
			armed = false;
			playing = false;
			faderec = false;
			RecordingTick = 0;
			recording = true;
			overdub = false;
			recordpos = 0;
//			printf("clock triggered recording after arm!\n");

			recordingbufferlength = SamplesPerBeat * BeatLength;
		}
		else
		{
			if (JumpBeat > 0)
			{
				playpos = (playbufferlength / 8) * JumpBeat;
				JumpBeat = -1;
			}

		}
	}

	int ClockedBlink;
	void SetLength(int B)
	{
		if (B == BeatLength) return;
		BeatLength = B;
	}

	void SetReset(bool newval)
	{
		ResetOnReset = newval;
	}

	void SetLoop(bool newval)
	{
		AutoLoop = newval;
	}

	void ButtonChange(int button, int state)
	{
		Buttons[button].state = state;
	}
	int JumpBeat;
	void StartChanged(int newstart)
	{
		if (playing) JumpBeat = newstart;
		startbeat = newstart;		
	}

	void UpdateButtons(bool Shifted)
	{
		if (Shifted)
		{
			if (Buttons[LooperButton_1_1].Update() == Button_Pressed) SetLength(1);
			if (Buttons[LooperButton_2_2].Update() == Button_Pressed) SetLength(2);
			if (Buttons[LooperButton_3_4].Update() == Button_Pressed) SetLength(4);
			if (Buttons[LooperButton_4_8].Update() == Button_Pressed) SetLength(8);
			if (Buttons[LooperButton_5_16].Update() == Button_Pressed) SetLength(16);
			if (Buttons[LooperButton_6_32].Update() == Button_Pressed) SetLength(32);
			if (Buttons[LooperButton_7_loop].Update() == Button_Pressed) SetLoop(AutoLoop ? false : true);
			if (Buttons[LooperButton_8_reset].Update() == Button_Pressed) SetReset(ResetOnReset ? false : true);
			
			if (Buttons[LooperButton_Arm].Update() == Button_Pressed)
					{
						if (overdub)
						{
							overdub = false;
						}
						else
						{
							overdub = true;
						}
					}

		}
		else
		{
			if (Buttons[LooperButton_1_1].Update() == Button_Pressed) StartChanged(0);
			if (Buttons[LooperButton_2_2].Update() == Button_Pressed) StartChanged(1);
			if (Buttons[LooperButton_3_4].Update() == Button_Pressed) StartChanged(2);
			if (Buttons[LooperButton_4_8].Update() == Button_Pressed) StartChanged(3);
			if (Buttons[LooperButton_5_16].Update() == Button_Pressed) StartChanged(4);
			if (Buttons[LooperButton_6_32].Update() == Button_Pressed) StartChanged(5);
			if (Buttons[LooperButton_7_loop].Update() == Button_Pressed) StartChanged(6);
			if (Buttons[LooperButton_8_reset].Update() == Button_Pressed) StartChanged(7);

			if (Buttons[LooperButton_Start].Update() == Button_Pressed)
			{
				if (playing)
				{
					playing = false;
					overdub = false;
				}
				else
				{
					playing = true;

				}

			}

			if (Buttons[LooperButton_StartGate].Update() == Button_Pressed)
			{
				playing = true;
			}


			if (Buttons[LooperButton_ArmGate].Update() == Button_Pressed)
			{
				if (!armed) armed = true;
			}

			if (Buttons[LooperButton_Arm].Update() == Button_Pressed)
			{
				if (armed)
				{
					armed = false;
				}
				else
				{
					armed = true;
				}
			}
		}
	}

	int ledT  ;
	void SetupLeds(bool Shift)
	{
		ledT++;
		int ClockedBlink = ((ledT++/1000)%2 == 0) ? 255:0;

		if (armed)
		{
			Leds[LooperLed_Arm].Brigthness = ClockedBlink;
		}
		else
		{
			Leds[LooperLed_Arm].Brigthness = 0;
		}

		if (recording)
		{
			Leds[LooperLed_Rec].Brigthness = 255;
		}
		else
		{
			Leds[LooperLed_Rec].Brigthness = 0;
		}

		//		if (playing)
			//	{
		Leds[LooperLed_Start].Brigthness = 255;
		//}

		if (Shift)
		{
			for (int i = LooperLed_1_1; i <= LooperLed_8_reset; i++) Leds[i].Brigthness = 0;

			switch (BeatLength)
			{
			case 1: Leds[LooperLed_1_1].Brigthness = 255; break;
			case 2:Leds[LooperLed_2_2].Brigthness = 255; break;
			case 4:Leds[LooperLed_3_4].Brigthness = 255; break;
			case 8:Leds[LooperLed_4_8].Brigthness = 255; break;
			case 16:Leds[LooperLed_5_16].Brigthness = 255; break;
			case 32:Leds[LooperLed_6_32].Brigthness = 255; break;
			}
			if (AutoLoop) Leds[LooperButton_7_loop].Brigthness = 255;
			if (ResetOnReset) Leds[LooperButton_8_reset].Brigthness = 255;
		}
		else
		{
			for (int i = LooperLed_1_1; i <= LooperLed_8_reset; i++) Leds[i].Brigthness = 0;
			if (recording)
			{
				int P = (recordpos * 8 * 255) / recordingbufferlength;
				int pp = P & 0xff;
				for (int i = 0; i < (P >> 8)+1; i++) Leds[i].Brigthness = 255;
				//Leds[(P >> 8)].Brigthness = 255 - pp;
				Leds[((P >> 8) + 1)%8].Brigthness = pp;
			}
			else
			{
				if (overdub)
				{
					Leds[LooperLed_Rec].Brigthness = ClockedBlink;

				}
				if (playing)
				{
					int P = (playpos * 8 * 255) / playbufferlength;
					int pp = P & 0xff;
					Leds[(P >> 8)].Brigthness = 255 - pp;
					Leds[((P >> 8) + 1)%8].Brigthness = pp;
				}
				else
				{
					Leds[startbeat].Brigthness = 128;

				}
			}

		}

		if (armed)
		{
			Leds[LooperLed_Arm].Brigthness = 255;
		}
	}
	int fixdc;
	bool firstrun;
	bool faderec;

	float Get(float input)
	{
		samplecountsinceclock++;
		float r = inlevel * input;
		
		if (playing)
		{
			if (playpos < FADETIME)
			{
				if (firstrun == true)
				{
				}
				else
				{
					firstrun = false;
				}
			}
			r += Buffers[playingbuffer][playpos] * outlevel;
			if (overdub)
			{
				 Buffers[playingbuffer][playpos] += input;
			}
			playpos++;
			fixdc /= 2;
			if (playpos > playbufferlength - FADETIME)
			{
				if (playpos > playbufferlength)
				{
					if (AutoLoop)
					{
						playpos = 0;
						fixdc = r;
						firstrun = false;
					}
					else
					{
						playing = false;
					}
				}
			}
		}

		if (recording)
		{
			Buffers[recordingbuffer][recordpos++] = input;
			if (RecordingTick >= BeatLength && ! faderec)
			{
				faderec = true;
		//		printf("starting playback, still recording tail\n");
				playing = true;
				playbufferlength = recordpos;
				playpos = 0;
			}
			else
			{
				if (recordpos >= playbufferlength   + FADETIME * 4)
				{
			//		printf("recording tail stopped\n");
					recording = false;
				}
			}
		}

		return r;
	}
};

class LooperModule
{
public:
	Looper A;
	Looper B;

	void SetBuffers(float *b1, float *b2, float *b3, float *b4)
	{
		A.SetBuffers(b1, b2);
		B.SetBuffers(b3, b4);
	}

	int TempoVal;
	LedState Leds[__ModuleLed_Count];
	GateState ClockGate;
	ButtonState Buttons[__ModuleButton_Count];
	
	bool Shifted;
	void Clock()
	{
	//	printf("C\n");
		A.Clock();
		B.Clock();
	}
	
	void Reset()
	{
		//printf("R\n");
		A.Reset();
		B.Reset();
	}

	void SetupLeds()
	{
		A.SetupLeds(Shifted);
		B.SetupLeds(Shifted);
#define __max(a,b) (((a)>(b))?(a):(b))

		if (Leds[ModuleLed_Clock].Brigthness > 0) Leds[ModuleLed_Clock].Brigthness-=20;
		int B = Buttons[ModuleButton_Clock].state?255:0;
		Leds[ModuleLed_Clock].Brigthness = __max(Leds[ModuleLed_Clock].Brigthness , B);
		if (Leds[ModuleLed_Clock].Brigthness<0) Leds[ModuleLed_Clock].Brigthness = 0;

		if (Leds[ModuleLed_Reset].Brigthness > 0) Leds[ModuleLed_Reset].Brigthness-=20;
		int B2 = Buttons[ModuleButton_Reset].state?255:0;
		Leds[ModuleLed_Reset].Brigthness = __max(Leds[ModuleLed_Reset].Brigthness , B2);
		if (Leds[ModuleLed_Reset].Brigthness<0) Leds[ModuleLed_Reset].Brigthness = 0;

	}

	void UpdateButtonScan()
	{
		Shifted = Buttons[ModuleButton_Shift].Update() == Button_Down;
		A.UpdateButtons(Shifted);
		B.UpdateButtons(Shifted);
	}

	void FillBuffer(int32_t* inp, int32_t* outp, int len)
	{
		for (int i = 0; i < len; i++)
		{
			*outp++ = A.Get(*inp++);
			*outp++ = B.Get(*inp++);
		}
	}
};
