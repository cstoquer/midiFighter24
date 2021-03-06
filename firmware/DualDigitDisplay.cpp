#include <stdlib.h>
#include "Arduino.h"
#include "DualDigitDisplay.h"

//                        0    1   2   3    4   5    6    7   8    9
const byte NUMBERS[10] = {119, 36, 93, 109, 46, 107, 123, 37, 127, 111};
//                        A   B    C   D    E   F   G    H   I   J    K   L   M   N   O    P   Q   R   S    T   U    V    W   X   Y    Z
const byte LETTERS[26] = {63, 122, 83, 124, 91, 27, 115, 62, 18, 116, 59, 82, 55, 56, 120, 31, 47, 24, 107, 90, 118, 112, 73, 54, 110, 85};
//                        C   C#  D    D#   E   F   F#  G    G#   A   A#  B
const byte NOTES[12]   = {83, 83, 124, 124, 91, 27, 27, 115, 115, 63, 63, 122};
const byte SHARP[12]   = {0, DOT, 0, DOT, 0, 0, DOT, 0, DOT, 0, DOT, 0};


DualDigitDisplay::DualDigitDisplay() {
	timer = BLINK_DURATION;
	blink = false;
}


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void DualDigitDisplay::setup() {
	pinMode(DISPLAY_LATCH, OUTPUT);
	pinMode(DISPLAY_CLOCK, OUTPUT);
	pinMode(DISPLAY_DATA,  OUTPUT);
}


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void DualDigitDisplay::display(int digitA, int digitB) {
	// take the DISPLAY_LATCH low so 
	// the LEDs don't change while you're sending in bits:
	digitalWrite(DISPLAY_LATCH, LOW);
	// shift out the bits:
	shiftOut(DISPLAY_DATA, DISPLAY_CLOCK, MSBFIRST, digitB);   
	shiftOut(DISPLAY_DATA, DISPLAY_CLOCK, MSBFIRST, digitA);
	//take the latch pin high so the LEDs will light up:
	digitalWrite(DISPLAY_LATCH, HIGH);
	timer = CLEAR_DURATION;
}


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void DualDigitDisplay::displayNumber(int number, int decimal, int quote) {
	display(
		NUMBERS[(number % 100) / 10] | decimal << 7,
		NUMBERS[number % 10]         | quote   << 7
	);
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void DualDigitDisplay::displayNumber(int number) {
	int firstDigit = (number % 100) / 10;
	if (firstDigit != 0) firstDigit = NUMBERS[firstDigit];
	display(
		firstDigit,
		NUMBERS[number % 10]
	);
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void DualDigitDisplay::displayNote(int noteNumber) {
	int note = noteNumber % 12;
	display(NOTES[note], NUMBERS[noteNumber / 12] | SHARP[note]);
};


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void DualDigitDisplay::displayString(char* letters) {
	display(LETTERS[letters[0] - 65], LETTERS[letters[1] - 65]);
};


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void DualDigitDisplay::clear() {
	display(0, 0);
};
