#include "globalVariables.h"
#include "keyboard.h"
#include <GL/glut.h>
#include "camera.h"

#define unused_function_arg(x) ((void)x)

extern bool camera_timer_active;

/* exit button */
Button bt_exit(27);

/* brick buttons */
Button bt_brick_up('e');
Button bt_brick_down('q');

/* camera buttons */
Button bt_camera_rotation_left('j');
Button bt_camera_rotation_right('l');
Button bt_camera_rotation_up('i');
Button bt_camera_rotation_down('k');

Button bt_camera_translate_up('o');
Button bt_camera_translate_down('u');
Button bt_camera_translate_left(GLUT_KEY_LEFT);
Button bt_camera_translate_right(GLUT_KEY_RIGHT);
Button bt_camera_translate_forward(GLUT_KEY_UP);
Button bt_camera_translate_backward(GLUT_KEY_DOWN);

/* animation buttons */
Button bt_animation_go('g');
Button bt_animation_stop('s');

void activate_camera_timer() {
	if (!camera_timer_active) {
		camera_timer_active = true;
		glutTimerFunc(10, camera_on_timer, CAMERA_TIMER_ID);
	}
}

void keyboard_ascii_down(unsigned char key, int x, int y) {
	unused_function_arg(x);
	unused_function_arg(y);

	/* exit program */
	if (key == bt_exit.key)
		exit(0);

	/* camera rotation */
	if (key == bt_camera_rotation_left.key) {
		bt_camera_rotation_left.change_state();
		activate_camera_timer();
	}
	if (key == bt_camera_rotation_right.key) {
		bt_camera_rotation_right.change_state();
		activate_camera_timer();
	}
	if (key == bt_camera_rotation_up.key) {
		bt_camera_rotation_up.change_state();
		activate_camera_timer();
	}
	if (key == bt_camera_rotation_down.key) {
		bt_camera_rotation_down.change_state();
		activate_camera_timer();
	}

	/* camera translation */
	if (key == bt_camera_translate_up.key) {
		bt_camera_translate_up.change_state();
		activate_camera_timer();
	}
	if (key == bt_camera_translate_down.key) {
		bt_camera_translate_down.change_state();
		activate_camera_timer();
	}

	/* brick up and down */
	if (key == bt_brick_down.key)
		bt_brick_down.change_state();
	if (key == bt_brick_up.key)
		bt_brick_up.change_state();

	/* animation */
	if (key == bt_animation_go.key)
		go = true;
	if (key == bt_animation_stop.key)
		go = false;
}

void keyboard_ascii_up(unsigned char key, int x, int y) {
	unused_function_arg(x);
	unused_function_arg(y);

	if (key == bt_camera_rotation_left.key) 
		bt_camera_rotation_left.change_state();
	if (key == bt_camera_rotation_right.key)
		bt_camera_rotation_right.change_state();
	if (key == bt_camera_rotation_up.key)
		bt_camera_rotation_up.change_state();
	if (key == bt_camera_rotation_down.key)
		bt_camera_rotation_down.change_state();

	if (key == bt_camera_translate_down.key)
		bt_camera_translate_down.change_state();
	if (key == bt_camera_translate_up.key)
		bt_camera_translate_up.change_state();

	if (key == bt_brick_down.key)
		bt_brick_down.change_state();
	if (key == bt_brick_up.key)
		bt_brick_up.change_state();
}

void keyboard_special_down(int key, int x, int y) {
	unused_function_arg(x);
	unused_function_arg(y);

	if (key == bt_camera_translate_forward.key) {
		bt_camera_translate_forward.change_state();
		activate_camera_timer();
	}
	if (key == bt_camera_translate_backward.key) {
		bt_camera_translate_backward.change_state();
		activate_camera_timer();
	}
	if (key == bt_camera_translate_left.key) {
		bt_camera_translate_left.change_state();
		activate_camera_timer();
	}
	if (key == bt_camera_translate_right.key) {
		bt_camera_translate_right.change_state();
		activate_camera_timer();
	}
}

void keyboard_special_up( int key, int x, int y ) { 
	unused_function_arg(x);
	unused_function_arg(y);

	if (key == bt_camera_translate_forward.key)
		bt_camera_translate_forward.change_state();
	if (key == bt_camera_translate_backward.key)
		bt_camera_translate_backward.change_state();
	if (key == bt_camera_translate_left.key)
		bt_camera_translate_left.change_state();
	if (key == bt_camera_translate_right.key)
		bt_camera_translate_right.change_state();

}



