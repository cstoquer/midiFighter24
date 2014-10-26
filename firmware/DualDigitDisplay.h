#ifndef DUAL_DIGIT_DISPLAY__H
#define DUAL_DIGIT_DISPLAY__H

#define DISPLAY_LATCH 9
#define DISPLAY_CLOCK 8
#define DISPLAY_DATA 10

#define BLINK_DURATION 4000
#define CLEAR_DURATION 10000

#define DOT 128


class DualDigitDisplay {
private:
	int  timer;
	bool blink;

public:
	DualDigitDisplay();
	void setup();
	void display(int digitA, int digitB);
	void displayNumber(int number, int decimal, int quote);
	void displayNote(int noteNumber);
	void displayString(char* letters);
	void clear();
	inline void click() {
		if (--timer == 0) {
			blink = !blink;
			display(blink ? DOT : 0, 0);
			timer = BLINK_DURATION;
		}
	}
};

#endif // DUAL_DIGIT_DISPLAY__H
