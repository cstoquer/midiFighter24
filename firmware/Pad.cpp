#include <stdlib.h>
#include "Arduino.h"
#include "Pad.h"
#include "MIDI.h"


Pad::Pad() {
	note  = 0;
	state = -1;
}

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Pad::setNote(int n) {
	note = n;
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄

void Pad::trigger(bool value) {
	if (value) {
		MIDI.sendNoteOn(note, 120, mainMidiChannel);
		state = note;
	} else if (state >= 0) {
		MIDI.sendNoteOff(state, 0, mainMidiChannel);
		state = -1;
	}
};
