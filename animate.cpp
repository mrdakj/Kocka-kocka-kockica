#include <GL/glut.h>
#include <stdio.h>
#include <unistd.h>
#include "global_variables.h" 
#include "camera.h"
#include "collision.h"
#include "keyboard.h"

void car_on_timer(int value);
void brick_keyboard_on_timer(int value);

Timer t_car(car_on_timer);
Timer t_brick(brick_keyboard_on_timer);

extern Button bt_brick_up, bt_brick_down;

void car_on_timer(int value) {
	if (!t_car.check(value)) return;

	space.car.go();

	glutPostRedisplay();

	if (!t_car.cont())
		space.car.stop();
}

void brick_keyboard_on_timer(int value) {
	if (!t_brick.check(value)) return;

	if (space.selected_brick != -1) {
		Brick& current_brick = space.bricks[space.selected_brick];

		if (bt_brick_up.pressed)
			move_brick(Up,current_brick, space.brick_move_speed);

		if (bt_brick_down.pressed)
			move_brick(Down,current_brick, space.brick_move_speed);
	}

	if (!bt_brick_up.pressed && !bt_brick_down.pressed)
		t_brick.stop();

	glutPostRedisplay();

	t_brick.cont();
}
