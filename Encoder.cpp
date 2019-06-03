#include "Encoder.h"

int cEncoder::GetEncoderTurns()
{
	int CurrenTurns = EncoderTurnCounter;

	ResetCounter();

	return CurrenTurns;

}

