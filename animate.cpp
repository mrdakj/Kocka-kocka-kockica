/* animate car and brick */
/* car move and brick up/down move on keyboard */

#include <GL/glut.h>
#include "global_variables.h"
#include "collision.h"

void car_on_timer(int value);
void brick_keyboard_on_timer(int value);

Timer t_car(car_on_timer);
Timer t_brick(brick_keyboard_on_timer);

void car_on_timer(int value) {
	if (!t_car.check(value)) return;

	space.car.go();

	glutPostRedisplay();

	if (!t_car.cont())
		space.car.stop();
}

void brick_keyboard_on_timer(int value) {
	if (!t_brick.check(value)) return;

	if (space.selected_brick != NONE) {
		Brick& current_brick = space.bricks[space.selected_brick];

		if (bt_brick_up.pressed)
			move_brick(Up, current_brick, space.brick_move_speed);

		if (bt_brick_down.pressed)
			move_brick(Down, current_brick, space.brick_move_speed);
	}

	if (!bt_brick_up.pressed && !bt_brick_down.pressed)
		t_brick.stop();

	glutPostRedisplay();

	t_brick.cont();
}
