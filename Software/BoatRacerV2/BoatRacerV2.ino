/*
    Name:       BoatRacerV2.ino
    Created:	2019-01-21 3:46:09 PM
    Author:     AKODO-THINK\Akodo
*/

#include <EEPROM.h>
#include <SoftwareSerial.h>

// Pin Declarations
int P1Laser = A0;
int P2Laser = A1;
int P1Slam = 2;
int P2Slam = 8;
int P1Ready = 3;
int P2Ready = 7;
int PSel1 = 4;
int PSel2 = 5;
int Start = 6;
int ERec = 10;
int Buzz = 11;
int LCDTx = 12;
int LCDRx = 13;

unsigned int SRec = 9999;
unsigned int ATRec = 9999;

unsigned long P1Start = 0;
unsigned long P2Start = 0;
unsigned long P1End = 0;
unsigned long P2End = 0;
unsigned long BuzzTime = 0;
unsigned int P1Time = 0;
unsigned int P2Time = 0;

float DecTime1 = 0.0;
float DecTime2 = 0.0;

char LowerBuff[16]; 

int Flag1 = 0;
int Flag2 = 0;

unsigned int Thresh = 500;
unsigned int SlamDelay = 500;
unsigned int tonefreq = 2900;

SoftwareSerial LCD(LCDRx, LCDTx);

void P1Init()
{
	Serial.write("P1Init");
	
	LCD.write(254);
	LCD.write(1);
	
	LCD.write("    Place Cup   ");
	LCD.write("    To Start    ");
	
	Flag1 = 0;
	
	while((Flag1 == 0)) {
		int val1 = analogRead(A0);
		if (val1 < Thresh) {
			Flag1 = 1;
		}
	}
}

void P1Race()
{
	Serial.write("P1Race");
	
	LCD.write(254);
	LCD.write(1);
	
	LCD.write("Drink when ready");
	
	Flag1 = 0;
	
	while((Flag1 == 0)) {
		int val1 = analogRead(A0);
		if (val1 > Thresh) {
			Flag1 = 1;
		}
	}
	
	LCD.write(254);
	LCD.write(1);
	LCD.write("     Racing     ");
	
	P1Start = millis();
	
	Flag1 = 0;
		
	while((Flag1 == 0)) {
		if (digitalRead(P1Slam)==LOW) {
			Flag1 = 1;
		}
	}
	
	P1End = millis();
	
	P1Time = P1End - P1Start;
	
}

void P1Results()
{
	Serial.write("P1Results");
	
	DecTime1 = P1Time/1000.0;	
	if (P1Time<ATRec)
	{
		LCD.write(254);
		LCD.write(1);
		LCD.write("All Time Record!");
 
		dtostrf(DecTime1,5, 3, LowerBuff);
		LCD.write("      ");
		LCD.write(LowerBuff);
		
		SRec = P1Time;
		ATRec = P1Time;
		
		EEPROM.write(0,highByte(P1Time));
		EEPROM.write(1,lowByte(P1Time));		
	}
	else if (P1Time<SRec)
	{
		LCD.write(254);
		LCD.write(1);
		LCD.write("Session Record!");
		
		dtostrf(DecTime1,5, 3, LowerBuff);
		LCD.write("      ");
		LCD.write(LowerBuff);
		
		SRec = P1Time;
		}
	else if (P1Time<3000)
	{
		LCD.write(254);
		LCD.write(1);
		LCD.write("   Very Good!   ");
		
		dtostrf(DecTime1,5, 3, LowerBuff);
		LCD.write("      ");
		LCD.write(LowerBuff);
	}
	else 
	{
		LCD.write(254);
		LCD.write(1);
		LCD.write("    Not Bad     ");
		
		dtostrf(DecTime1,5, 3, LowerBuff);
		LCD.write("      ");
		LCD.write(LowerBuff);
	}
	
	delay(5000);
	
	LCD.write(254);
	LCD.write(1);

	LCD.write("  Ready to Race ");
	LCD.write("Select 1P or 2P!");
}

void P2Init()
{
	Serial.write("P2Init");
	
	LCD.write(254);
	LCD.write(1);
	
	LCD.write("    Place Cups  ");
	LCD.write("    To Start    ");
	
	Flag1 = 0;
	Flag2 = 0;
	
	while((Flag1 == 0) || (Flag2 == 0)) {
		int val1 = analogRead(A0);
		if (val1 < Thresh) {
			Flag1 = 1;
		}
		int val2 = analogRead(A1);
		if (val2 < Thresh) {
			Flag2 = 1;
		}
	}
}

void P2Countdown()
{
	Serial.write("P2Race");
	
	LCD.write(254);
	LCD.write(1);
	LCD.write("  Press Button  ");
	LCD.write("   When Ready   ");
	
	Flag1 = 0;
	Flag2 = 0;
	
	while((Flag1 == 0) || (Flag2 == 0)) {
		if (digitalRead(P1Ready) == LOW) {
			Flag1 = 1;
		}
		if (digitalRead(P2Ready) ==  LOW) {
			Flag2 = 1;
		}
	}
	
	LCD.write(254);
	LCD.write(1);
	LCD.write("       5        ");
	
	delay(1000);
	
	LCD.write(254);
	LCD.write(1);
	LCD.write("       4        ");
	
	delay(1000);
	
	LCD.write(254);
	LCD.write(1);
	LCD.write("       3        ");
	
	tone(Buzz, tonefreq);
	
	delay(500);
	
	noTone(Buzz);
	
	delay(500);
	
	LCD.write(254);
	LCD.write(1);
	LCD.write("       2        ");
	
	tone(Buzz, tonefreq);
	
	delay(500);
	
	noTone(Buzz);
	
	delay(500);
	
	LCD.write(254);
	LCD.write(1);
	LCD.write("       1        ");
	
	tone(Buzz, tonefreq);

	delay(500);

	noTone(Buzz);
	
	delay(500);
	
}

void P2Race()
{

	
	Flag1 = 0;
	Flag2 = 0;
	
	int val1 = analogRead(A0);
	int val2 = analogRead(A1);
	
	if ((val1 > Thresh) || (val2 > Thresh)) {
		Flag1 = 1;
		Flag2 = 1;
		LCD.write(254);
		LCD.write(1);
		LCD.write("   False Start  ");	
		
		P1End = P1Start;
		P2End = P2Start;
		
		delay(3000);
	}
	else {
			P1Start = millis();
			P2Start = P1Start;
			tone(Buzz, tonefreq);
			BuzzTime = millis();
			
			LCD.write(254);
			LCD.write(1);
			LCD.write("     Racing     ");
	}
	
	while((Flag1 == 0) || (Flag2 == 0)) {
		if (millis() >= (BuzzTime+SlamDelay)) {
			if ((digitalRead(P1Slam) == LOW) && Flag1 == 0) {
				Flag1 = 1;
				P1End = millis();
			}
			if ((digitalRead(P2Slam) ==  LOW) && Flag2 == 0){
				Flag2 = 1;
				P2End = millis();
			}
			if (millis() > (BuzzTime + 1000)) {
				noTone(Buzz);
			}
		}	
	}
	noTone(Buzz);
}

void P2Results()
{
	P1Time = P1End-P1Start;
	P2Time = P2End-P2Start;
	DecTime1 = P1Time/1000.0;
	DecTime2 = P2Time/1000.0;
	
	LCD.write(254);
	LCD.write(1);
	
	if(P1Time==P2Time) {
		LCD.write("      Tie       ");
	} 
	else if(P1Time<P2Time) {
		LCD.write("Winner          ");
	}
	else {
		LCD.write("          Winner");
	}
	
	dtostrf(DecTime1,5, 3, LowerBuff);
	LCD.write(LowerBuff);
	LCD.write("    ");
	dtostrf(DecTime2,5, 3, LowerBuff);
	LCD.write(LowerBuff);
	
	delay(5000);
	
	LCD.write(254);
	LCD.write(1);

	LCD.write("  Ready to Race ");
	LCD.write("Select 1P or 2P!");
}

void setup()
{
pinMode(P1Slam,INPUT_PULLUP);
pinMode(P2Slam,INPUT_PULLUP);
pinMode(P1Ready,INPUT_PULLUP);
pinMode(P2Ready,INPUT_PULLUP);
pinMode(PSel1,INPUT_PULLUP);
pinMode(PSel2,INPUT_PULLUP);
pinMode(Start,INPUT_PULLUP);
pinMode(ERec,INPUT_PULLUP);
pinMode(Buzz,OUTPUT);

LCD.begin(9600);
Serial.begin(115200);

delay(4000);

LCD.write(254);
LCD.write(1);

LCD.write("    SPD AOE     ");
LCD.write("  Boat Racer V2 ");

delay(2000);

LCD.write(254);
LCD.write(1);

byte high = EEPROM.read(0);
byte low = EEPROM.read(1);
ATRec=word(high,low);

LCD.write("  All Time Best ");

DecTime1 = ATRec/1000.0;

dtostrf(DecTime1,5, 3, LowerBuff);
LCD.write("      ");
LCD.write(LowerBuff);

delay(5000);

LCD.write(254);
LCD.write(1);

LCD.write("  Ready to Race ");
LCD.write("Select 1P or 2P!");
}

void loop()
{
if ((digitalRead(Start) == LOW) && (digitalRead(PSel1) == LOW)){
	P1Init();
	P1Race();
	P1Results();
} else if ((digitalRead(Start) == LOW) && (digitalRead(PSel2) == LOW)) {
	P2Init();
	P2Countdown();
	P2Race();
	P2Results();
}
if (digitalRead(ERec) == LOW) {
	ATRec = 9999;
	EEPROM.write(0,highByte(ATRec));
	EEPROM.write(1,lowByte(ATRec));
	delay(1000);
}

}