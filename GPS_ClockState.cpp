#include "GPS_ClockState.h"
#include "GPS_Clock.h"
#include <EEPROM.h>


GPS_ClockStaeBase* CreateState(EClockState::Value Type, GPS_Clock& Clock)
{
	switch (Type)
	{
	case EClockState::Normal:

		return new GPS_ClockStaeNormal(Clock);

	case EClockState::Synchronization:

		return new GPS_ClockStaeSynchronization(Clock);

	case EClockState::SetAlaramHour:

		return new GPS_ClockStaeSetAlaramHour(Clock);

	case EClockState::SetAlaramMin:

		return new GPS_ClockStaeSetAlaramMin(Clock);

	case EClockState::SetTimeZone:

		return new GPS_ClockStaeSetTimeZone(Clock);

	case EClockState::SetSnoozeTime:

		return new GPS_ClockStaeSetSnoozeTime(Clock);

	case EClockState::AlaramAlert:

		return new GPS_ClockStaeAlaramAlert(Clock);

	case EClockState::AlaramSnooz:

		return new GPS_ClockStaeAlaramSnooz(Clock);

	case EClockState::GPSTracing:

		return new GPS_ClockStaeGPSTracing(Clock);
	
	default:

		Serial.println("CreateStateErr");

		return NULL;

	}
}
//GPS_ClockStaeBase..................................




uint8_t GPS_ClockStaeBase::GetMinFromNow(uint8_t DelayMin)
{
	uint8_t Min = OwnClock.GetMin() + DelayMin;

	if (Min > 59)
	{
		Min = Min % 60;
	}

	return Min;
}

uint8_t GPS_ClockStaeBase::GetNumberInRange(uint8_t StartRange, uint8_t EndRange, int Count)
{

	if (Count < StartRange) return StartRange;

	else if (Count > EndRange) return EndRange;

	else return Count;

}


//GPS_ClockStaeNormal..................................

GPS_ClockStaeNormal::GPS_ClockStaeNormal(GPS_Clock& Clock) : GPS_ClockStaeBase(Clock)
{
	OwnClock.SwitchOnDisplayHour(EDisplayMode::Auto);
	OwnClock.SwitchOnDisplayMin(EDisplayMode::Auto);
	digitalWrite(TwoDotCenter, HIGH);
	OwnClock.CheckAllaramEnabelLight();
	OwnClock.RefreshClock();
}

void GPS_ClockStaeNormal::Loop()
{
	int EncoderTurn = OwnClock.Encoder.GetEncoderTurns();

	if (EncoderTurn)
	{
		CurrentDisplayMode = (NormaStaelDisplayMode::Value)GetNumberInRange(NormaStaelDisplayMode::Time, NormaStaelDisplayMode::AlarmTime, EncoderTurn + CurrentDisplayMode);

		DelayFromLastDisplayModeSwitchCount = millis();

		switch (CurrentDisplayMode)
		{
		case NormaStaelDisplayMode::Time:

			OwnClock.RefreshClock();
			break;

		case NormaStaelDisplayMode::SynchronizationTime:

			OwnClock.RefreshClockDisplay(OwnClock.GetSynchronizationHour(), OwnClock.GetSynchronizationMin());
			break;

		case NormaStaelDisplayMode::AlarmTime:

			OwnClock.RefreshClockDisplay(OwnClock.GetAlaramHour(), OwnClock.GetAlaramMin());
			break;
		}
		
	}

	if (millis() - DelayCount > 1000)
	{
		if (CurrentDisplayMode == NormaStaelDisplayMode::Time) OwnClock.RefreshClock();

		else
		{
			if (millis() - DelayFromLastDisplayModeSwitchCount > DelayFromLastDisplayModeSwitchMiliSec)
			{
				CurrentDisplayMode = NormaStaelDisplayMode::Time;
				OwnClock.RefreshClock();
			}
		}
		
		if (OwnClock.GetSecondsFromLastSinhronization() > SinhronizationDelaySec) OwnClock.SetState(EClockState::Synchronization);

		if (OwnClock.IsAlaramRedy()) OwnClock.SetState(EClockState::AlaramAlert);

		DelayCount = millis();
	}

	/*while (OwnClock.GPSModuleSerial.available())
	{
		Serial.write(OwnClock.GPSModuleSerial.read());
	}*/

	switch (OwnClock.AlaramEnableButton.GetState())
	{
	case EButtonState::Holding:

		if (OwnClock.EncoderButton.GetState() == EButtonState::Pressed) OwnClock.SetState(EClockState::SetTimeZone);
		break;

	case EButtonState::Pressed:
		
		OwnClock.SetAlaramEnabel(!OwnClock.IsAlaramEnabel());
		break;

	}

	if (OwnClock.EncoderButton.GetState() == EButtonState::Pressed) OwnClock.SetState(EClockState::SetAlaramHour);

}

//GPS_ClockStaeSynchronization..................................



GPS_ClockStaeSynchronization::GPS_ClockStaeSynchronization(GPS_Clock& Clock) : GPS_ClockStaeBase(Clock)
{
	//OwnClock.SendHotStartMsg();

	if (OwnClock.IsFistSynchronization())
	{
		OwnClock.SwitchOnDisplayHour(EDisplayMode::NotDisplay);
		
	}
	else
	{
		OwnClock.SwitchOnDisplayHour(EDisplayMode::Auto);
		
	}

	digitalWrite(TwoDotCenter, HIGH);

	OwnClock.SwitchOnDisplayMin(EDisplayMode::Auto);
	

	OwnClock.CheckAllaramEnabelLight();

	digitalWrite(EnanbelGPS, HIGH);
	

	
}

GPS_ClockStaeSynchronization::~GPS_ClockStaeSynchronization()
{
	//OwnClock.SendSleepModeMsg();

	digitalWrite(EnanbelGPS, LOW);
	OwnClock.FistSynchronizationSet();

}

void GPS_ClockStaeSynchronization::Loop()
{
	if (millis() - DelayCount > 1000)
	{
		if (OwnClock.IsFistSynchronization())
		{
			OwnClock.RefreshClockDisplay(0, SateliteDataString.GetSatteliteCount(), true, EDisplayMode::NotDisplay);
			
		}
		else
		{
			OwnClock.RefreshClock();

			if (OwnClock.IsAlaramRedy()) OwnClock.SetState(EClockState::AlaramAlert);

		}

		DelayCount = millis();
	}

	OwnClock.OutPutBlink(TwoDotCenter);
	
	

	byte data;
	while (OwnClock.GPSModuleSerial.available())
	{
		data = OwnClock.GPSModuleSerial.read();
		SateliteDataString.Encode(data);
		GPSDataString.Encode(data);
	}

	if (GPSDataString.IsTimeValid())
	{
		OwnClock.SetTime(GPSDataString);
		OwnClock.SetState(EClockState::Normal);
		
	}

	if (OwnClock.AlaramEnableButton.GetState() == EButtonState::Pressed) OwnClock.SetAlaramEnabel(!OwnClock.IsAlaramEnabel());

}



//GPS_ClockStaeSetAlaramHour..................................

GPS_ClockStaeSetAlaramHour::GPS_ClockStaeSetAlaramHour(GPS_Clock& Clock) : GPS_ClockStaeBase(Clock)
{
	OwnClock.SwitchOnDisplayHour(EDisplayMode::Auto);
	OwnClock.SwitchOnDisplayMin(EDisplayMode::Auto);
	digitalWrite(TwoDotCenter, HIGH);
	OwnClock.CheckAllaramEnabelLight();
	OwnClock.Encoder.ResetCounter();
	OwnClock.RefreshClockDisplay(OwnClock.GetAlaramHour(), OwnClock.GetAlaramMin());
	
}

GPS_ClockStaeSetAlaramHour::~GPS_ClockStaeSetAlaramHour()
{
	EEPROM.write(EEEPROMAddr::AlaramHour, OwnClock.GetAlaramHour());

}

void GPS_ClockStaeSetAlaramHour::Loop()
{
	int EncoderTurn = OwnClock.Encoder.GetEncoderTurns();

	if (EncoderTurn)
	{
		OwnClock.SetAlaramHour(OwnClock.GetAlaramHour() + EncoderTurn);
		OwnClock.RefreshClockDisplay(OwnClock.GetAlaramHour(), OwnClock.GetAlaramMin(),true, EDisplayMode::Auto);
	}
	
	OwnClock.DisplayBlinking(false, true, false, true, false);

	if (OwnClock.EncoderButton.GetState() == EButtonState::Pressed)
	{

		OwnClock.SetState(EClockState::SetAlaramMin);

	}

	else if (OwnClock.AlaramEnableButton.GetState() == EButtonState::Pressed)
	{
		OwnClock.SetState(EClockState::Normal);
	}
	
}






//GPS_ClockStaeSetAlaramMin..................................

GPS_ClockStaeSetAlaramMin::GPS_ClockStaeSetAlaramMin(GPS_Clock& Clock) : GPS_ClockStaeBase(Clock)
{
	OwnClock.SwitchOnDisplayHour(EDisplayMode::Auto);
	OwnClock.SwitchOnDisplayMin(EDisplayMode::Auto);
	digitalWrite(TwoDotCenter, HIGH);
	OwnClock.CheckAllaramEnabelLight();
	OwnClock.Encoder.ResetCounter();
	OwnClock.RefreshClockDisplay(OwnClock.GetAlaramHour(), OwnClock.GetAlaramMin());
	
}

GPS_ClockStaeSetAlaramMin::~GPS_ClockStaeSetAlaramMin()
{
	EEPROM.write(EEEPROMAddr::AlaramMin, OwnClock.GetAlaramMin());

}

void GPS_ClockStaeSetAlaramMin::Loop()
{
	int EncoderTurn = OwnClock.Encoder.GetEncoderTurns();

	if (EncoderTurn)
	{
		OwnClock.SetAlaramMin(OwnClock.GetAlaramMin() + EncoderTurn);
		OwnClock.RefreshClockDisplay(OwnClock.GetAlaramHour(), OwnClock.GetAlaramMin());
	}

	OwnClock.DisplayBlinking(true, false, false, true, false);

	if (OwnClock.AlaramEnableButton.GetState() == EButtonState::Pressed || OwnClock.EncoderButton.GetState() == EButtonState::Pressed)
	{
		OwnClock.SetState(EClockState::Normal);
	}

}




//GPS_ClockStaeSetTimeZone..................................

GPS_ClockStaeSetTimeZone::GPS_ClockStaeSetTimeZone(GPS_Clock& Clock) : GPS_ClockStaeBase(Clock)
{
	OwnClock.SwitchOnDisplayHour(EDisplayMode::Auto);
	OwnClock.SwitchOnDisplayMin(EDisplayMode::Auto);
	digitalWrite(TwoDotCenter, HIGH);
	OwnClock.CheckAllaramEnabelLight();
	OwnClock.Encoder.ResetCounter();
	OwnClock.RefreshClock();
}

 GPS_ClockStaeSetTimeZone::~GPS_ClockStaeSetTimeZone()
{
	OwnClock.SaveTimeZoneInEPROM();
}

void GPS_ClockStaeSetTimeZone::Loop()
{

	int EncoderTurn = OwnClock.Encoder.GetEncoderTurns();

	if (EncoderTurn)
	{
		OwnClock.SetTimeZone(OwnClock.GetTimeZoneDelay() + EncoderTurn);

		OwnClock.RefreshClock();
	}

	

	OwnClock.DisplayBlinking(false, true, false, true, false);

	if (OwnClock.EncoderButton.GetState() == EButtonState::Pressed)
	{
		OwnClock.SetState(EClockState::SetSnoozeTime);
	}

	else if (OwnClock.AlaramEnableButton.GetState() == EButtonState::Pressed)
	{
		OwnClock.SetState(EClockState::Normal);
	}
}






//GPS_ClockStaeSetSnoozeTime..................................

GPS_ClockStaeSetSnoozeTime::GPS_ClockStaeSetSnoozeTime(GPS_Clock& Clock) : GPS_ClockStaeBase(Clock) 
{
	OwnClock.SwitchOnDisplayHour(EDisplayMode::NotDisplay);
	OwnClock.SwitchOnDisplayMin(EDisplayMode::Auto);
	digitalWrite(TwoDotCenter, HIGH);
	OwnClock.CheckAllaramEnabelLight();
	OwnClock.Encoder.ResetCounter();
	OwnClock.RefreshClockDisplay(0, OwnClock.GetSnoozDelayMin(), true, EDisplayMode::NotDisplay);
}

GPS_ClockStaeSetSnoozeTime::~GPS_ClockStaeSetSnoozeTime()
{
	OwnClock.SaveSnoozDelayInEPROM();
}

void GPS_ClockStaeSetSnoozeTime::Loop()
{

	int EncoderTurn = OwnClock.Encoder.GetEncoderTurns();

	if (EncoderTurn)
	{
		OwnClock.SetSnoozDelay(OwnClock.GetSnoozDelayMin() + EncoderTurn);

		OwnClock.RefreshClockDisplay(0, OwnClock.GetSnoozDelayMin(), true, EDisplayMode::NotDisplay);
	}



	if (OwnClock.EncoderButton.GetState() == EButtonState::Pressed || OwnClock.AlaramEnableButton.GetState() == EButtonState::Pressed)
	{
		OwnClock.SetState(EClockState::Normal);
	}
}


//GPS_ClockStaeAlaramAlert..................................

GPS_ClockStaeAlaramAlert::GPS_ClockStaeAlaramAlert(GPS_Clock& Clock) : GPS_ClockStaeBase(Clock) 
{
	OwnClock.SwitchOnDisplayHour(EDisplayMode::Auto);
	OwnClock.SwitchOnDisplayMin(EDisplayMode::Auto);
	digitalWrite(TwoDotCenter, HIGH);
	OwnClock.CheckAllaramEnabelLight();
	analogWrite(DisplayBrightness, 255);
	SetAlaramAllertEndTime();
}

GPS_ClockStaeAlaramAlert::~GPS_ClockStaeAlaramAlert()
{
	digitalWrite(Buzzer, LOW);
}

void GPS_ClockStaeAlaramAlert::Loop()
{

	if (millis() - DelayCount > 1000)
	{
		OwnClock.RefreshClock(false);

		DelayCount = millis();
	}

	OwnClock.DisplayBlinking(true, true, true, true, true);

	if (digitalRead(SnoozeButton) || IsAlaramAlertEnd()) OwnClock.SetState(EClockState::AlaramSnooz);
	
	else if (OwnClock.AlaramEnableButton.GetState() == EButtonState::Pressed || OwnClock.EncoderButton.GetState() == EButtonState::Pressed)
	{
		OwnClock.SetAlaramEnabel(false);
		OwnClock.SetState(EClockState::Normal);
	}
	
}


void GPS_ClockStaeAlaramAlert::SetAlaramAllertEndTime()
{
	AlaramAlertEndMin = GetMinFromNow(AlaramAllertDurationMin);
}



bool GPS_ClockStaeAlaramAlert::IsAlaramAlertEnd()
{
	return OwnClock.GetMin() == AlaramAlertEndMin; 
}

//GPS_ClockStaeAlaramSnooz..................................

GPS_ClockStaeAlaramSnooz::GPS_ClockStaeAlaramSnooz(GPS_Clock& Clock) : GPS_ClockStaeBase(Clock)
{
	OwnClock.SwitchOnDisplayHour(EDisplayMode::Auto);
	OwnClock.SwitchOnDisplayMin(EDisplayMode::Auto);
	digitalWrite(TwoDotCenter, HIGH);
	OwnClock.CheckAllaramEnabelLight();
	SetSnoozeAlaramTime();

}

void GPS_ClockStaeAlaramSnooz::SetSnoozeAlaramTime()
{ 

	SnoozeAlaramMin = GetMinFromNow(OwnClock.GetSnoozDelayMin());
	
}

void GPS_ClockStaeAlaramSnooz::Loop()
{
	

	if (millis() - DelayCount > 1000)
	{
		OwnClock.RefreshClock();
	}

	OwnClock.OutPutBlink(AllaramEnableLight);

	if (OwnClock.AlaramEnableButton.GetState() == EButtonState::Pressed)
	{
		OwnClock.SetAlaramEnabel(false);
		OwnClock.SetState(EClockState::Normal);
	}

	else if (IsSnoozeAlaramRedy()) OwnClock.SetState(EClockState::AlaramAlert);
}


bool GPS_ClockStaeAlaramSnooz::IsSnoozeAlaramRedy()
{ 
	return OwnClock.GetMin() == SnoozeAlaramMin;
}

//GPS_ClockStaeGPSTracing..................................

GPS_ClockStaeGPSTracing::GPS_ClockStaeGPSTracing(GPS_Clock& Clock) : GPS_ClockStaeBase(Clock)
{

	OwnClock.SwitchOnDisplayHour(EDisplayMode::TwoNumbers);
	OwnClock.SwitchOnDisplayMin(EDisplayMode::Auto);
	digitalWrite(TwoDotCenter, LOW);
	digitalWrite(AllaramEnableLight, LOW);
	digitalWrite(EnanbelGPS, HIGH);
	OwnClock.SetDisplayBrightnessFromLightMeter();
	
}

GPS_ClockStaeGPSTracing::~GPS_ClockStaeGPSTracing()
{
	
	digitalWrite(EnanbelGPS, LOW);
}

void GPS_ClockStaeGPSTracing::Loop()
{
	int EncoderTurn = OwnClock.Encoder.GetEncoderTurns();

	if (EncoderTurn)
	{
		if (GPSDataString.IsCoordinateValide())
		{

			CurrentDisplayMode = (GPSTracingDisplayMode::Value)GetNumberInRange(GPSTracingDisplayMode::SateliteCount, GPSTracingDisplayMode::longitude, EncoderTurn + CurrentDisplayMode);

			switch (CurrentDisplayMode)
			{
			case GPSTracingDisplayMode::SateliteCount:

				OwnClock.RefreshClockDisplay(0, SateliteDataString.GetSatteliteCount(), true, EDisplayMode::TwoNumbers);
				digitalWrite(TwoDotCenter, LOW);
				break;

			case GPSTracingDisplayMode::latitude:

				OwnClock.RefreshClockDisplay(GPSDataString.GetlatitudeDegrees(), GPSDataString.GetlatitudeMinute(), true, EDisplayMode::TwoNumbers);
				digitalWrite(TwoDotCenter, HIGH);
				break;

			case GPSTracingDisplayMode::longitude:

				OwnClock.RefreshClockDisplay(GPSDataString.GetlongitudeDegrees(), GPSDataString.GetlongitudeMinute(), true, EDisplayMode::TwoNumbers);
				digitalWrite(TwoDotCenter, HIGH);
				break;
			}
		}
	}
	

	if (millis() - DelayCount > 1000)
	{
		if (GPSDataString.IsCoordinateValide())
		{
			digitalWrite(AllaramEnableLight, HIGH);
		}
		switch (CurrentDisplayMode)
		{
		case GPSTracingDisplayMode::SateliteCount:

			OwnClock.RefreshClockDisplay(0, SateliteDataString.GetSatteliteCount(), true, EDisplayMode::TwoNumbers);
			digitalWrite(TwoDotCenter, LOW);
			break;

		case GPSTracingDisplayMode::latitude:

			OwnClock.RefreshClockDisplay(GPSDataString.GetlatitudeDegrees(), GPSDataString.GetlatitudeMinute(), true, EDisplayMode::TwoNumbers);
			digitalWrite(TwoDotCenter, HIGH);
			break;

		case GPSTracingDisplayMode::longitude:

			OwnClock.RefreshClockDisplay(GPSDataString.GetlongitudeDegrees(), GPSDataString.GetlongitudeMinute(), true, EDisplayMode::TwoNumbers);
			digitalWrite(TwoDotCenter, HIGH);
			break;
		}
		
		DelayCount = millis();
	}

		
	
	byte data;

	while (OwnClock.GPSModuleSerial.available())
	{
		data = OwnClock.GPSModuleSerial.read();
		GPSDataString.Encode(data);
		SateliteDataString.Encode(data);
		Serial.write(data);
	}

}
