#include <GL/glut.h>
#include "camera.h"
#include <stdio.h>
#include "global_variables.h"
#include "vector3f.h"
#include "keyboard.h"

Vector3f to(0,0,-1);
Vector3f view, horizontal_vector, vertical_vector;
Vector3f camera_position(0,3,4);

Timer t_camera(camera_on_timer);

static float theta = -PI/2;
static float phi = PI/2;
static float theta_step = 0;
static float phi_step = 0;
static float move_forward = 0;
static float move_left = 0;
static float move_up = 0;

void get_vectors() {
	view = to;
	view.normalize();
	horizontal_vector = view * Vector3f(0,1,0);
	horizontal_vector.normalize();
	vertical_vector=horizontal_vector*view;
	vertical_vector.normalize();
	int nearClippingPlaneDistance = 1;
	int fovy = 40;
	float rad = fovy * 3.14159 / 180;
	float vLength = std::tan( rad / 2 ) * nearClippingPlaneDistance;
	float hLength = vLength * ((float)window_width / window_height);
	vertical_vector = vertical_vector*vLength;
	horizontal_vector = horizontal_vector*hLength;
}

void recover_angles() {
	phi = std::acos(-to.y);
	theta = std::acos(to.x/(std::sin(phi)));
	if (to.z<0)
		theta *= -1;

	/* important */
	if (std::isnan(theta)) {
		printf("here\n");
		theta = 0;
	}

	get_vectors();
}

void calculate_camera_forward_position() {
	float d = std::sqrt(to.x*to.x+to.z*to.z);
	camera_position.x += move_forward * to.x/d * 0.2;
	camera_position.z += move_forward * to.z/d * 0.2;
}

void calculate_camera_left_position() {
	float d = std::sqrt(to.x*to.x+to.z*to.z);
	camera_position.x += move_left * to.z/d * 0.2;
	camera_position.z += move_left * -to.x/d * 0.2;
}

void calculate_camera_up_position() {
	camera_position.y += move_up;
}

void calculate_camera_look(float theta_step, float phi_step) {

	theta += theta_step;
	phi += phi_step;

	if (phi >= PI) phi = PI;
	if (phi < 0.001) phi= 0.001;

	float sin_phi = std::sin(phi);
	float cos_phi = std::cos(phi);
	float sin_theta = std::sin(theta);
	float cos_theta = std::cos(theta);

	to.x = sin_phi*cos_theta;
	to.z = sin_phi*sin_theta;
	to.y = -cos_phi;

	get_vectors();
}

/* get steps from keyboard buttons */
void get_theta_and_phi_step() {
	theta_step = camera_buttons[rotation_left].pressed ? -0.02 :
				 camera_buttons[rotation_right].pressed ? 0.02 : 0;

	if (camera_buttons[rotation_left].pressed == camera_buttons[rotation_right].pressed)
		theta_step = 0;

	phi_step = camera_buttons[rotation_up].pressed ? 0.02 :
			   camera_buttons[rotation_down].pressed ? -0.02 : 0;

	if (camera_buttons[rotation_up].pressed == camera_buttons[rotation_down].pressed)
		phi_step = 0;
}

/* get move_up, move_left and move_forward from keyboard buttons */
void get_move_up_left_forward() {
	move_up = camera_buttons[translation_up].pressed ? 0.05 :
			  camera_buttons[translation_down].pressed ? -0.05 : 0;

	if (camera_buttons[translation_up].pressed == camera_buttons[translation_down].pressed)
		move_up = 0;

	move_left = camera_buttons[translation_left].pressed ? 0.5 :
				camera_buttons[translation_right].pressed ? -0.5 : 0;

	if (camera_buttons[translation_left].pressed == camera_buttons[translation_right].pressed)
		move_left = 0;

	move_forward = camera_buttons[translation_forward].pressed ? 0.5 :
				   camera_buttons[translation_backward].pressed ? -0.5 : 0;

	if (camera_buttons[translation_forward].pressed == camera_buttons[translation_backward].pressed)
		move_forward = 0;
}

void camera_on_timer(int value) {
	if (!t_camera.check(value)) return;

	if (space.selected_brick == NONE) {
		/* rotation */
		get_theta_and_phi_step();

		/* translation */
		get_move_up_left_forward();

		if (theta_step || phi_step) calculate_camera_look(theta_step, phi_step);

		if (move_up) calculate_camera_up_position();

		if (move_forward) calculate_camera_forward_position();

		if (move_left) calculate_camera_left_position();

		glutPostRedisplay();
	}


	if (all_released(camera_buttons))
		t_camera.stop();

	t_camera.cont();
}
