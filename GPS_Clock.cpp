
#include "GPS_Clock.h"
#include "GPS_ClockState.h"
#include <EEPROM.h>
#include <Wire.h>
#include <Arduino.h>


void GPS_Clock::Setup()
{

	
	Wire.begin();

	GPSModuleSerial.begin(9600);

	CheckDataFrom_EEPROM();

	if (!digitalRead(AllaramEnableButton) || !digitalRead(EncodeClick))
	{
		CurrentState = CreateState(EClockState::GPSTracing, this);
	}
	else
	{
		CurrentState = CreateState(EClockState::Synchronization, this);
	}
	

	if (lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE))
	{
		Serial.println(F("BH1750 begin"));
	}
	else
	{
		Serial.println(F("Error initialising BH1750"));
	}


}

void GPS_Clock::Loop()
{
	AlaramEnableButton.Loop();

	EncoderButton.Loop();

	CurrentState->Loop();
}



uint8_t GPS_Clock::GetSec() const
{
	unsigned long Result = SynhronizationTimeSec + GetSecondsFromLastSinhronization();
	return Result % 60;
	
}


uint8_t GPS_Clock::GetMin() const
{
	unsigned long Result = SynhronizationTimeSec + GetSecondsFromLastSinhronization();

	Result = Result / 60;

	return Result % 60;
}


uint8_t GPS_Clock::GetHour() const
{
	unsigned long Result = SynhronizationTimeSec + GetSecondsFromLastSinhronization();
	Result = Result / 3600;

	Result = TimeZone + Result;

	return Result % 24;
}

EClockState::Value GPS_Clock::GetCurrentState()
{ 
	return CurrentState->GetType();
}

void GPS_Clock::SetTime(GPGGADataString& GPSTime)
{
	SynhronizationTimeSec =  (unsigned long)GPSTime.GetHour() * 3600;


	SynhronizationTimeSec = SynhronizationTimeSec + (unsigned long)GPSTime.GetMin() * 60;


	SynhronizationTimeSec = SynhronizationTimeSec + GPSTime.GetSec();

	LastSynhronizationCount = GPSTime.GetTimeCount();
}



bool GPS_Clock::SetState(EClockState::Value NewState)
{
	if (NewState == CurrentState->GetType()) return true;
	
	if (CanChangeState())
	{
		Serial.print("Change state "); Serial.print(int(CurrentState)); Serial.print(" to "); Serial.println(int(NewState));

		delete CurrentState;
		
		CurrentState = CreateState(NewState, this);
    
		LastChangeStateTime = millis();
		return true;
	}
	return false;
}

bool GPS_Clock::CanChangeState()
{
	
	if (!CurrentState->IsStateBlock()) return true;

	else
	{
		if ((millis() - LastChangeStateTime) > ChangeStaeClockDelay)
		{
			return true;
		}

		else return false;
	}
}






void GPS_Clock::RefreshClockDisplay(uint8_t Hour_, uint8_t Min_, bool ChangeBrightness, EDisplayMode::Value HourDisplayMode)
{
	bool HourChanged = Hour_ != DisplayHour;


//Hour..............

	if (Hour_ < DisplayHour || Min_ < DisplayMin)
	{
		digitalWrite(ResetTime, HIGH);
		digitalWrite(ResetTime, LOW);
		DisplayHour = 0;
		DisplayMin = 0;
		
	}
	
	
	int Count = Hour_ - DisplayHour;
	for (int i = 0; i < Count; i++)
	{
		digitalWrite(H_Clock, HIGH);
		digitalWrite(H_Clock, LOW);
	}


	Count = Min_ - DisplayMin;
	for (int i = 0; i < Count; i++)
	{
		digitalWrite(M_Clock, HIGH);
		digitalWrite(M_Clock, LOW);
	}

	DisplayHour = Hour_;
	DisplayMin = Min_;

	if (HourChanged) SwitchOnDisplayHour(HourDisplayMode);

	if (ChangeBrightness) SetDisplayBrightnessFromLightMeter();

}

void GPS_Clock::CheckDataFrom_EEPROM()
{
	if (EEPROM.read(EEEPROMAddr::AlaramEnabel) == 1) bAlaramEnabel = true;
	
	uint8_t HourEEPROM = EEPROM.read(EEEPROMAddr::AlaramHour);
	if (HourEEPROM < 25) AlaramHour = HourEEPROM;

	uint8_t MinEEPROM = EEPROM.read(EEEPROMAddr::AlaramMin);
	if (MinEEPROM < 61) AlaramMin = MinEEPROM;

	uint8_t TimeZoneEEPROM = EEPROM.read(EEEPROMAddr::TimeZoneDelay);
	if (TimeZoneEEPROM < 25) TimeZone = TimeZoneEEPROM;

	uint8_t SnoozDelayEEPROM = EEPROM.read(EEEPROMAddr::SnoozDelay);
	if (SnoozDelayEEPROM < 16) SnoozDelayMin = SnoozDelayEEPROM;
	
}


void GPS_Clock::SetAlaramEnabel(bool IsEnabel)
{
	if (bAlaramEnabel == IsEnabel) return;

	if (IsEnabel)
	{
		EEPROM.write(0, 1);

	}
	else
	{
		EEPROM.write(0, 0);
	}

	bAlaramEnabel = IsEnabel;

	CheckAllaramEnabelLight();
}


void GPS_Clock::RefreshClock(bool ChangeBrightness)
{
	if (IsMilliseOverloaf()) SetState(EClockState::Synchronization);

	RefreshClockDisplay(GetHour(), GetMin(), ChangeBrightness, EDisplayMode::Auto);

	//Serial.print("time "); Serial.print(Hour); Serial.print(":"); Serial.print(Min); Serial.print(":"); Serial.println(GetSec());
}


void GPS_Clock::SwitchOnDisplayHour(EDisplayMode::Value BrightnessLevelHour)
{
	switch (BrightnessLevelHour)
	{
	case EDisplayMode::Auto:

		
		if (DisplayHour > 9)
		{
			digitalWrite(EnabelHourFistDisplay, HIGH);
		}
		else
		{
			digitalWrite(EnabelHourFistDisplay, LOW);
		}
		digitalWrite(EnabelHourSecondDisplay, HIGH);

		break;

	case EDisplayMode::TwoNumbers:

		digitalWrite(EnabelHourFistDisplay, HIGH);
		digitalWrite(EnabelHourSecondDisplay, HIGH);

		break;

	case EDisplayMode::NotDisplay:

		digitalWrite(EnabelHourFistDisplay, LOW);
		digitalWrite(EnabelHourSecondDisplay, LOW);

		break;

	}

	
		
}

void GPS_Clock::SwitchOnDisplayMin(EDisplayMode::Value BrightnessLevelMin)
{

	switch (BrightnessLevelMin)
	{
	case EDisplayMode::Auto:
	case EDisplayMode::TwoNumbers:

		digitalWrite(EnabelMinDisplay, HIGH);

		break;

	case EDisplayMode::NotDisplay:

		digitalWrite(EnabelMinDisplay, LOW);

		break;

	}

}

void  GPS_Clock::DisplayBlinking(bool HourBlink, bool MinBlink, bool WithBuzzer, bool WithTwoDot, bool WithAllaramEnableLight)
{
	unsigned long BlinkDelay = millis() - BlinkCount;

	if (BlinkDelay > 1000 || BlinkDelay < 0)
	{
		BlinkCount = millis();

		if (DisplayLight) 
		{
			if (HourBlink) SwitchOnDisplayHour(EDisplayMode::Auto);
			if (MinBlink) SwitchOnDisplayMin(EDisplayMode::Auto);

			if (WithBuzzer) digitalWrite(Buzzer, HIGH);
			if (WithTwoDot) digitalWrite(TwoDotCenter, HIGH);
			if (WithAllaramEnableLight) digitalWrite(AllaramEnableLight, HIGH);
		}
		
		else
		{
			if (HourBlink) SwitchOnDisplayHour(EDisplayMode::NotDisplay);
			if (MinBlink) SwitchOnDisplayMin(EDisplayMode::NotDisplay);

			if (WithBuzzer) digitalWrite(Buzzer, LOW);
			if (WithTwoDot) digitalWrite(TwoDotCenter, LOW);
			if (WithAllaramEnableLight) digitalWrite(AllaramEnableLight, LOW);
		}

		DisplayLight = !DisplayLight;

	}
}

void GPS_Clock::OutPutBlink(uint8_t PortForBliking)
{
	if (!PortForBliking) return;

	unsigned long BlinkDelay = millis() - BlinkCount;

	if (BlinkDelay > 1000 || BlinkDelay < 0)
	{
		BlinkCount = millis();
		if (DisplayLight)
		{
			
			digitalWrite(PortForBliking, HIGH);
			
			
		}
		else
		{
			
			digitalWrite(PortForBliking, LOW);
			
		}

		DisplayLight = !DisplayLight;
	}
}




void GPS_Clock::SetAlaramHour(int NewHour)
{
	if (NewHour >= 0)
	{
		AlaramHour = NewHour % 24;
	}
	else
	{
		AlaramHour = 24 + NewHour % 24;
	}
}



void GPS_Clock::SetAlaramMin(int NewMin)
{

	if (NewMin >= 0)
	{
		AlaramMin = NewMin % 60;
	}
	else
	{
		AlaramMin = 60 + NewMin % 60;
	}

	
}

void GPS_Clock::SetTimeZone(int  NewTimeZone)
{
	if (NewTimeZone >= 0)
	{
		TimeZone = NewTimeZone % 24;
	}
	else
	{
		TimeZone = 24 + NewTimeZone % 24;
	}
		
}

void GPS_Clock::SaveTimeZoneInEPROM()
{
	EEPROM.write(EEEPROMAddr::TimeZoneDelay, TimeZone);
}

void GPS_Clock::SaveSnoozDelayInEPROM()
{
	EEPROM.write(EEEPROMAddr::SnoozDelay, SnoozDelayMin);
}

void GPS_Clock::SetDisplayBrightnessFromLightMeter()
{
	float lux = lightMeter.readLightLevel();
	
	float k = lux / 50;
	if (k > 1) k = 1;
	uint8_t LightLevel = 20 + k * 235;

	analogWrite(DisplayBrightness, LightLevel);
}


void GPS_Clock::CheckAllaramEnabelLight()
{
	if (IsAlaramEnabel()) digitalWrite(AllaramEnableLight, HIGH);
	else digitalWrite(AllaramEnableLight, LOW);
}

void GPS_Clock::SetSnoozDelay(int NewSnoozDelay)
{
	NewSnoozDelay = NewSnoozDelay - 5;

	if (NewSnoozDelay >= 0)
	{
		SnoozDelayMin = NewSnoozDelay % 11 + 5;
	}
	else
	{
		SnoozDelayMin = 11 + NewSnoozDelay % 11 + 5;
	}
}

//void GPS_Clock::SendSleepModeMsg()
//{
//	/*Serial.println("SendSleepModeMsg()");
//	
//	GPSModuleSerial.write(HeaderMessage, sizeof(HeaderMessage));
//	GPSModuleSerial.write(SleepMode, sizeof(SleepMode));*/
//	
//		
//}
//
//void GPS_Clock::SendHotStartMsg()
//{
//	/*Serial.println("SendHotStartMsg()");
//	GPSModuleSerial.write(HeaderMessage, sizeof(HeaderMessage));
//	GPSModuleSerial.write(HotStart, sizeof(HotStart));*/
//
//}
