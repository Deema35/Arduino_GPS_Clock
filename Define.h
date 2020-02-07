#pragma once
#define ResetTime D0

#define EncoderS1 D2
#define EncoderS2 D3

#define TwoDotCenter D4
#define GPSTX 99 //Not used
#define GPSRX D5
#define H_Clock D6
#define M_Clock D7
#define EnabelMinDisplay D8
#define DisplayBrightness D9
#define EnabelHourSecondDisplay D10
#define Buzzer D11
#define EnanbelGPS D12
#define EnabelHourFistDisplay D13

#define EncodeClick A0
#define AllaramEnableLight A1
#define AllaramEnableButton A2
#define SnoozeButton A3
#define LightSensorSDA A4
#define LightSensorSCL A5

#define DebounceButtonClickDelayMiliSec 100 
#define ChangeStaeClockDelayMiliSec 500 //Delay between change state if state blocked
#define SinhronizationDelaySec  86400 //Deleay between time syncronization in seconds 86400
#define AlaramAllertDurationMin 1 
#define DelayFromLastDisplayModeSwitchMiliSec 10000




namespace EClockState
{
	enum Value 
	{
		Normal,
		Synchronization,
		SetAlaramHour,
		SetAlaramMin,
		SetTimeZone,
		SetSnoozeTime,
		AlaramAlert,
		AlaramSnooz,
		GPSTracing,
		end
	};

};

namespace EDisplayMode
{
	enum Value
	{
		Auto,
		TwoNumbers,
		NotDisplay,
		end
	};

};


namespace EEEPROMAddr
{
	enum Value
	{
		AlaramEnabel,
		AlaramHour,
		AlaramMin,
		TimeZoneDelay,
		SnoozDelay,
		end
	};
}

namespace EButtonState
{
	enum Value
	{
		UnPressed, 
		Pressed,
		Holding,
		end
	};
}

namespace GPSTracingDisplayMode
{
	enum Value
	{
		SateliteCount,
		latitude,
		longitude
	};
}

namespace NormaStaelDisplayMode
{
	enum Value
	{
		Time,
		SynchronizationTime,
		AlarmTime
	};
}

namespace GPSDataStringState
{
	enum Value
	{
		none,
		HeaderRead,
		DataRead,
		WrongHeader,
		CheckSummRead
	};
}