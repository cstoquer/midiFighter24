#ifndef PAD__H
#define PAD__H


class Pad {
private:
	int  note;

public:
	int  state;

	Pad();
	void setNote(int);
	void trigger(bool);
};

#endif // PAD__H
