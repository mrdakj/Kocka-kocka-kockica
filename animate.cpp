#include "animate.h"
#include <stdio.h>
#include <unistd.h>
#include "globalVariables.h" 
#include "camera.h"
#include "collision.h"

bool car_timer_active = false;

bool brick_keyboard_timer_active = false;


extern Button bt_brick_up, bt_brick_down;

void car_on_timer(int value) {
	if (value != CAR_TIMER_ID) return;

	space.car.go();

	glutPostRedisplay();

	if (car_timer_active)
		glutTimerFunc(TIMER_INTERVAL, car_on_timer, CAR_TIMER_ID);
	else
		space.car.stop();
}

void brick_keyboard_on_timer(int value) {
	if (value != BRICK_KEYBOARD_TIMER_ID) return;

	if (space.selected_brick != -1) {
		Brick& current_brick = space.bricks[space.selected_brick];

		if (bt_brick_up.pressed)
			move_brick(Up,current_brick, space.brick_move_speed);

		if (bt_brick_down.pressed)
			move_brick(Down,current_brick, space.brick_move_speed);
	}

	if (!bt_brick_up.pressed && !bt_brick_down.pressed)
		brick_keyboard_timer_active = false;

	glutPostRedisplay();

	if (brick_keyboard_timer_active)
		glutTimerFunc(TIMER_INTERVAL, brick_keyboard_on_timer, BRICK_KEYBOARD_TIMER_ID);
}
