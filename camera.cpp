#include "camera.h"
#include <stdio.h>
#include "globalVariables.h"

bool camera_timer_active = false;
float theta=-3.141592/2;
float phi=3.141592/2;
float theta_step=0;
float phi_step=0;
float move_forward = 0;
float move_left = 0;
float move_up=0;

extern Button bt_camera_rotation_up, bt_camera_rotation_down, bt_camera_rotation_left, bt_camera_rotation_right,
	   bt_camera_translate_up, bt_camera_translate_down, bt_camera_translate_left, bt_camera_translate_right,
	   bt_camera_translate_forward, bt_camera_translate_backward;

void get_vectors() {
	view = to;
	view.normalize();
	hvector = view * Vector3f(0,1,0);
	hvector.normalize();
	v=hvector*view;
	v.normalize();
	int nearClippingPlaneDistance = 1;
	int fovy = 40;
	float rad = fovy * 3.14159 / 180;
	float vLength = tan( rad / 2 ) * nearClippingPlaneDistance;
	float hLength = vLength * ((float)window_width / window_height);
	v = v*vLength;
	hvector = hvector*hLength;
}


void calculate_camera_forward_position() {
	float d = sqrt(to.x*to.x+to.z*to.z);
	camera_position.x += move_forward * to.x/d * 0.2;
	camera_position.z += move_forward * to.z/d * 0.2;
}

void calculate_camera_left_position() {
	float d = sqrt(to.x*to.x+to.z*to.z); 
	camera_position.x += move_left * to.z/d * 0.2;
	camera_position.z += move_left * -to.x/d * 0.2;
}

void calculate_camera_up_position() {
	camera_position.y += move_up;
}

void calculate_camera_look() {

	theta += theta_step;
	phi += phi_step;



	float PI=3.141592;
	if (phi >= PI)
		phi = PI;
	if (phi<0.001) {
		phi= 0.001;
	}

	float sin_phi = std::sin(phi);
	float cos_phi = std::cos(phi);
	float sin_theta = std::sin(theta);
	float cos_theta = std::cos(theta);

	to.x = sin_phi*cos_theta;
	to.z = sin_phi*sin_theta;
	to.y = -cos_phi;

	get_vectors();
}

void camera_on_timer(int value) {
	if (value != CAMERA_TIMER_ID) return;

	/* rotation */
	if (bt_camera_rotation_left.pressed)
		theta_step=-0.02;

	if (bt_camera_rotation_right.pressed)
		theta_step=0.02;

	if ((bt_camera_rotation_left.pressed == bt_camera_rotation_right.pressed) || space.selected_brick != NONE)
		theta_step = 0;

	if (bt_camera_rotation_up.pressed)
		phi_step=0.02;

	if (bt_camera_rotation_down.pressed)
		phi_step=-0.02;

	if ((bt_camera_rotation_up.pressed == bt_camera_rotation_down.pressed) || space.selected_brick != NONE)
		phi_step = 0;

	/* translation */
	if (bt_camera_translate_up.pressed)
		move_up = 0.02;
	if (bt_camera_translate_down.pressed)
		move_up = -0.02;
	if (bt_camera_translate_forward.pressed)
		move_forward = 0.5;
	if (bt_camera_translate_backward.pressed)
		move_forward = -0.5;
	if (bt_camera_translate_left.pressed)
		move_left = 0.5;
	if (bt_camera_translate_right.pressed)
		move_left = -0.5;

	if ((bt_camera_translate_up.pressed == bt_camera_translate_down.pressed) || space.selected_brick != NONE)
		move_up = 0;
	if ((bt_camera_translate_forward.pressed == bt_camera_translate_backward.pressed) || space.selected_brick != NONE)
		move_forward = 0;
	if ((bt_camera_translate_left.pressed == bt_camera_translate_right.pressed) || space.selected_brick != NONE)
		move_left = 0;

	if (!bt_camera_translate_up.pressed && !bt_camera_translate_down.pressed
		&& !bt_camera_translate_left.pressed && !bt_camera_translate_right.pressed
		&& !bt_camera_translate_forward.pressed && !bt_camera_translate_backward.pressed
		&& !bt_camera_rotation_up.pressed && !bt_camera_rotation_down.pressed
		&& !bt_camera_rotation_left.pressed && !bt_camera_rotation_right.pressed)
		camera_timer_active = false;


	if (theta_step != 0 || phi_step != 0)
		calculate_camera_look();

	if (move_up)
		calculate_camera_up_position();

	if (move_forward)
		calculate_camera_forward_position();

	if (move_left)
		calculate_camera_left_position();

	glutPostRedisplay();

	if (camera_timer_active)
		glutTimerFunc(TIMER_INTERVAL, camera_on_timer, CAMERA_TIMER_ID);
}
