#include <stdlib.h>
#include "Arduino.h"
#include "Program.h"


Program::Program() {
	currentOctave = 1;
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::init() {
	currentOctave = 1;
	setupPads(currentOctave);
};


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Program::setupPads(int octave) {
	for (int i = 0; i < 24; ++i) {
		int note = 12 + octave * 24 + i;
		pads[i].setNote(note);
	}
};
