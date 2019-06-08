
#include "Define.h"
#include "GPS_Clock.h"

volatile int EncoderTurnCounter = 0;

volatile bool S1EncoderTick = false;

volatile bool S2EncoderTick = false;

GPS_Clock Clock(EncoderTurnCounter);

// The setup() function runs once each time the micro-controller starts
void setup()
{
  
	TCCR1A = TCCR1A & 0xe0 | 1;
	TCCR1B = TCCR1B & 0xe0 | 0x0b; // PWM on D9 and D10  976,56 Гц


	
	Serial.begin(9600);
	UCSR0B &= ~bit(RXEN0);  // disable resive
	

	pinMode(EncodeClick, INPUT);
	pinMode(AllaramEnableButton, INPUT_PULLUP);

	pinMode(H_Clock, OUTPUT);
	pinMode(M_Clock, OUTPUT);
	pinMode(EnabelHourFistDisplay, OUTPUT);
	pinMode(EnabelHourSecondDisplay, OUTPUT);
	pinMode(EnabelMinDisplay, OUTPUT);
	pinMode(EnanbelGPS, OUTPUT);
	pinMode(ResetTime, OUTPUT);

	pinMode(Buzzer, OUTPUT);

	pinMode(AllaramEnableLight, OUTPUT);

	pinMode(TwoDotCenter, OUTPUT);
	pinMode(SnoozeButton, INPUT);

	pinMode(EncoderS1, INPUT);
	pinMode(EncoderS2, INPUT);

	attachInterrupt(0, EncoderS1ClickFunk, FALLING);
	attachInterrupt(1, EncoderS2ClickFunk, FALLING);


	Clock.Setup();
	
	

	Serial.println("Setup complet");
	Serial.println("");
	
}

// Add the main program code into the continuous loop() function





void loop()
{
	Clock.Loop();

}

void EncoderS1ClickFunk()
{
	if (S2EncoderTick)
	{
		if (!digitalRead(EncoderS2))
		{
			EncoderTurnCounter++;
		}
		{
			S1EncoderTick = true;
		}
		
		S2EncoderTick = false;
	}
	else
	{
		S1EncoderTick = true;
	}
}

void EncoderS2ClickFunk()
{
	if (S1EncoderTick)
	{
		if (!digitalRead(EncoderS1))
		{
			EncoderTurnCounter--;
		}
		{
			S2EncoderTick = true;
		}
		S1EncoderTick = false;
	}
	else
	{
		S2EncoderTick = true;
	}
	
}
