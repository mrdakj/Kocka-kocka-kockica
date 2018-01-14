#ifndef _GLOBALVARIABLES_H_
#define _GLOBALVARIABLES_H_

#include "room.h"
#include "timer.h"
#include "camera.h"
#include "model.h"

#define PI 3.141592

/* window size */
extern int window_width;
extern int window_height;
extern int near_clipping_distance;
extern int fovy;

extern Room room; 

/* timers */
extern Timer t_camera, t_car, t_brick;

extern Camera camera;

#endif
