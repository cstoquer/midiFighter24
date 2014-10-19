#include "MIDI.h"
#include "DualDigitDisplay.h"

#define PULSE_WIDTH_USEC 5
#define DEBOUNCE_TRIGGER 8

#define LOAD  2
#define CLOCK 3
#define READ1 4
#define READ2 5
#define READ3 6

int   pinStates[24];  // state values of all 24 pins
int   changed[24];    // a list of button with changed state in the current loop
int   debounce[24];   // debounce counter values of all 24 pins
bool  debouncing[24]; // pins currently in debounce mode

DualDigitDisplay display;

const int pinToPadMap[24] = {
	2,  50, 51, 3,  52, 53, 6, 55, 56, 7, 58, 59,
	0, 60, 61, 1, 63, 64, 4, 66, 67, 5, 69, 70
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
				display.displayNumber(pinToPadMap[changed[i]], 0, 0);
			} else {
				MIDI.sendNoteOff(pinToPadMap[changed[i]] + 36, 0, 1);
			}
		}
	}
}

