/* init car, brick and camera timer */

#include <GL/glut.h>
#include "headers/collision.h"
#include "headers/keyboard.h"

void car_on_timer(int value);
void brick_keyboard_on_timer(int value);
void camera_on_timer(int value);

Timer t_car(car_on_timer);
Timer t_brick(brick_keyboard_on_timer);
Timer t_camera(camera_on_timer);

void car_on_timer(int value) {
	if (!t_car.check(value)) return;

	room.car.go();

	glutPostRedisplay();

	if (!t_car.cont())
		room.car.stop();
}

void brick_keyboard_on_timer(int value) {
	if (!t_brick.check(value)) return;

	if (room.selected_brick != NONE) {
		Brick& current_brick = room.bricks[room.selected_brick];


		if (brick_buttons[translation_up].pressed)
			move_brick(Up, current_brick, room.brick_move_speed);

		if (brick_buttons[translation_down].pressed)
			move_brick(Down, current_brick, room.brick_move_speed);

		float delta_x = brick_buttons[translation_left].pressed ? -room.brick_move_speed :
						brick_buttons[translation_right].pressed ? room.brick_move_speed : 0;

		if (brick_buttons[translation_left].pressed == brick_buttons[translation_right].pressed)
			delta_x = 0;


		float delta_y = brick_buttons[translation_forward].pressed ? room.brick_move_speed :
						brick_buttons[translation_backward].pressed ? -room.brick_move_speed : 0;

		if (brick_buttons[translation_forward].pressed == brick_buttons[translation_backward].pressed)
			delta_y = 0;

		move_delta(delta_x, delta_y, current_brick);

		glutPostRedisplay();
	}

	if (all_released(brick_buttons))
		t_brick.stop();


	t_brick.cont();
}

void camera_on_timer(int value) {
	if (!t_camera.check(value)) return;

	if (room.selected_brick == NONE) {

		/* get theta_delta and phi_delta from keyboard buttons */
		float theta_delta = camera_buttons[rotation_left].pressed ? -0.02 :
					 camera_buttons[rotation_right].pressed ? 0.02 : 0;

		if (camera_buttons[rotation_left].pressed == camera_buttons[rotation_right].pressed)
			theta_delta = 0;

		float phi_delta = camera_buttons[rotation_up].pressed ? 0.02 :
				   camera_buttons[rotation_down].pressed ? -0.02 : 0;

		if (camera_buttons[rotation_up].pressed == camera_buttons[rotation_down].pressed)
			phi_delta = 0;

		/* get up_delta, left_delta and forward_delta from keyboard buttons */
		float up_delta = camera_buttons[translation_up].pressed ? 0.05 :
				  camera_buttons[translation_down].pressed ? -0.05 : 0;

		if (camera_buttons[translation_up].pressed == camera_buttons[translation_down].pressed)
			up_delta = 0;

		float left_delta = camera_buttons[translation_left].pressed ? 0.5 :
					camera_buttons[translation_right].pressed ? -0.5 : 0;

		if (camera_buttons[translation_left].pressed == camera_buttons[translation_right].pressed)
			left_delta = 0;

		float forward_delta = camera_buttons[translation_forward].pressed ? 0.5 :
					   camera_buttons[translation_backward].pressed ? -0.5 : 0;

		if (camera_buttons[translation_forward].pressed == camera_buttons[translation_backward].pressed)
			forward_delta = 0;

		camera.move_on_keyboard(theta_delta, phi_delta, forward_delta, left_delta, up_delta);

		glutPostRedisplay();
	}


	if (all_released(camera_buttons))
		t_camera.stop();

	t_camera.cont();
}

