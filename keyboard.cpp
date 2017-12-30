#include <GL/glut.h>
#include "global_variables.h"
#include "keyboard.h"
#include <stdio.h>

#define unused_function_arg(x) ((void)x)


/* exit button */
Button bt_exit(27);

/* brick buttons */
Button bt_brick_up('e');
Button bt_brick_down('q');

/* animation buttons */
Button bt_animation_go('n');
Button bt_animation_stop('m');

/* camera buttons */
std::vector<Button> camera_buttons = {
	Button('j'), // rotation left
	Button('l'), // rotation right
	Button('i'), // rotation up
	Button('k'), // rotation down
	Button('e'), // transtale up
	Button('q'), // translate down
	Button('a'), // translate left
	Button('d'), // translate right
	Button('w'), // translate forward
	Button('s') // tranlate backward
};


void keyboard_ascii_down(unsigned char key, int x, int y) {
	unused_function_arg(x);
	unused_function_arg(y);


	/* exit program */
	if (key == bt_exit.key)
		exit(0);

	for (Button& bt : camera_buttons) {
		if (key == bt.key) {
			bt.change_state();
			t_camera.activate();
		}
	}

	/* brick up and down */
	if (key == bt_brick_down.key) {
		bt_brick_down.change_state();
		t_brick.activate();
	}
	if (key == bt_brick_up.key) {
		bt_brick_up.change_state();
		t_brick.activate();
	}

	/* animation */
	if (key == bt_animation_go.key) {
		t_car.activate();
	}
	if (key == bt_animation_stop.key) {
		t_car.stop();
		space.car.stop();
	}
}

void keyboard_ascii_up(unsigned char key, int x, int y) {
	unused_function_arg(x);
	unused_function_arg(y);

	for (Button& bt : camera_buttons) {
		if (key == bt.key) {
			bt.change_state();
		}
	}

	if (key == bt_brick_down.key)
		bt_brick_down.change_state();
	if (key == bt_brick_up.key)
		bt_brick_up.change_state();
}

void keyboard_special_down(int key, int x, int y) {
	unused_function_arg(x);
	unused_function_arg(y);

	for (Button& bt : camera_buttons) {
		if (key == bt.key) {
			bt.change_state();
			t_camera.activate();
		}
	}
}

void keyboard_special_up( int key, int x, int y ) { 
	unused_function_arg(x);
	unused_function_arg(y);

	for (Button& bt : camera_buttons) {
		if (key == bt.key) {
			bt.change_state();
		}
	}
}



