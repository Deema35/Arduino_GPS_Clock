#pragma once
#include "Define.h"
#include "GPSDataString.h"

class GPS_Clock;
class GPS_ClockStaeBase;



GPS_ClockStaeBase* CreateState(EClockState::Value Type, GPS_Clock& Clock);

class GPS_ClockStaeBase
{
public:
	GPS_ClockStaeBase(GPS_Clock& Clock) : OwnClock(Clock) {}

	virtual ~GPS_ClockStaeBase() {}

	virtual EClockState::Value GetType() = 0;

	virtual void Loop() {};

	virtual bool IsStateBlock() = 0;

protected:

	uint8_t GetNumberInRange(uint8_t StartRange, uint8_t EndRange, int Count);

	uint8_t GetMinFromNow(uint8_t DelayMin);

	GPS_Clock& OwnClock;

	unsigned long DelayCount = 0;

	
};

class GPS_ClockStaeNormal : public GPS_ClockStaeBase
{
public:

	GPS_ClockStaeNormal(GPS_Clock& Clock);

	virtual EClockState::Value GetType() override { return EClockState::Normal; };

	virtual void Loop() override;

	virtual bool IsStateBlock() override { return true; }

private:


	NormaStaelDisplayMode::Value CurrentDisplayMode = NormaStaelDisplayMode::Time;

	unsigned long DelayFromLastDisplayModeSwitchCount = 0;

};



class GPS_ClockStaeSynchronization : public GPS_ClockStaeBase
{
public:

	GPS_ClockStaeSynchronization(GPS_Clock& Clock);

	~GPS_ClockStaeSynchronization();

	virtual EClockState::Value GetType() override { return EClockState::Synchronization; };

	virtual void Loop() override;

	virtual bool IsStateBlock() override { return false; }

private:

	

	GPGGADataString GPSDataString;

	GPGSVDataString SateliteDataString;

};

class GPS_ClockStaeSetAlaramHour : public GPS_ClockStaeBase
{
public:

	GPS_ClockStaeSetAlaramHour(GPS_Clock& Clock);

	~GPS_ClockStaeSetAlaramHour();

	virtual EClockState::Value GetType() override { return EClockState::SetAlaramHour; };

	virtual void Loop() override;

	virtual bool IsStateBlock() override { return true; }
};

class GPS_ClockStaeSetAlaramMin : public GPS_ClockStaeBase
{
public:

	GPS_ClockStaeSetAlaramMin(GPS_Clock& Clock);

	~GPS_ClockStaeSetAlaramMin();

	virtual EClockState::Value GetType() override { return EClockState::SetAlaramMin; };

	virtual void Loop() override;

	virtual bool IsStateBlock() override { return true; }
};

class GPS_ClockStaeSetTimeZone : public GPS_ClockStaeBase
{
public:

	GPS_ClockStaeSetTimeZone(GPS_Clock& Clock);

	~GPS_ClockStaeSetTimeZone();

	virtual EClockState::Value GetType() override { return EClockState::SetTimeZone; };

	virtual void Loop() override;

	virtual bool IsStateBlock() override { return true; }
};


class GPS_ClockStaeSetSnoozeTime : public GPS_ClockStaeBase
{
public:

	GPS_ClockStaeSetSnoozeTime(GPS_Clock& Clock);

	~GPS_ClockStaeSetSnoozeTime();

	virtual EClockState::Value GetType() override { return EClockState::SetSnoozeTime; };

	virtual void Loop() override;

	virtual bool IsStateBlock() override { return true; }
};

class GPS_ClockStaeAlaramAlert : public GPS_ClockStaeBase
{
public:

	GPS_ClockStaeAlaramAlert(GPS_Clock& Clock);

	~GPS_ClockStaeAlaramAlert();

	virtual EClockState::Value GetType() override { return EClockState::AlaramAlert; };

	virtual void Loop() override;

	virtual bool IsStateBlock() override { return false; }

private:

	bool IsAlaramAlertEnd();

	void SetAlaramAllertEndTime();

	uint8_t AlaramAlertEndMin = 0;
};

class GPS_ClockStaeAlaramSnooz : public GPS_ClockStaeBase
{
public:

	GPS_ClockStaeAlaramSnooz(GPS_Clock& Clock);

	virtual EClockState::Value GetType() override { return EClockState::AlaramSnooz; };

	virtual void Loop() override;

	virtual bool IsStateBlock() override { return true; }

private:

	void SetSnoozeAlaramTime();

	bool IsSnoozeAlaramRedy();
	
	uint8_t SnoozeAlaramMin = 0;
};



class GPS_ClockStaeGPSTracing : public GPS_ClockStaeBase
{
public:

	GPS_ClockStaeGPSTracing(GPS_Clock& Clock);

	~GPS_ClockStaeGPSTracing();

	virtual EClockState::Value GetType() override { return EClockState::GPSTracing; };

	virtual void Loop() override;

	virtual bool IsStateBlock() override { return false; }



private:

	GPGGADataString GPSDataString;

	GPGSVDataString SateliteDataString;

	GPSTracingDisplayMode::Value CurrentDisplayMode = GPSTracingDisplayMode::SateliteCount;

};