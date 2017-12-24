#ifndef _GLOBALVARIABLES_H_
#define _GLOBALVARIABLES_H_

#include "space.h"
#include "keyboard.h"
#include "vector3f.h"
#include <GL/glut.h>

#define TIMER_INTERVAL 10  

extern int window_width;
extern int window_height;

extern Space space;
extern float speed;

extern float yplane;
extern Vector3f camera_position;
extern Vector3f to;
extern Vector3f view, hvector, v;
// coordinates of selected_brick object
extern GLdouble objX;
extern GLdouble objY;
extern GLdouble objZ;

#endif
