#include <stdlib.h>
#include "Arduino.h"
#include "Pad.h"
#include "MIDI.h"


Pad::Pad() {
	note  = 0;
	state = 0;
}

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Pad::setNote(int n) {
	note = n;
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Pad::trigger(bool value) {
	if (value) {
		MIDI.sendNoteOn(note, 120, 1);
		state = note;
	} else {
		MIDI.sendNoteOff(state, 0, 1);
	}
};
