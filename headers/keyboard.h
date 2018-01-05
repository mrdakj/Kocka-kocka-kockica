#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <vector>
#include "timer.h"

struct Button {
	const int key;
	bool pressed;

	Button(int key) : key(key), pressed(false) {}
	void change_state() { pressed = !pressed; }
};

extern std::vector<Button> camera_buttons;
extern std::vector<Button> brick_buttons;

enum {
	translation_up,
	translation_down,
	translation_left,
	translation_right,
	translation_forward,
	translation_backward,
	rotation_left, 
	rotation_right,
	rotation_up,
	rotation_down
};

void keyboard_ascii_down(unsigned char key, int x, int y);
void keyboard_ascii_up(unsigned char key, int x,int y);

/* returns true if all buttons are released */
bool all_released(const std::vector<Button>& buttons);


#endif
