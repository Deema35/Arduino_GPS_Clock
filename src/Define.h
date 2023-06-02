#pragma once
#define ResetTime 0

#define EncoderS1 2
#define EncoderS2 3

#define TwoDotCenter 4
#define GPSTX 99 //Not used
#define GPSRX 5
#define H_Clock 6
#define M_Clock 7
#define EnabelMinDisplay 8
#define DisplayBrightness 9
#define EnabelHourSecondDisplay 10
#define Buzzer 11
#define EnanbelGPS 12
#define EnabelHourFistDisplay 13

#define EncodeClick 14
#define AllaramEnableLight 15
#define AllaramEnableButton 16
#define SnoozeButton 17
#define LightSensorSDA 18
#define LightSensorSCL 19

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