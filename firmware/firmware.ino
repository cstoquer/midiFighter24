#include "MIDI.h"
#include "DualDigitDisplay.h"
#include "Program.h"
#include "scales.h"
#include "global.h"

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

// long  footControlRead;    // analog read value of connected foot controller
// long  footControlValue;   // analog current value of connected foot controller
// int   footTimer;

DualDigitDisplay display;
Program          program;

const int pinToPadMap[26] = {
	2, 14, 23, 3, 15, 19, 6, 10, 22, 7, 11, 18,
	0, 13, 17, 1, 12, 21, 4,  9, 16, 5,  8, 20,
	24, 25 // shift buttons
};


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

	display.setup();
	program.init(&display);
	MIDI.begin();
	mainMidiChannel = 4;

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
		int values[3];

		values[0] = digitalRead(READ1);
		values[1] = digitalRead(READ2);
		values[2] = digitalRead(READ3);

		digitalWrite(CLOCK, HIGH);
		// delayMicroseconds(PULSE_WIDTH_USEC);

		for (int i = 0; i < 3; ++i) {
			int value = values[i];
			//-------------------------------------
			// update and debounce value 1
			if (pinStates[pin] != value) {
				// pin changed value
				pinStates[pin] = value;
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
		}

		//-------------------------------------
		// update and debounce value 2
		/*if (pinStates[++pin] != value2) {
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
		}*/

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

	if (nChanged == 0) return;

	for (int i = 0; i < nChanged; ++i) {
		if (changed[i] < 24) {
			program.triggerPad(pinToPadMap[changed[i]], pinStates[changed[i]]);
		} else if (changed[i] == SHIFT_A_BIT) {
			// shift A + shift B
			if (pinStates[SHIFT_A_BIT] && pinStates[SHIFT_B_BIT]) {
				mainMenu();
				return;
			}
			// shift A, down edge
			if (!pinStates[SHIFT_A_BIT]) program.shiftUp();
		} else if (changed[i] == SHIFT_B_BIT) {
			// shift B + shift A
			if (pinStates[SHIFT_A_BIT] && pinStates[SHIFT_B_BIT]) {
				mainMenu();
				return;
			}
			// shift B, down edge
			if (!pinStates[SHIFT_B_BIT]) program.shiftDown();
		}
	}
}

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void mainMenu() {
	display.displayString("MN");
	program.allPadOff();
	while (true) {
		int nChanged = readButtons();
		if (nChanged == 0) continue;
		for (int i = 0; i < nChanged; ++i) {
			// int state = pinStates[changed[i]];
			if (pinStates[changed[i]]) continue; // down edge only
			int button = pinToPadMap[changed[i]];
			switch (button) {
				case 16: channelMenu(); break;
				case 20: scaleMenu(); break;
				case 21: rootNoteMenu(); break;
				case 22: rootPadMenu(); break;
				case 23: display.displayString("GO"); return; // exit menu
			}
		}
	}
}

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void scaleMenu() {
	display.displayString("SC");
	while (true) {
		int nChanged = readButtons();
		if (nChanged == 0) continue;
		for (int i = 0; i < nChanged; ++i) {
			// int state = pinStates[changed[i]];
			if (pinStates[changed[i]]) continue; // down edge only
			int button = pinToPadMap[changed[i]];
			switch (button) {
				case 0: display.displayString("MA"); program.setScaleMode(scale_major, 6); return;
				case 1: display.displayString("HI"); program.setScaleMode(scale_harmonicMinor, 6); return;
				case 2: display.displayString("MM"); program.setScaleMode(scale_melodicMinor, 6); return;
				case 3: display.displayString("WT"); program.setScaleMode(scale_wholeTone, 5); return;
				case 4: display.displayString("DI"); program.setScaleMode(scale_diminished, 7); return;
				case 5: display.displayString("PA"); program.setScaleMode(scale_majorPentatonic, 4); return;
				case 6: display.displayString("PI"); program.setScaleMode(scale_minorPentatonic, 4); return;
				case 7: display.displayString("JP"); program.setScaleMode(scale_japInSen, 4); return;
				case 8: display.displayString("BM"); program.setScaleMode(scale_majorBebop, 7); return;
				case 9: display.displayString("BD"); program.setScaleMode(scale_dominantBebop, 7); return;
				case 10: display.displayString("BL"); program.setScaleMode(scale_blues, 5); return;
				case 11: display.displayString("AR"); program.setScaleMode(scale_arabic, 6); return;
				case 12: display.displayString("EN"); program.setScaleMode(scale_enigmatic, 6); return;
				case 13: display.displayString("NE"); program.setScaleMode(scale_neapolitan, 6); return;
				case 14: display.displayString("NI"); program.setScaleMode(scale_neapolitanMinor, 6); return;
				case 15: display.displayString("HU"); program.setScaleMode(scale_hungarianMinor, 6); return;
				case 16: display.displayString("DO"); program.setScaleMode(scale_dorian, 6); return;
				case 17: display.displayString("PH"); program.setScaleMode(scale_phrygian, 6); return;
				case 18: display.displayString("LY"); program.setScaleMode(scale_lydian, 6); return;
				case 19: display.displayString("MI"); program.setScaleMode(scale_mixolidian, 6); return;
				case 20: display.displayString("AE"); program.setScaleMode(scale_aeolidian, 6); return;
				case 21: display.displayString("LO"); program.setScaleMode(scale_locrian, 6); return;
				case 22: display.displayString("CR"); program.setChromaticMode(12); return;
				case 23: display.displayString("MP"); program.setChromaticMode(24); return;
				case 24: /*shift A*/
				case 25: /*shift B*/ return;
			}
		}
	}
}

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void rootNoteMenu() {
	display.displayString("RN");
	while (true) {
		int nChanged = readButtons();
		if (nChanged == 0) continue;
		for (int i = 0; i < nChanged; ++i) {
			// int state = pinStates[changed[i]];
			if (pinStates[changed[i]]) continue; // down edge only
			int button = pinToPadMap[changed[i]];
			if (button < 12) {
				display.displayNote(button);
				program.setRootNote(button);
				return;
			}
		}
	}
}

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void rootPadMenu() {
	display.displayString("RP");
	while (true) {
		int nChanged = readButtons();
		if (nChanged == 0) continue;
		for (int i = 0; i < nChanged; ++i) {
			// int state = pinStates[changed[i]];
			if (pinStates[changed[i]]) continue; // down edge only
			int button = pinToPadMap[changed[i]];
			if (button < 24) {
				display.displayNumber(button);
				program.setRootPad(button);
				return;
			}
		}
	}
}

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void channelMenu() {
	display.displayString("CH");
	while (true) {
		int nChanged = readButtons();
		if (nChanged == 0) continue;
		for (int i = 0; i < nChanged; ++i) {
			// int state = pinStates[changed[i]];
			if (pinStates[changed[i]]) continue; // down edge only
			int button = pinToPadMap[changed[i]];
			if (button < 16) {
				mainMidiChannel = button + 1;
				display.displayNumber(button + 1);
				return;
			}
		}
	}
}