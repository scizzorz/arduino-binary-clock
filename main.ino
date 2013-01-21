#include "digitalWriteFast.h"
#define PULSE 150
#define BRIGHTNESS 6

int pins[] = {6, 5, 4, 3, 12, 11, 10, 9, 8, 7};
int duty[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int dutyMod[] = {PULSE/BRIGHTNESS, PULSE, PULSE/BRIGHTNESS, PULSE/BRIGHTNESS, PULSE/BRIGHTNESS, PULSE, PULSE/BRIGHTNESS, PULSE/BRIGHTNESS, PULSE/BRIGHTNESS, PULSE};
int buttonMinutes=A1;
int buttonHours=A0;

int seconds=0;
int minutes=0;
int hours=0;
int i;

unsigned long c=0;
unsigned long pause=0;
unsigned long time;

void setup() {
	for(i=0;i<10;i++) pinMode(pins[i],OUTPUT);
	pinMode(buttonMinutes,INPUT);
	pinMode(buttonHours,INPUT);
}

void loop() {
	time = micros();
	pause = 1000140;
	seconds++;

	if(digitalRead(buttonHours)) {
		hours++;
		seconds = 0;
		pause = 250140;
	}
	if(digitalRead(buttonMinutes)) {
		minutes++;
		seconds = 0;
		pause = 250140;
	}

	minutes += seconds/60;
	seconds = seconds%60;

	hours += minutes/60;
	minutes = minutes%60;

	hours = hours%12;


	int dh=hours;
	if(hours==0) dh=12;
	for(i=0;i<4;i++) duty[i] = ((dh>>i)&1)*dutyMod[i];
	for(i=0;i<6;i++) duty[i+4] = ((minutes>>i)&1)*dutyMod[i];
	if(!(dh & 8) && (seconds & 1)) duty[3] = 1;
	if(!(minutes & 8) && (seconds & 1)) duty[7] = 1;

	c=0;
	do {
		for(i=0;i<10;i++) {
			digitalWriteFast(pins[i], c < duty[i]);
		}
		if(++c == PULSE) c=0;
	} while(micros() - time < pause);
}
