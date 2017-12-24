#include "animate.h"
#include <unistd.h>
#include "globalVariables.h" 
#include "camera.h"


bool move(Direction d, Brick& c,float speed) {
	if (abs(speed)>1)
		return false;

	bool returnVal = false;
	float xstart = c.pos.x;
	float ystart = c.pos.y;
	float zstart = c.pos.z;

	float& coordinate =
		(d == Left || d == Right) ? c.pos.x : ((d == Forward || d == Backward) ? c.pos.y : c.pos.z);

	int z = (d == Left || d == Forward || d == Down) ? -1 : 1;

	int line = (z == 1) ? ceil(coordinate) : floor(coordinate);


	float limit = 1.2 * speed; // 1.2>1 so we are sure the brick cannot go inside
	if (d == Down)
		limit += 0.2;

	if (fabs(line - coordinate) <= limit) {
		if (d != Down)
			coordinate = line;

		if (space.move(space.selected_brick, d)) 
		{
			coordinate += z*speed;
			returnVal =true;
		}
		else
		{
			if (d == Down)
				coordinate = line+0.2;
			returnVal=false;
		}
	}
	else
	{
		coordinate += z*speed;
		returnVal=true;
	}

	if (d==Left || d==Right)
		objX += c.pos.x - xstart;
	if (d==Forward || d==Backward)
		objZ += -c.pos.y + ystart;
	if (d==Up || d==Down)
		objY += c.pos.z-zstart;

	return returnVal;
}


void on_timer(int value) {
	if (value != TIMER_ID)
	  return;

	if (space.selected_brick==-1 || !somethingIsPressed())
		return;

	Brick& selected_brickBrick = space.bricks[space.selected_brick];

	if (bLeft.pressed) {
		if (to.x-to.z>=0 && -to.x-to.z>=0)
			move(Left,selected_brickBrick,speed);
		if (-to.x-to.z>=0 && -to.x+to.z>=0)
			move(Forward,selected_brickBrick,speed);
		if (-to.x+to.z>=0 && to.x+to.z>=0)
			move(Right,selected_brickBrick,speed);
		if (to.x+to.z>=0 && to.x-to.z>=0)
			move(Backward,selected_brickBrick,speed);
	}

	if (bRight.pressed) {
		if (to.x-to.z>=0 && -to.x-to.z>=0)
			move(Right,selected_brickBrick,speed);
		if (-to.x-to.z>=0 && -to.x+to.z>=0)
			move(Backward,selected_brickBrick,speed);
		if (-to.x+to.z>=0 && to.x+to.z>=0)
			move(Left,selected_brickBrick,speed);
		if (to.x+to.z>=0 && to.x-to.z>=0)
			move(Forward,selected_brickBrick,speed);
	}

	if (bForward.pressed) {
		if (to.x-to.z>=0 && -to.x-to.z>=0)
			move(Forward,selected_brickBrick,speed);
		if (-to.x-to.z>=0 && -to.x+to.z>=0)
			move(Right,selected_brickBrick,speed);
		if (-to.x+to.z>=0 && to.x+to.z>=0)
			move(Backward,selected_brickBrick,speed);
		if (to.x+to.z>=0 && to.x-to.z>=0)
			move(Left,selected_brickBrick,speed);
	}

	if (bBackward.pressed) {
		if (to.x-to.z>=0 && -to.x-to.z>=0)
			move(Backward,selected_brickBrick,speed);
		if (-to.x-to.z>=0 && -to.x+to.z>=0)
			move(Left,selected_brickBrick,speed);
		if (-to.x+to.z>=0 && to.x+to.z>=0)
			move(Forward,selected_brickBrick,speed);
		if (to.x+to.z>=0 && to.x-to.z>=0)
			move(Right,selected_brickBrick,speed);
	}

	if (bUp.pressed)
		move(Up,selected_brickBrick,speed);

	if (bDown.pressed)
		move(Down,selected_brickBrick,speed);


	glutPostRedisplay();

	if (animation_ongoing)
	  glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);

}
