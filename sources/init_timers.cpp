/* init car, brick and camera timer */
#include <GL/glut.h>
#include "../headers/global_variables.h"
#include "../headers/keyboard.h"
#include "../headers/room.h"
#include "../headers/camera.h"

void car_on_timer(int value);
void brick_keyboard_on_timer(int value);
void camera_on_timer(int value);

Timer t_car(car_on_timer);
Timer t_brick(brick_keyboard_on_timer);
Timer t_camera(camera_on_timer);

extern ut::Vector3f selection;

void car_on_timer(int value)
{
	if (!t_car.check(value))
		return;

	room.car.go();

	glutPostRedisplay();

	if (!t_car.cont())
		room.car.stop();
}

void brick_keyboard_on_timer(int value)
{
	if (!t_brick.check(value))
		return;

	if (!room.nothing_selected()) {

		if (brick_buttons[Translation_up].pressed)
			selection += room.move_selected_brick(Up_direction, BRICK_SPEED);

		if (brick_buttons[Translation_down].pressed)
			selection += room.move_selected_brick(Down_direction, BRICK_SPEED);

		float delta_x = brick_buttons[Translation_left].pressed ? -BRICK_SPEED :
						brick_buttons[Translation_right].pressed ? BRICK_SPEED : 0;

		if (brick_buttons[Translation_left].pressed == brick_buttons[Translation_right].pressed)
			delta_x = 0;


		float delta_y = brick_buttons[Translation_forward].pressed ? BRICK_SPEED :
						brick_buttons[Translation_backward].pressed ? -BRICK_SPEED : 0;

		if (brick_buttons[Translation_forward].pressed == brick_buttons[Translation_backward].pressed)
			delta_y = 0;


		float distance =ut::Vector3f(selection.x-camera.position.x, 0, selection.z-camera.position.z).norm_squared();

		if (distance<1*1 && delta_y<0)
			delta_y = 0;

		selection += room.move_selected_brick(camera.view, delta_x, delta_y);

		camera.look_at_point(selection);

		glutPostRedisplay();
	}

	if (all_released(brick_buttons))
		t_brick.stop();


	t_brick.cont();
}

void camera_on_timer(int value)
{
	if (!t_camera.check(value)) return;

	if (room.nothing_selected()) {

		/* get theta_delta and phi_delta from keyboard buttons */
		float theta_delta =
			camera_buttons[Rotation_left].pressed ? -0.02 :
			camera_buttons[Rotation_right].pressed ? 0.02 : 0;

		if (camera_buttons[Rotation_left].pressed == camera_buttons[Rotation_right].pressed)
			theta_delta = 0;

		float phi_delta =
			camera_buttons[Rotation_up].pressed ? 0.02 :
			camera_buttons[Rotation_down].pressed ? -0.02 : 0;

		if (camera_buttons[Rotation_up].pressed == camera_buttons[Rotation_down].pressed)
			phi_delta = 0;

		/* get up_delta, left_delta and forward_delta from keyboard buttons */
		float up_delta =
			camera_buttons[Translation_up].pressed ? 0.05 :
			camera_buttons[Translation_down].pressed ? -0.05 : 0;

		if (camera_buttons[Translation_up].pressed == camera_buttons[Translation_down].pressed)
			up_delta = 0;

		float left_delta = camera_buttons[Translation_left].pressed ? 0.5 :
					camera_buttons[Translation_right].pressed ? -0.5 : 0;

		if (camera_buttons[Translation_left].pressed == camera_buttons[Translation_right].pressed)
			left_delta = 0;

		float forward_delta =
			camera_buttons[Translation_forward].pressed ? 0.5 :
			camera_buttons[Translation_backward].pressed ? -0.5 : 0;

		if (camera_buttons[Translation_forward].pressed == camera_buttons[Translation_backward].pressed)
			forward_delta = 0;

		camera.move_on_keyboard(theta_delta, phi_delta, forward_delta, left_delta, up_delta);

		glutPostRedisplay();
	}


	if (all_released(camera_buttons))
		t_camera.stop();

	t_camera.cont();
}
