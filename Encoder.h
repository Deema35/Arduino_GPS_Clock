#pragma once
#include "Define.h"

class cEncoder
{
public:

	cEncoder(volatile int& _EncoderTurnCounter) : EncoderTurnCounter(_EncoderTurnCounter) {}

	void ResetCounter() { EncoderTurnCounter = 0; }

	int GetEncoderTurns();

private:

	volatile int& EncoderTurnCounter;
};