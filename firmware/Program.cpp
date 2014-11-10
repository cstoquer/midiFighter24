#include <stdlib.h>
#include "Arduino.h"
#include "Program.h"
#include "scales.h"


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

void setupPadFromScale(Pad* pads, int octave, int key, const byte* scale, int scaleSize) {
	int root = octave * 12 + key;
	int note = root;
	byte index = 0;
	for (int i = 0; i < 24; ++i) {
		pads[i].setNote(note);
		if (index < scaleSize) note += scale[index];
		if (++index > scaleSize) {
			root += 12;
			note = root;
			index = 0;
		}
	}
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::setupPads(int octave) {
	/*int note = octave * 24 + 12;
	for (int i = 0; i < 24; ++i) {
		pads[i].setNote(note);
		note += 1;
	}*/
	setupPadFromScale(pads, octave, 12, scale_major, 6);
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
	// both shiftUp and shiftDown pressed together
	// TODO
};
