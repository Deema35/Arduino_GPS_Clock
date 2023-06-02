#include <Arduino.h>
#include "Define.h"
#include "GPS_Clock.h"


static GPS_Clock Clock;

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

	attachInterrupt(0, Clock.Encoder.S1Function, FALLING);
	attachInterrupt(1, Clock.Encoder.S2Function, FALLING);


	Clock.Setup();
	
	

	Serial.println("Setup complet");
	Serial.println("");
	
}

// Add the main program code into the continuous loop() function

void loop()
{
	Clock.Loop();

}


