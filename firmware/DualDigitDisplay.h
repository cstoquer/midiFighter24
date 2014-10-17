#ifndef DUAL_DIGIT_DISPLAY__H
#define DUAL_DIGIT_DISPLAY__H

#define DISPLAY_LATCH 9
#define DISPLAY_CLOCK 8
#define DISPLAY_DATA 10

class DualDigitDisplay {

public:
	DualDigitDisplay();
	void setup();
	void display(int digitA, int digitB);
	void displayNumber(int number, int decimal, byte quote);
	void displayNote(int noteNumber);
	void displayString(char* letters);
	void clear();
};

#endif // DUAL_DIGIT_DISPLAY__H
