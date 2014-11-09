#include <stdlib.h>
#include "Arduino.h"
#include "Program.h"


Program::Program() {
	currentOctave = 1;
	nOctaves = 4;
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::init(DualDigitDisplay* d) {
	currentOctave = 1;
	setupPads(currentOctave);
	display = d;
};


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::setupPads(int octave) {
	for (int i = 0; i < 24; ++i) {
		int note = 12 + octave * 24 + i;
		pads[i].setNote(note);
	}
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::shiftUp() {
	if (++currentOctave >= nOctaves) currentOctave = 0;
	setupPads(currentOctave);
	display->displayNumber(currentOctave, 0, 0);
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::shiftDown() {
	if (--currentOctave < 0) currentOctave = nOctaves - 1;
	setupPads(currentOctave);
	display->displayNumber(currentOctave, 0, 0);
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::shiftUpDown() {
	// TODO
};
