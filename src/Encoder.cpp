#include "Encoder.h"
#include "Arduino.h"

volatile int EncoderTurnCounter = 0;

volatile bool S1EncoderTick = false;

volatile bool S2EncoderTick = false;


int cEncoder::GetEncoderTurns()
{
	int CurrenTurns = EncoderTurnCounter;

	ResetCounter();

	return CurrenTurns;

}

void cEncoder::ResetCounter()
{
	EncoderTurnCounter = 0; 
}

void cEncoder::S1Function()
{
	if (S2EncoderTick)
	{
		if (!digitalRead(EncoderS2))
		{
			EncoderTurnCounter++;
		}
		else
		{
			S1EncoderTick = true;
		}

		S2EncoderTick = false;
	}
	else
	{
		S1EncoderTick = true;
	}
}

void cEncoder::S2Function()
{
	if (S1EncoderTick)
	{
		if (!digitalRead(EncoderS1))
		{
			EncoderTurnCounter--;
		}
		else
		{
			S2EncoderTick = true;
		}
		S1EncoderTick = false;
	}
	else
	{
		S2EncoderTick = true;
	}
}

