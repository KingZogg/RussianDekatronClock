﻿

#define Guide1 5 //Guide
#define Guide2 6 //Guide
#define katmain 3 //Common Cathode ?
#define katmain0 2 //Index Cathode ?
#define DECA_IMPULS_DELAY 50 //uS
char deka_step;
byte brightness;



char mode;
//point from 0 to 31 - values ​​from 0 to 31
#define mode_phase0 128
#define mode_phase1 129



#define data_input0 A0
#define data_input1 A1
#define data_input2 A2
#define data_input3 A3
#define data_input4 A4
#define data_input5 A5
#define data_input6 11 //PB3
#define data_input7 12 //PB4


void dekatron_setup() {
	// initialize the digital pin as an output.
	pinMode(Guide1, OUTPUT);
	pinMode(Guide2, OUTPUT);
	pinMode(katmain, OUTPUT);
	pinMode(katmain0, OUTPUT);
	digitalWrite(Guide1, LOW);
	digitalWrite(Guide2, LOW);
	digitalWrite(katmain, LOW);
	digitalWrite(katmain0, LOW);
	deka_step = 0;
	brightness = 255;
}

void dekatron_on() {
	analogWrite(katmain, brightness);
	deka_step = 0;
}

void dekatron_off() {
	digitalWrite(katmain, LOW);
	digitalWrite(katmain0, LOW);
	digitalWrite(Guide1, LOW);
	digitalWrite(Guide2, LOW);
}

void dekatron_res() {
	digitalWrite(Guide1, LOW);
	digitalWrite(Guide2, LOW);
	analogWrite(katmain0, 255);
	digitalWrite(katmain, LOW);
	delayMicroseconds(DECA_IMPULS_DELAY);
	delayMicroseconds(DECA_IMPULS_DELAY);
	delayMicroseconds(DECA_IMPULS_DELAY);
	delayMicroseconds(DECA_IMPULS_DELAY);
	delayMicroseconds(DECA_IMPULS_DELAY);
	delayMicroseconds(DECA_IMPULS_DELAY);
	delayMicroseconds(DECA_IMPULS_DELAY);
	delayMicroseconds(DECA_IMPULS_DELAY);
	delayMicroseconds(DECA_IMPULS_DELAY);
	analogWrite(katmain, brightness);
	delayMicroseconds(DECA_IMPULS_DELAY);
	digitalWrite(katmain0, LOW);
	deka_step = 0;
}

void dekatron_plus() {
	deka_step++;
	deka_step = deka_step % 3;
	if (deka_step == 1)
	{
		analogWrite(Guide1, brightness);
		delayMicroseconds(DECA_IMPULS_DELAY);
		digitalWrite(katmain, LOW);
		digitalWrite(Guide2, LOW);
	};
	if (deka_step == 2)
	{
		analogWrite(Guide2, brightness);
		delayMicroseconds(DECA_IMPULS_DELAY);
		digitalWrite(katmain, LOW);
		digitalWrite(Guide1, LOW);
	};
	if (deka_step == 0)
	{
		analogWrite(katmain, brightness);
		delayMicroseconds(DECA_IMPULS_DELAY);
		digitalWrite(Guide1, LOW);
		digitalWrite(Guide2, LOW);
	};
	delayMicroseconds(DECA_IMPULS_DELAY);

}

void dekatron_minus() {
	deka_step += 2;
	deka_step = deka_step % 3;
	if (deka_step == 1)
	{
		analogWrite(Guide1, brightness);
		delayMicroseconds(DECA_IMPULS_DELAY);
		digitalWrite(katmain, LOW);
		digitalWrite(Guide2, LOW);
	};
	if (deka_step == 2)
	{
		analogWrite(Guide2, brightness);
		delayMicroseconds(DECA_IMPULS_DELAY);
		digitalWrite(Guide1, LOW);
		digitalWrite(katmain, LOW);
	};
	if (deka_step == 0)
	{
		analogWrite(katmain, brightness);
		delayMicroseconds(DECA_IMPULS_DELAY);
		digitalWrite(Guide1, LOW);
		digitalWrite(Guide2, LOW);
	};
	delayMicroseconds(DECA_IMPULS_DELAY);
}


// the setup routine runs once when you press reset:
void setup()
{


	// brightness setting
	TCCR0B = TCCR0B & 0b11111000 | 0x01; //set 62000 Hz  pwm
	TCCR1B = TCCR1B & 0b11110000 | 0b00001001; //set 62000 Hz  pwm
	TCCR2A = TCCR2A & 0b11111101 | 0x02;
	TCCR2B = TCCR2B & 0b11111000 | 0x01; //set 62000 Hz  pwm

	pinMode(data_input0, INPUT_PULLUP);
	pinMode(data_input1, INPUT_PULLUP);
	pinMode(data_input2, INPUT_PULLUP);
	pinMode(data_input3, INPUT_PULLUP);
	pinMode(data_input4, INPUT_PULLUP);
	pinMode(data_input5, INPUT_PULLUP);
	pinMode(data_input6, INPUT_PULLUP);
	pinMode(data_input7, INPUT_PULLUP);
	dekatron_setup();
	dekatron_res();
	dekatron_off();
	//Serial.begin(57600);
}

char half = 0;
byte sec1, sec2;

// the loop routine runs over and over again forever:
void loop() {
	while (1)
	{
		char i;
		char sec;
		int input;
		int accel;
		accel = 2000;


		input = PINC & B00111111;
		input = input | ((PINB & B00011000) * 8); //pb3 = pin11, pb4 = pin12
												  // Serial.println(input);
												  // draw a point and dump
		if ((input >= 0x60) && (input < 0x80)) //0x60 = 96 Decimal 0x80 = 128

		{
			if (mode != (input - 96))
			{
				brightness = 255 / 12;
				mode = (input - 96);
				// draw a point of degree
				dekatron_res();
				for (byte bbb = 0; bbb < mode; bbb++);
				dekatron_minus();
			}
			delay(30);
			continue;
		}

		// Otherwise we draw stripes
		// if the inputs are not connected (they are tightened to the plus), then instead of seconds the screensaver
		//if (input == 0xFF) sec = (millis() / 500); //delay effect
		//if (input == 0xFF) sec = (millis() / 60000) % 60; //roughly seconds
		if (input == 0xFF) sec = (millis() / accel) % 60;


		// oh, we have an ordinary second in binary form at the entrance. we can easily identify them and draw them
		if (input < 0x60) //less than 96 Decimal
		{
			sec = input & B00001111;
			sec += ((input & B11110000) / 16) * 10;
		};
		//Serial.println((int)sec);

		// here probably a dynamic indication. The first two bits of 10xxxxxx tell us about this
		// catch first the first digit, and then the second
		if ((input >= 0x80) && (input < 0xC0)) //Greater than or equal to 128 Decimal AND Less than 192 Decimal
		{
			if (
				(input & B00100000) &&
				((input & B00001111) < 6)
				)
				sec1 = input & B00001111;
			sec2 = 11;
			if (
				(input & B00010000) &&
				((input & B00001111) < 10)
				)
				sec2 = input & B00001111;
			sec = sec1 * 10 + sec2;
		};

		brightness = 255;
		if (sec == 0)
		{
			mode = mode_phase0;
			dekatron_off();
		};
		if (sec == 1)
		{
			brightness = 255 / 12;
			mode = mode_phase0;
			dekatron_res();
		};
		if ((sec<30))
		{
			if ((sec < 12) && (sec > 0)) brightness = sec * (255 / 12);
			mode = mode_phase0;
			half = 0;
			for (i = 1; i < sec; i++)
			{
				dekatron_plus();
			}

			for (i = 1; i < sec; i++)
			{
				dekatron_minus();
			}
		}
		else
		{
			if (sec > 47) brightness = (60 - sec) * (255 / 12);

			if (half == 0)
			{
				dekatron_minus();
			};
			half = 1;
			mode = mode_phase1;
			for (i = (60 - sec); i >1; i--)
			{
				dekatron_minus();
			}

			for (i = (60 - sec); i >1; i--)
			{
				dekatron_plus();
			}
		}

	}
}