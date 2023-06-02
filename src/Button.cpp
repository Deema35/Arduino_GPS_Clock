#include "Button.h"


void cButton::Loop()
{
	if (millis() - ButtonLastStateChange < DebounceButtonClickDelayMiliSec)
	{
		return;
	}


	if (!digitalRead(ButtonID))
	{
		if (CurrentState == EButtonState::UnPressed)
		{
			SetState(EButtonState::Pressed);
		}
	}
	
	else 
	{
		SetState(EButtonState::UnPressed);
	}

	return;
}

void cButton::SetState(EButtonState::Value NewState)
{
	ButtonLastStateChange = millis();
	CurrentState = NewState;
}

EButtonState::Value cButton::GetState()
{
	if (CurrentState == EButtonState::Pressed)
	{
		CurrentState = EButtonState::Holding;
		return EButtonState::Pressed;
	}
	else
	{
		return CurrentState;
	}
}