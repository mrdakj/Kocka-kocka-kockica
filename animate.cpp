/* animate car and brick on keyboard */

#include <GL/glut.h>
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


		if (brick_buttons[translation_up].pressed)
			move_brick(Up, current_brick, space.brick_move_speed);

		if (brick_buttons[translation_down].pressed)
			move_brick(Down, current_brick, space.brick_move_speed);

		float delta_x = brick_buttons[translation_left].pressed ? -space.brick_move_speed :
						brick_buttons[translation_right].pressed ? space.brick_move_speed : 0;

		if (brick_buttons[translation_left].pressed == brick_buttons[translation_right].pressed)
			delta_x = 0;


		float delta_y = brick_buttons[translation_forward].pressed ? space.brick_move_speed :
						brick_buttons[translation_backward].pressed ? -space.brick_move_speed : 0;

		if (brick_buttons[translation_forward].pressed == brick_buttons[translation_backward].pressed)
			delta_y = 0;

		move_delta(delta_x, delta_y, current_brick);

		glutPostRedisplay();
	}

	if (all_released(brick_buttons))
		t_brick.stop();


	t_brick.cont();
}
