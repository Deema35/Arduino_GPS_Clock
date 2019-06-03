#pragma once
#include <Arduino.h>

class GPSDataString
{

public:

	void Encode(char c);

protected:

	virtual bool HeaderCheck(char c, uint8_t position) = 0;

	virtual bool ReadData(char c, uint8_t FieldNumber, uint8_t position) = 0;

	unsigned int ConvertStringToInt(const char* String, uint8_t StringLenght) const;

private:
	uint8_t FieldPosition = 0;

	uint8_t FieldsCount = 0;

	

};


class GPGGADataString : public GPSDataString
{
public:

	bool IsTimeValid() const { return TimeValid; }

	bool IsCoordinateValide() const { return CoordinateValide; }

	uint8_t GetHour() const;

	uint8_t GetMin() const;

	uint8_t GetSec() const;

	unsigned long GetTimeCount() const { return TimeCount; }


	uint8_t GetlatitudeDegrees() const;

	uint8_t GetlatitudeMinute() const;

	uint8_t GetlongitudeDegrees() const;

	uint8_t GetlongitudeMinute() const;

protected:

	virtual bool HeaderCheck(char c, uint8_t position) override;

	virtual bool ReadData(char c, uint8_t FieldNumber, uint8_t position) override;

private:

	bool TimeValid = false;

	unsigned long TimeCount = 0;

	uint8_t TimeStringPosition = 0;

	char TimeString[6];

	char latitude[4]{ '0', '0',  '0', '0' };

	char longitude[5]{ '0', '0', '0', '0', '0' };

	bool CoordinateValide = false;

};

class GPGSVDataString : public GPSDataString
{
public:


	uint8_t GetSatteliteCount() const;

protected:

	virtual bool HeaderCheck(char c, uint8_t position) override;

	virtual bool ReadData(char c, uint8_t FieldNumber, uint8_t position) override;
private:
	char SatteliteCountString[2]{ '0', '0' };

};

