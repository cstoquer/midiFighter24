#ifndef PROGRAM__H
#define PROGRAM__H

#include "Pad.h"
#include "scales.h"


class Program {
private:
	Pad pads[24];
	int currentOctave;

public:
	Program();
	void init();
	void setupPads(int octave);

	//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
	inline void triggerPad(int pad, int state) {
		pads[pad].trigger(state);
	}
};


#endif // PROGRAM__H
