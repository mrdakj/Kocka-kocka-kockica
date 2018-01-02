#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vector3f.h"

class Camera {

private:
	float theta;
	float phi;

	/* translation */
	void move_forward(float forward_delta);
	void move_left(float left_delta);
	void move_up(float up_delta);

public:
	Vector3f view;
	Vector3f position;

	Camera();
	void look_at() const;

	/* get theta and phi from view vector */
	void recover_angles();

	/* rotation */
	void rotate(float theta_delta, float phi_delta);

	/* rotate and translate for given arguments */
	void move_on_keyboard(float theta_delta, float phi_delta, float forward_delta, float left_delta, float up_delta);

};


#endif
