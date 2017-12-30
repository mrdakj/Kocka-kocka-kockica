#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "global_variables.h"

/* returns true if no collision is detected and moves the brick, otherwise returns false */
bool move_brick(Direction d, Brick& c, float brick_move_speed);

/* calculate direction and move brick */
void move_delta(float delta_x, float delta_y, Brick& current_brick);

#endif
