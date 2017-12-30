#ifndef _GLOBALVARIABLES_H_
#define _GLOBALVARIABLES_H_

#include "space.h"
#include "timer.h"
#include "keyboard.h"

#define PI 3.141592

/* window size */
extern int window_width;
extern int window_height;

/* out space */
extern Space space;

/* timers */
extern Timer t_camera, t_car, t_brick;

/* buttons */
extern Button bt_brick_up, bt_brick_down;

extern std::vector<Button> camera_buttons;

enum {
	rotation_left, 
	rotation_right,
	rotation_up,
	rotation_down,
	translate_up,
	translate_down,
	translate_left,
	translate_right,
	translate_forward,
	translate_backward
};


#endif
