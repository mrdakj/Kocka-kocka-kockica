#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "globalVariables.h"
#include "vector3f.h"


void mouse(int button, int state, int x, int y);
void on_mouse_active_move(int x, int y);
void on_mouse_passive_move(int x, int y);

#endif
