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
	Translation_up,
	Translation_down,
	Translation_left,
	Translation_right,
	Translation_forward,
	Translation_backward,
	Rotation_left, 
	Rotation_right,
	Rotation_up,
	Rotation_down
};

void keyboard_ascii_down(unsigned char key, int x, int y);
void keyboard_ascii_up(unsigned char key, int x,int y);

/* returns true if all buttons are released */
bool all_released(const std::vector<Button>& buttons);


#endif
