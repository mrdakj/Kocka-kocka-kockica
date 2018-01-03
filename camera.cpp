#include <GL/glut.h>
#include "headers/camera.h"

#define PI 3.141592

Camera::Camera() {
	view = Vector3f(0, 0 , -1);
	position = Vector3f(0 ,3, 4);

	theta = -PI/2;
	phi = PI/2;
}

void Camera::look_at() const {
	gluLookAt(
		position.x, position.y, position.z,
		position.x+view.x, position.y+view.y, position.z+view.z,
		0, 1, 0
	);
}


void Camera::look_at_point(const Vector3f& A) {
	view = A - position;
	view.normalize();
}

void Camera::recover_angles() {
	phi = std::acos(-view.y);
	theta = std::acos(view.x/(std::sin(phi)));
	if (view.z<0)
		theta *= -1;

	/* important */
	if (std::isnan(theta)) {
		theta = 0;
	}
}

void Camera::move_forward(float forward_delta) {
	float d = std::sqrt(view.x*view.x+view.z*view.z);
	position += (forward_delta/d*0.2)*Vector3f(view.x, 0, view.z);
}

void Camera::move_left(float left_delta) {
	float d = std::sqrt(view.x*view.x+view.z*view.z);
	position += (left_delta/d*0.2)*Vector3f(view.z, 0, -view.x);
}

void Camera::move_up(float up_delta) {
	position.y += up_delta;
}

void Camera::rotate(float theta_delta, float phi_delta) {

	theta += theta_delta;
	phi += phi_delta;

	if (phi >= PI) 
		phi = PI;
	if (phi < 0.001)
		phi = 0.001;

	float sin_phi = std::sin(phi);
	float cos_phi = std::cos(phi);
	float sin_theta = std::sin(theta);
	float cos_theta = std::cos(theta);

	view = Vector3f(sin_phi*cos_theta, -cos_phi, sin_phi*sin_theta);
}


void Camera::move_on_keyboard(float theta_delta, float phi_delta, float forward_delta, float left_delta, float up_delta) {

	if (theta_delta || phi_delta)
		rotate(theta_delta, phi_delta);

	if (up_delta)
		move_up(up_delta);

	if (forward_delta)
		move_forward(forward_delta);

	if (left_delta)
		move_left(left_delta);
}
