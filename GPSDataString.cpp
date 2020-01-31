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

		else if(!HeaderCheck(c, FieldPosition++))
		{
			ChangeState(GPSDataStringState::WrongHeader);
			FieldsCount = 0;
			FieldPosition = 0;
		}

		break;

	case GPSDataStringState::DataRead:

		if (c == ',')
		{
			FieldsCount++;
			FieldPosition = 0;
		}

		else if (!ReadData(c, FieldsCount, FieldPosition++))
		{

			ChangeState(GPSDataStringState::none);
			FieldsCount = 0;
			FieldPosition = 0;
		}

		break;

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
				TimeValid = true;
				TimeCount = millis();
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
				CoordinateValide = true;
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

uint8_t GPGGADataString::GetHour() const
{

	return ConvertStringToInt(TimeString, 2);
	
}

uint8_t GPGGADataString::GetMin() const
{
	return ConvertStringToInt(TimeString + 2, 2);
}

uint8_t GPGGADataString::GetSec() const
{
	return ConvertStringToInt(TimeString + 4, 2);
}



uint8_t GPGGADataString::GetlatitudeDegrees() const
{
	return ConvertStringToInt(latitude, 2);
	
}

uint8_t GPGGADataString::GetlatitudeMinute() const
{
	return ConvertStringToInt(latitude + 2, 2);
}

uint8_t GPGGADataString::GetlongitudeDegrees() const
{
	return ConvertStringToInt(longitude + 1, 2);
}

uint8_t GPGGADataString::GetlongitudeMinute() const
{
	return ConvertStringToInt(longitude + 3, 2);
}

void GPGGADataString::ResetTimeData()
{
	TimeValid = false;

	TimeCount = 0;
}

//GPGSVDataString.............................................................

uint8_t GPGSVDataString::GetSatteliteCount() const
{

	return ConvertStringToInt(SatteliteCountString, 2);

}

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