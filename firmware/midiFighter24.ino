#include "MIDI.h"
#include "DualDigitDisplay.h"

//-------------------------------------------
//                ARDUINO TYPES
//---------+----------------------+---------+
// SIGNED  |  UNSIGNED            | SIZE    |
//---------+----------------------+---------+
// char    |  byte                | 8 bits  |
// int     |  word, unsigned int  | 16 bits |
// long    |  unsigned long       | 32 bits |
//---------+----------------------+---------+
// float, double                  | 32 bits |
//--------------------------------+---------+

#define PULSE_WIDTH_USEC 5
#define DEBOUNCE_TRIGGER 8


int   LOAD  = 2;
int   CLOCK = 3;
int   READ1 = 4;
int   READ2 = 5;
int   READ3 = 6;

int   pinStates[24];  // state values of all 24 pins
int   changed[24];    // a list of button with changed state in the current loop
int   debounce[24];   // debounce counter values of all 24 pins
bool  debouncing[24]; // pins currently in debounce mode


DualDigitDisplay display;

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
// 23 24 20 19  |  15 16 12 11  |   7  8  4  3 
// -------------+---------------+--------------
// 12,15,18,21  |  13,16,19,22  |  14,17,20,23
//    CHIP 1    |     CHIP 2    |     CHIP 3
//  9, 6, 3, 0  |  10, 7, 4, 1  |  11, 8, 5, 2
// -------------+---------------+--------------
// 22 21 17 18  |  14 13  9 10  |   6  5  1  2 
//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

int pinToPadMap[24] = {
	17,  9, 1,
	16,  8, 0,
	20, 12, 4,
	21, 13, 5,
	//////////
	22, 14, 6,
	23, 15, 7,
	19, 11, 3,
	18, 10, 2
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void setup() {
	pinMode(LOAD,  OUTPUT);
	pinMode(CLOCK, OUTPUT);
	pinMode(READ1, INPUT);
	pinMode(READ2, INPUT);
	pinMode(READ3, INPUT);

	for (int i = 0; i < 24; ++i) {
		pinStates[i]  = 0;
		changed[i]    = 0;
		debounce[i]   = 0;
		debouncing[i] = false;
	}

	display.setup();
	MIDI.begin();

	display.clear();
}

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

int readButtons() {
	int nChanged = 0;
	//-------------------------------------
	// store pin values for all shift register
	digitalWrite(LOAD, LOW);
	delayMicroseconds(PULSE_WIDTH_USEC); // TODO: here, insert some computation
	digitalWrite(LOAD, HIGH);

	for (int pin = 0; pin < 24; ++pin) {
		//-------------------------------------
		// read shift registers values
		int value1 = digitalRead(READ1);
		int value2 = digitalRead(READ2);
		int value3 = digitalRead(READ3);
		digitalWrite(CLOCK, HIGH);
		// delayMicroseconds(PULSE_WIDTH_USEC);

		//-------------------------------------
		// update and debounce value 1
		if (pinStates[pin] != value1) {
			// pin changed value
			pinStates[pin] = value1;
			// switch debouncing
			// - if previously bebouncing, then it was an artefact
			// - if not, then debouncing start
			debouncing[pin] = !debouncing[pin];
			// initiate debounce counter for this pin
			debounce[pin] = 0;
		} else if (debouncing[pin]) {
			// increment debouncing counter value for this pin
			if (debounce[pin]++ == DEBOUNCE_TRIGGER) {
				// debounce is finished, button state has changed
				changed[nChanged++] = pin;
				debouncing[pin] = false;
			}
		}

		//-------------------------------------
		// update and debounce value 2
		if (pinStates[++pin] != value2) {
			pinStates[pin] = value2;
			debouncing[pin] = !debouncing[pin];
			debounce[pin] = 0;
		} else if (debouncing[pin]) {
			if (debounce[pin]++ == DEBOUNCE_TRIGGER) {
				changed[nChanged++] = pin;
				debouncing[pin] = false;
			}
		}

		//-------------------------------------
		// update and debounce value 3
		if (pinStates[++pin] != value3) {
			pinStates[pin] = value3;
			debouncing[pin] = !debouncing[pin];
			debounce[pin] = 0;
		} else if (debouncing[pin]) {
			if (debounce[pin]++ == DEBOUNCE_TRIGGER) {
				changed[nChanged++] = pin;
				debouncing[pin] = false;
			}
		}

		//-------------------------------------
		digitalWrite(CLOCK, LOW);
	}
	return nChanged;
}


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void loop() {
	int nChanged = readButtons();
	if (nChanged) {
		for (int i = 0; i < nChanged; ++i) {
			if (pinStates[changed[i]]) {
				int note = pinToPadMap[changed[i]] + 36;
				MIDI.sendNoteOn(note, 120, 1);
				display.displayNote(note);
			} else {
				MIDI.sendNoteOff(pinToPadMap[changed[i]] + 36, 0, 1);
			}
		}
	}
}

