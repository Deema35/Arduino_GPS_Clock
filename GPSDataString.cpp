#include "GPSDataString.h"

void GPSDataString::Encode(char c)
{
	switch (State)
	{
	case GPSDataStringState::none:
	case GPSDataStringState::WrongHeader:

		if (c == '$')
		{
			FieldPosition = 0;
			FieldsCount = 0;
			CheckSumm = 0;
			StringCheckSumm = 0;
			ChangeState(GPSDataStringState::HeaderRead);
		}
		
		break;

	case GPSDataStringState::HeaderRead:

		if (c == ',')
		{
			FieldsCount++;
			FieldPosition = 0;
			ChangeState(GPSDataStringState::DataRead);
		}

		else
		{
			if (!HeaderCheck(c, FieldPosition++))
			{
				ChangeState(GPSDataStringState::WrongHeader);
				FieldsCount = 0;
				FieldPosition = 0;
			}
		}

		CheckSumm ^= c;
		break;

	case GPSDataStringState::DataRead:

		if (c == ',')
		{
			FieldsCount++;
			FieldPosition = 0;
			CheckSumm ^= c;
		}
		else if (c == '*')
		{
			FieldsCount++;
			FieldPosition = 0;
			ChangeState(GPSDataStringState::CheckSummRead);
		}

		else 
		{
			ReadData(c, FieldsCount, FieldPosition++);
			CheckSumm ^= c;
		}
		
		
		break;

	case GPSDataStringState::CheckSummRead:

		if (FieldPosition == 2)
		{
			if (CheckSummTest())
			{
				DataCommit();
			}
			

			ChangeState(GPSDataStringState::none);
			FieldsCount = 0;
			FieldPosition = 0;
		}

		else CheckSummRead(c, FieldPosition++);

	}

	return;
	
}

unsigned int GPSDataString::ConvertStringToInt(const char* String, uint8_t StringLenght) const
{
	unsigned int Res = 0;
	int k = 0;
	for (int i = 0; i < StringLenght; i++)
	{
		k = 10 * (StringLenght - 1 - i);
		if (k != 0) Res = Res + (String[i] - '0') * 10 * (StringLenght - 1 - i);
		else Res = Res + (String[i] - '0');
	}

	return Res;
}

int GPSDataString::FromHexCharToInt(char c)
{
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else
		return c - '0';
}

void GPSDataString::CheckSummRead(char c, uint8_t position)
{
	switch (position)
	{
	case 0:
		StringCheckSumm = 16 * FromHexCharToInt(c);
		break;

	case 1:

		StringCheckSumm += FromHexCharToInt(c);
		break;

	}

	
}
//GPGGADataString...................................................


bool GPGGADataString::ReadData(char c, uint8_t FieldNumber, uint8_t position)
{
	switch (FieldNumber)
	{
	case 0:
		return true;

	case 1 :

		if (position < TIME_STRING_LEN)
		{
			TimeString[position] = c;
			if (position == TIME_STRING_LEN - 1)
			{
				TimeCountBeforeCommited = millis();
			}
			
		}

		return true;

	case 2:

		if (position < LATITUDE_STRING_LEN)
		{
			latitude[position] = c;

		}

		return true;

	case 3:
		return true;
	case 4:

		if (position < LONGITUDE_STRING_LEN)
		{
			longitude[position] = c;

			if (position == LONGITUDE_STRING_LEN - 1)
			{
				CoordinateValideBeforeCommited = true;
			}
		}

		return true;

	default:

		return false;

	}
	
	
}


bool GPGGADataString::HeaderCheck(char c, uint8_t position)
{
	switch (position)
	{

	case 0:

		if (c == 'G') return true;
		

		break;
	case 1:

		if (c == 'P') return true;
		

		break;


	case 2:

		if (c == 'G') return true;

		break;

	case 3:

		if (c == 'G')  return true;

		break;

	case 4:

		if (c == 'A')  return true;

		break;
	}

	return false;
}



void GPGGADataString::ResetTimeData()
{
	TimeCount = 0;
}

void GPGGADataString::DataCommit()
{
	TimeInSec = ConvertStringToInt(TimeString, 2) * 3600; //GetHoure
	TimeInSec += ConvertStringToInt(TimeString + 2, 2) * 60; //GetMin
	TimeInSec += ConvertStringToInt(TimeString + 4, 2); //GetSec

	LatitudeDegrees = ConvertStringToInt(latitude, 2); // GetlatitudeDegrees
	LatitudeMinute = ConvertStringToInt(latitude + 2, 2); // GetlatitudeMinute
	
	LongitudeDegrees = ConvertStringToInt(longitude + 1, 2); //GetlongitudeDegrees
	LongitudeMinute = ConvertStringToInt(longitude + 3, 2); //GetlongitudeMinute

	TimeCount = TimeCountBeforeCommited;

	CoordinateValide = CoordinateValideBeforeCommited;
	
}

//GPGSVDataString.............................................................



bool GPGSVDataString::ReadData(char c, uint8_t FieldNumber, uint8_t position)
{
	switch (FieldNumber)
	{
	case 0:
	case 1:
	case 2:
		return true;

	case 3:

		if (position < SATTELITE_COUNT_STRING_LEN)
		{
			SatteliteCountString[position] = c;
		}
		return true;
	

	default:

		return false;

	}


}


bool GPGSVDataString::HeaderCheck(char c, uint8_t position)
{
	switch (position)
	{

	case 0:

		if (c == 'G') return true;


		break;
	case 1:

		if (c == 'P') return true;


		break;


	case 2:

		if (c == 'G') return true;

		break;

	case 3:

		if (c == 'S')  return true;

		break;

	case 4:

		if (c == 'V')  return true;

		break;
	}

	return false;
}

void GPGSVDataString::DataCommit()
{
	SatteliteCount = ConvertStringToInt(SatteliteCountString, 2);
}