#ifndef PAD__H
#define PAD__H


class Pad {
private:
	int  note;
	int  state;

public:
	Pad();
	void setNote(int);
	void trigger(bool);
};

#endif // PAD__H
