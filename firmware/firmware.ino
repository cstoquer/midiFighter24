#include "MIDI.h"
#include "DualDigitDisplay.h"
#include "Pad.h"

#define PULSE_WIDTH_USEC 5
#define ANALOG_READ_RATE 50
#define DEBOUNCE_TRIGGER 8

#define LOAD  2
#define CLOCK 3
#define READ1 4
#define READ2 5
#define READ3 6

#define FOOT_SWITCH 7
#define SHIFT_A 12
#define SHIFT_B 11

#define SHIFT_A_BIT 24
#define SHIFT_B_BIT 25

int   pinStates[26];  // state values of all 24 button pins
int   changed[26];    // a list of button with changed state in the current loop
int   debounce[26];   // debounce counter values of all 24 pins
bool  debouncing[26]; // pins currently in debounce mode
Pad   pads[24];

// long  footControlRead;    // analog read value of connected foot controller
// long  footControlValue;   // analog current value of connected foot controller
// int   footTimer;

int   selectedOctave;

DualDigitDisplay display;

const int pinToPadMap[24] = {
	2, 14, 23, 3, 15, 19, 6, 10, 22, 7, 11, 18,
	0, 13, 17, 1, 12, 21, 4,  9, 16, 5,  8, 20
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
void setupPads(int octave) {
	for (int i = 0; i < 24; ++i) {
		int note = 12 + octave * 24 + i;
		pads[i].setNote(note);
	}
}

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void setup() {
	pinMode(LOAD,        OUTPUT);
	pinMode(CLOCK,       OUTPUT);
	pinMode(READ1,       INPUT);
	pinMode(READ2,       INPUT);
	pinMode(FOOT_SWITCH, INPUT);
	pinMode(SHIFT_A,     INPUT);
	pinMode(SHIFT_B,     INPUT);

	for (int i = 0; i < 24; ++i) {
		pinStates[i]  = 0;
		changed[i]    = 0;
		debounce[i]   = 0;
		debouncing[i] = false;
	}

	// footControlRead = 0;
	// footControlValue = 0;
	// footTimer = ANALOG_READ_RATE;

	selectedOctave = 1;
	setupPads(selectedOctave);

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
	display.click();

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


	// TODO: refactor this

	int shiftA = digitalRead(SHIFT_A);
	if (pinStates[SHIFT_A_BIT] != shiftA) {
		pinStates[SHIFT_A_BIT] = shiftA;
		debouncing[SHIFT_A_BIT] = !debouncing[SHIFT_A_BIT];
		debounce[SHIFT_A_BIT] = 0;
	} else if (debouncing[SHIFT_A_BIT]) {
		if (debounce[SHIFT_A_BIT]++ == DEBOUNCE_TRIGGER) {
			changed[nChanged++] = SHIFT_A_BIT;
			debouncing[SHIFT_A_BIT] = false;
		}
	}

	int shiftB = digitalRead(SHIFT_B);
	if (pinStates[SHIFT_B_BIT] != shiftB) {
		pinStates[SHIFT_B_BIT] = shiftB;
		debouncing[SHIFT_B_BIT] = !debouncing[SHIFT_B_BIT];
		debounce[SHIFT_B_BIT] = 0;
	} else if (debouncing[SHIFT_B_BIT]) {
		if (debounce[SHIFT_B_BIT]++ == DEBOUNCE_TRIGGER) {
			changed[nChanged++] = SHIFT_B_BIT;
			debouncing[SHIFT_B_BIT] = false;
		}
	}

	return nChanged;
}


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void loop() {
	int nChanged = readButtons();
	if (nChanged) {
		for (int i = 0; i < nChanged; ++i) {
			if (changed[i] < 24) {
				// pad
				pads[pinToPadMap[changed[i]]].trigger(pinStates[changed[i]]);
				/*int note = 12 + selectedOctave * 24 + pinToPadMap[changed[i]];
				if (pinStates[changed[i]]) {
					MIDI.sendNoteOn(note, 120, 1);
					// display.displayNote(note);
				} else {
					MIDI.sendNoteOff(note, 0, 1);
				}*/
			// TODO: refactor the following
			} else if (changed[i] == SHIFT_A_BIT) {
				// shift A
				if (!pinStates[SHIFT_A_BIT]) {
					if (++selectedOctave > 3) selectedOctave = 0;
					// display.displayString("SA");
					display.displayNumber(selectedOctave, 0, 0);
					setupPads(selectedOctave);
				}
			} else if (changed[i] == SHIFT_B_BIT) {
				// shift B
				if (!pinStates[SHIFT_B_BIT]) {
					if (--selectedOctave < 0) selectedOctave = 3;
					// display.displayString("SB");
					display.displayNumber(selectedOctave, 0, 0);
					setupPads(selectedOctave);
				}
			}
		}
	}

	// foot controler

	/*if (--footTimer == 0) {
		// only one read every 50 cycles for stability
		footControlRead = analogRead(A0);
		footTimer = ANALOG_READ_RATE;
		if (abs(footControlRead - footControlValue) > 60) {
			footControlValue = footControlRead;
			long c = (footControlRead - 10) * 128 / 990;
			if (c < 0) c = 0;
			else if (c > 127) c = 127;
			byte cc = (byte) c;
			MIDI.sendControlChange(24, cc, 1);
		}
	}*/
}



