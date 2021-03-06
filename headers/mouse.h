#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "global_variables.h"
#include "vector3f.h"


void on_mouse_click(int button, int state, int x, int y);
void on_mouse_active_move(int x, int y);
void on_mouse_passive_move(int x, int y);
void set_cursor_in_center();

#endif
