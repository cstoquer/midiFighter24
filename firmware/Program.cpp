#include <stdlib.h>
#include "Arduino.h"
#include "Program.h"


Program::Program() {
	currentOctave = 1;
	nOctaves = 4;
	octaveSize = 24;
	scale = NULL;
	scaleSize = 0;
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::init(DualDigitDisplay* d) {
	currentOctave = 1;
	setupPads();
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

void Program::setupPads() {
	if (scale != NULL) {
		setupPadFromScale(pads, currentOctave, 12, scale, scaleSize);
		return;
	}
	int note = currentOctave * octaveSize + 12;
	for (int i = 0; i < 24; ++i) {
		pads[i].setNote(note);
		note += 1;
	}
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::setScaleMode(const byte* _scale, int _scaleSize) {
	scale = _scale;
	scaleSize = _scaleSize;
	setupPads();
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::setChromaticMode(int _octaveSize) {
	scale = NULL;
	octaveSize = _octaveSize;
	setupPads();
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::shiftUp() {
	if (++currentOctave >= nOctaves) currentOctave = 0;
	setupPads();
	display->displayNumber(currentOctave, 0, 0);
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::shiftDown() {
	if (--currentOctave < 0) currentOctave = nOctaves - 1;
	setupPads();
	display->displayNumber(currentOctave, 0, 0);
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::allPadOff() {
	// cancel all pad notes
	for (int i = 0; i < 24; ++i) {
		pads[i].trigger(0);
	}
};

