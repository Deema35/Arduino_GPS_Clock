#include "GPSDataString.h"

void GPSDataString::Encode(char c)
{
	if (FieldsCount == 0 && FieldPosition == 0)
	{
		if (c == '$')
		{
			FieldPosition++;
		}

		return;
	}

	if (c == ',')
	{
		FieldsCount++;
		FieldPosition = 0;
		return;
	}

	if (FieldsCount == 0)
	{
		if (!HeaderCheck(c, FieldPosition++))
		{
			FieldsCount = 0;
			FieldPosition = 0;
		}

		return;
	}

	if (!ReadData(c, FieldsCount, FieldPosition++))
	{
		
		FieldsCount = 0;
		FieldPosition = 0;
		return;
	}
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

		if (position < 6)
		{
			TimeString[position] = c;
			if (position == 5)
			{
				TimeValid = true;
				TimeCount = millis();
			}
			
		}

		return true;

	case 2:

		if (position < 4)
		{
			latitude[position] = c;

		}

		return true;

	case 3:
		return true;
	case 4:

		if (position < 5)
		{
			longitude[position] = c;

			if (position == 4)
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

	case 1:

		if (c == 'G') return true;
		

		break;
	case 2:

		if (c == 'P') return true;
		

		break;


	case 3:

		if (c == 'G') return true;

		break;

	case 4:

		if (c == 'G')  return true;

		break;

	case 5:

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

		if (position < 2)
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

	case 1:

		if (c == 'G') return true;


		break;
	case 2:

		if (c == 'P') return true;


		break;


	case 3:

		if (c == 'G') return true;

		break;

	case 4:

		if (c == 'S')  return true;

		break;

	case 5:

		if (c == 'V')  return true;

		break;
	}

	return false;
}