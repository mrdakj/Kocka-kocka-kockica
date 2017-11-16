#include "animate.h"
#include <unistd.h>


bool move(Direction d, Cuboid& c,float speed) {
	float& coordinate =
		(d == Left || d == Right) ? c.pos.x : ((d == Forward || d == Backward) ? c.pos.y : c.pos.z);

	int z = (d == Left || d == Forward || d == Down) ? -1 : 1;

	int line = (z == 1) ? ceil(coordinate) : floor(coordinate);


	float limit = 1.2 * speed; // 1.2>1 so we are sure the cuboid cannot go inside
	if (d == Down)
		limit += 0.2;

	if (fabs(line - coordinate) <= limit) {
		if (d != Down)
			coordinate = line;

		if (space.move(selected, d)) 
		{
			coordinate += z*speed;
			return true;
		}
		else
		{
			if (d == Down)
				coordinate = line+0.2;
			return false;
		}
	}
	else
	{
		coordinate += z*speed;
		return true;
	}
}

void on_timer(int value) {
	if (value != TIMER_ID)
	  return;

	if (selected==-1 || !somethingIsPressed())
		return;

	Cuboid& selectedCuboid = space.cuboids[selected];

	if (bLeft.pressed)
		move(Left,selectedCuboid,speed);

	if (bRight.pressed)
		move(Right,selectedCuboid,speed);

	if (bForward.pressed)
		move(Forward,selectedCuboid,speed);

	if (bBackward.pressed)
		move(Backward,selectedCuboid,speed);

	if (bUp.pressed)
		move(Up,selectedCuboid,speed);

	if (bDown.pressed)
		move(Down,selectedCuboid,speed);


	glutPostRedisplay();

	if (animation_ongoing)
	  glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);

}
