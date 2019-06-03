#pragma once

#include "Define.h"
#include <SoftwareSerial.h>
#include "Encoder.h"
#include "Button.h"
#include <BH1750.h>


class GPS_ClockStaeBase;
class GPGGADataString;


class GPS_Clock
{
public:

	GPS_Clock(volatile int& EncoderTurnCounter) : GPSModuleSerial(GPSRX, GPSTX), Encoder(EncoderTurnCounter){}

	void Setup();

	void Loop();

	uint8_t GetSec() const;

	uint8_t GetMin() const;

	uint8_t GetHour() const;

	uint8_t GetAlaramHour() const { return AlaramHour; }

	uint8_t GetAlaramMin() const { return AlaramMin; }

	unsigned long  GetSecondsFromLastSinhronization() const { return (millis() - LastSynhronizationCount) / 1000; }

	EClockState::Value GetCurrentState();

	bool SetState(EClockState::Value NewState);

	void SetTime(GPGGADataString& GPSTime);

	bool CanChangeState();

	bool IsAlaramRedy() { return bAlaramEnabel && GetMin() == AlaramMin && GetHour() == AlaramHour; }

	void RefreshClockDisplay(uint8_t Hour_, uint8_t Min_, bool ChangeBrightness = true, EDisplayMode::Value HourDisplayMode = EDisplayMode::Auto);

	void CheckDataFrom_EEPROM();

	bool IsAlaramEnabel() const { return bAlaramEnabel; }

	void SetAlaramEnabel(bool IsEnabel);

	bool IsMilliseOverloaf() const { return LastSynhronizationCount > millis(); }

	void RefreshClock( bool ChangeBrightness = true);

	void SwitchOnDisplayHour(EDisplayMode::Value BrightnessLevelHour);

	void SwitchOnDisplayMin(EDisplayMode::Value BrightnessLevelMin);

	void DisplayBlinking(bool HourBlink, bool MinBlink, bool WithBuzzer, bool WithTwoDot, bool WithAllaramEnableLight);

	void OutPutBlink(uint8_t PortForBliking);

	void SetAlaramHour(int  Hour);

	void SetAlaramMin(int  Min);

	void SetTimeZone(int  NewTimeZone);

	void SaveTimeZoneInEPROM();

	void SaveSnoozDelayInEPROM();

	int GetTimeZoneDelay() { return TimeZone; }

	uint8_t GetSnoozDelayMin() { return SnoozDelayMin; }

	void SetDisplayBrightnessFromLightMeter();

	void CheckAllaramEnabelLight();

	void SetSnoozDelay(int NewSnoozDelay);


	//void SendSleepModeMsg();

	//void SendHotStartMsg();

	bool IsFistSynchronization() { return FistSynchronization; }

	void FistSynchronizationSet() { FistSynchronization = false; }

	cEncoder Encoder;

	cButton EncoderButton{ EncodeClick};

	cButton AlaramEnableButton { AllaramEnableButton} ;

	SoftwareSerial GPSModuleSerial;

private:

	uint8_t AlaramHour = 0;

	uint8_t AlaramMin = 0;

	uint8_t DisplayHour = 0;

	uint8_t DisplayMin = 0;

	unsigned long SynhronizationTimeSec = 0;

	unsigned long LastSynhronizationCount = 0;

	bool bAlaramEnabel = false;

	GPS_ClockStaeBase* CurrentState = NULL;

	unsigned long LastChangeStateTime = 0;

	unsigned long BlinkCount = 0;

	bool DisplayLight = true;

	uint8_t TimeZone = 3;

	bool FistSynchronization = true;

	uint8_t SnoozDelayMin = 5;

	BH1750 lightMeter{ 0x23 };

	

	//byte HeaderMessage[8] = { 0xB5, 0x62, 0x06, 0x04, 0x04, 0x00, 0x00, 0x00 };

	//byte SleepMode[4] = { 0x08, 0x00, 0x16, 0x74 };

	//byte HotStart[4] = { 0x02, 0x00, 0x10, 0x68 };

};
