#include "headers/global_variables.h"
#include "headers/keyboard.h"
#include "headers/textures.h"

#define unused_function_arg(x) ((void)x)

/* define extern variables */

/* exit button */
Button bt_exit(27);

/* animation buttons */
Button bt_animation_go('n');
Button bt_animation_stop('m');

/* brick buttons */
std::vector<Button> brick_buttons = {
	Button('e'), // translation up
	Button('q'), // translation down
	Button('a'), // translation left
	Button('d'), // translation right
	Button('w'), // translation forward
	Button('s')  // translation backward
};


/* camera buttons */
std::vector<Button> camera_buttons = {
	Button('e'), // translation up
	Button('q'), // translation down
	Button('a'), // translation left
	Button('d'), // translation right
	Button('w'), // translation forward
	Button('s'), // translation backward
	Button('j'), // rotation left
	Button('l'), // rotation right
	Button('i'), // rotation up
	Button('k')  // rotation down
};
/* end of extern variables */

/* private functions */

/* change button state with key key and activate timer if needed (if it is not NULL) */
static void action(unsigned char key, std::vector<Button>& buttons, Timer* timer);

/* end private functions */

/* function definitions */

static void action(unsigned char key, std::vector<Button>& buttons, Timer* timer) {
	for (Button& bt : buttons) {
		if (key == bt.key) {
			bt.change_state();
			if (timer != NULL)
				timer->activate();
		}
	}
}

bool all_released(const std::vector<Button>& buttons) {
	for (const Button& bt : buttons) {
		if (bt.pressed) return false;
	}

	return true;
}


void keyboard_ascii_down(unsigned char key, int x, int y) {

	unused_function_arg(x);
	unused_function_arg(y);

	/* exit program */
	if (key == bt_exit.key) {
        glDeleteTextures(2, texture_names);
		exit(0);
	}

	/* camera */
	action(key, camera_buttons, &t_camera);

	/* brick */
	action(key, brick_buttons, &t_brick);

	/* animation */
	if (key == bt_animation_go.key)
		t_car.activate();

	if (key == bt_animation_stop.key) {
		t_car.stop();
		room.car.stop();
	}
}

void keyboard_ascii_up(unsigned char key, int x, int y) {
	unused_function_arg(x);
	unused_function_arg(y);

	/* camera */
	action(key, camera_buttons, NULL);

	/* brick */
	action(key, brick_buttons, NULL);
}

/* end of function definitions */

