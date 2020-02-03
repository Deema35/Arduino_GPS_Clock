#pragma once
#include <Arduino.h>
#include "Define.h"

#define TIME_STRING_LEN 6
#define LATITUDE_STRING_LEN 4
#define LONGITUDE_STRING_LEN 5
#define SATTELITE_COUNT_STRING_LEN 2

class GPSDataString
{

public:

	void Encode(char c);

protected:

	

	unsigned int ConvertStringToInt(const char* String, uint8_t StringLenght) const;

private:

	virtual bool HeaderCheck(char c, uint8_t position) = 0;

	virtual bool ReadData(char c, uint8_t FieldNumber, uint8_t position) = 0;

	virtual void DataCommit() = 0;

	void ChangeState(GPSDataStringState::Value NewState) { State = NewState; }

	int FromHexCharToInt(char c);

	void CheckSummRead(char c, uint8_t position);

	bool CheckSummTest() { return CheckSumm == StringCheckSumm; }

private:
	uint8_t FieldPosition = 0;

	uint8_t FieldsCount = 0;

	GPSDataStringState::Value State = GPSDataStringState::none;

	int CheckSumm = 0;

	int StringCheckSumm = 0;
};


class GPGGADataString : public GPSDataString
{
public:

	bool IsTimeValid() const { return TimeCount != 0; }

	bool IsCoordinateValide() const { return CoordinateValide; }

	unsigned long GetTimeInSec() const { return TimeInSec; }

	unsigned long GetTimeCount() const { return TimeCount; }

	uint8_t GetlatitudeDegrees() const { return LatitudeDegrees; }

	uint8_t GetlatitudeMinute() const { return LatitudeMinute; }

	uint8_t GetlongitudeDegrees() const { return LongitudeDegrees; }

	uint8_t GetlongitudeMinute() const { return LongitudeMinute; }

	void ResetTimeData();

private:

	virtual bool HeaderCheck(char c, uint8_t position) override;

	virtual bool ReadData(char c, uint8_t FieldNumber, uint8_t position) override;

	virtual void DataCommit() override;

private:

	unsigned long TimeCountBeforeCommited = 0;

	unsigned long TimeCount = 0;

	unsigned long TimeInSec = 0;

	char TimeString[TIME_STRING_LEN] { '0', '0', '0', '0', '0', '0' };

	char latitude[LATITUDE_STRING_LEN] { '0', '0',  '0', '0' };

	char longitude[LONGITUDE_STRING_LEN] { '0', '0', '0', '0', '0' };

	bool CoordinateValideBeforeCommited = false;

	bool CoordinateValide = false;

	uint8_t LatitudeDegrees = 0;

	uint8_t LatitudeMinute = 0;

	uint8_t LongitudeDegrees = 0;

	uint8_t LongitudeMinute = 0;

};

class GPGSVDataString : public GPSDataString
{
public:


	uint8_t GetSatteliteCount() const { return SatteliteCount; }

private:

	virtual bool HeaderCheck(char c, uint8_t position) override;

	virtual bool ReadData(char c, uint8_t FieldNumber, uint8_t position) override;

	virtual void DataCommit() override;

private:
	char SatteliteCountString[SATTELITE_COUNT_STRING_LEN]{ '0', '0' };

	uint8_t SatteliteCount = 0;
};

