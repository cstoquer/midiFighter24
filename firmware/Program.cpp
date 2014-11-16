#include <stdlib.h>
#include "Arduino.h"
#include "Program.h"


Program::Program() {
	currentOctave = 1;
	maxOctave  = 5;
	octaveSize = 24;
	scale      = NULL;
	scaleSize  = 0;
	rootPad    = 0;
	rootNote   = 0;
	initIndex  = 0;
	initNote   = 0;
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::init(DualDigitDisplay* d) {
	currentOctave = 1;
	setupPads();
	display = d;
};


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::setupPadFromScale() {
	int root = currentOctave * 12 + initNote;
	int note = root;
	byte index = initIndex;
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
		setupPadFromScale();
		return;
	}
	int note = currentOctave * octaveSize;
	for (int i = 0; i < 24; ++i) {
		pads[i].setNote(note);
		note += 1;
	}
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::prepare() {
	int scaleValues[12];
	scaleValues[0] = 0;
	for (int i = 0; i < scaleSize; ++i) {
		scaleValues[i + 1] = scaleValues[i] + scale[i];
	}

	int s = scaleSize + 1;

	initIndex = s - (rootPad % s);
	initNote  = (scaleValues[initIndex] + rootNote) % 12;
	maxOctave = (127 - initNote - (24 % s)) / 12 - (24 / s);

	if (currentOctave > maxOctave) currentOctave = maxOctave;

	setupPads();
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::setRootPad(int r) {
	rootPad = r;
	prepare();
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::setRootNote(int r) {
	rootNote = r;
	prepare();
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::setScaleMode(const byte* _scale, int _scaleSize) {
	scale = _scale;
	scaleSize = _scaleSize;
	prepare();
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::setChromaticMode(int _octaveSize) {
	scale = NULL;
	octaveSize = _octaveSize;
	maxOctave = 5; // TODO
	setupPads();
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::shiftUp() {
	if (++currentOctave >= maxOctave) currentOctave = 0;
	setupPads();
	display->displayNumber(currentOctave);
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::shiftDown() {
	if (--currentOctave < 0) currentOctave = maxOctave - 1;
	setupPads();
	display->displayNumber(currentOctave);
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::allPadOff() {
	// cancel all pad notes
	for (int i = 0; i < 24; ++i) {
		pads[i].trigger(0);
	}
};

