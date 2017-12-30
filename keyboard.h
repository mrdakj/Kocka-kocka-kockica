#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

struct Button {
	const int key;
	bool pressed;

	Button(int key) : key(key), pressed(false) {}
	void change_state() { pressed = !pressed; }
};

void keyboard_ascii_down(unsigned char key, int x, int y);
void keyboard_ascii_up(unsigned char key, int x,int y);
void keyboard_special_down(int key, int x, int y);
void keyboard_special_up(int key, int x, int y );

#endif
