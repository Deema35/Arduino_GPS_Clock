#pragma once
#include "Define.h"
#include <Arduino.h>

class cButton
{
public:

	cButton(uint8_t _ButtonID) : ButtonID(_ButtonID){}

	void Loop();

	EButtonState::Value GetState();

private:

	void SetState(EButtonState::Value NewState);

	EButtonState::Value CurrentState;

	uint8_t ButtonID;


	unsigned long ButtonLastStateChange = 0;
};