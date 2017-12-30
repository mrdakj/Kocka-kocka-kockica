#include "collision.h"
#include <cmath>
#include "vector3f.h"

extern float objX, objY, objZ;
extern Vector3f camera_position, to;

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
		objX += c.pos.x - xstart;
	if (d==Forward || d==Backward)
		objZ += -c.pos.y + ystart;
	if (d==Up || d==Down)
		objY += c.pos.z-zstart;

	to.x = objX-camera_position.x;
	to.z = objZ-camera_position.z;
	to.y = objY-camera_position.y;
	float modultheta = std::sqrt(to.x*to.x+to.z*to.z+to.y*to.y);

	to.x /= modultheta;
	to.z /= modultheta;
	to.y /= modultheta;

	return returnVal;
}
