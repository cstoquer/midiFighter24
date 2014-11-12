#ifndef PROGRAM__H
#define PROGRAM__H

#include "DualDigitDisplay.h"
#include "Pad.h"


class Program {
private:
	Pad pads[24];
	int octaveSize;
	int currentOctave;
	int nOctaves;
	byte const *scale;
	int scaleSize;
	DualDigitDisplay *display;

public:
	Program();
	void init(DualDigitDisplay*);
	void setupPads();
	void shiftUp();
	void shiftDown();
	void allPadOff();

	void setScaleMode(const byte*, int);
	void setChromaticMode(int);

	//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
	inline void triggerPad(int pad, int state) {
		pads[pad].trigger(state);
	}
};


#endif // PROGRAM__H
