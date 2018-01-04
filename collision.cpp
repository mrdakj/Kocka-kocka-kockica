#include "headers/collision.h"
#include <cmath>
#include "headers/vector3f.h"
#include <stdio.h>

extern Vector3f selection;

bool move_brick(Direction d, Brick& c,float brick_move_speed) {
	if (std::fabs(brick_move_speed)>1)
		return false;

	bool returnVal = false;
	float xstart = c.pos.x;
	float ystart = c.pos.y;
	float zstart = c.pos.z;

	float& coordinate =
		(d == Left || d == Right) ? c.pos.x : ((d == Forward || d == Backward) ? c.pos.y : c.pos.z);

	int z = (d == Left || d == Forward || d == Down) ? -1 : 1;

	int line = (z == 1) ? std::ceil(coordinate) : std::floor(coordinate);


	float limit = 1.2 * brick_move_speed; // 1.2>1 so we are sure the brick cannot go inside
	if (d == Down)
		limit += 0.2;


	if (std::fabs(line - coordinate) <= limit) {
		if (d != Down)
			coordinate = line;

		if (space.move(space.selected_brick, d)) {
			coordinate += z*brick_move_speed;
			returnVal =true;
		}
		else {
			if (d == Down)
				coordinate = line+0.2;

			returnVal=false;
		}
	}
	else {
		coordinate += z*brick_move_speed;
		returnVal=true;
	}

	if (d==Left || d==Right)
		selection += Vector3f(c.pos.x-xstart, 0, 0);
	if (d==Forward || d==Backward)
		selection += Vector3f(0, 0,  -c.pos.y + ystart);
	if (d==Up || d==Down)
		selection += Vector3f(0, c.pos.z-zstart, 0);

	camera.look_at_point(selection);

	return returnVal;
}

void move_delta(float delta_x, float delta_y, Brick& current_brick) {

	float d = std::sqrt(camera.view.x*camera.view.x+camera.view.z*camera.view.z);


	float ax=0,bx=0,ay=0,by=0;

	/* project normal vector of (projection of TO vector in XZ plane) of length delta_x camera.view x and z axes */
	if (delta_x) {
		bx=-camera.view.z*(delta_x/d);
		ax=camera.view.x*(delta_x/d);
	}


	/* project (projection of TO) vector of length delta_y camera.view x and z axes */
	if (delta_y) {
		by=camera.view.x*(delta_y/d);
		ay=camera.view.z*(delta_y/d);
	}

	float a = ax+ay;
	float b = bx+by;

	/* move only along tangent of circle */
	float distance =Vector3f(selection.x-camera.position.x, 0, selection.z-camera.position.z).norm_squared();
	if (distance<1*1 && delta_y<0) {
		a=ax;
		b=bx;
	}

	/* if this is > 1 collision will not work properly because we cannot skip a field */
	if (a>0.9)
		a = 0.9;
	if (a<-0.9)
		a = -0.9;
	if (b>0.9)
		b = 0.9;
	if (b<-0.9)
		b = -0.9;


	if (b>0)
		move_brick(Right,current_brick,b);
	if (b<0)
		move_brick(Left,current_brick,-b);
	if (a>0)
		move_brick(Forward,current_brick,a);
	if (a<0)
		move_brick(Backward,current_brick,-a);

}
