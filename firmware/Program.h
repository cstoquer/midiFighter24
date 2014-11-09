#ifndef PROGRAM__H
#define PROGRAM__H

#include "DualDigitDisplay.h"
#include "Pad.h"
#include "scales.h"


class Program {
private:
	Pad pads[24];
	int currentOctave;
	int nOctaves;
	DualDigitDisplay *display;

public:
	Program();
	void init(DualDigitDisplay*);
	void setupPads(int);
	void shiftUp();
	void shiftDown();
	void shiftUpDown();

	//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
	inline void triggerPad(int pad, int state) {
		pads[pad].trigger(state);
	}
};


#endif // PROGRAM__H
