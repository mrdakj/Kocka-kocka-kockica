#ifndef _GLOBALVARIABLES_H_
#define _GLOBALVARIABLES_H_

#include "space.h"
#include "timer.h"
#include "keyboard.h"
#include "camera.h"
#include <GL/glut.h>

#define PI 3.141592

/* window size */
extern int window_width;
extern int window_height;
extern int near_clipping_distance;
extern int fovy;

/* out space */
extern Space space; 

/* timers */
extern Timer t_camera, t_car, t_brick;
extern Camera camera;

/* buttons */
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


#endif
