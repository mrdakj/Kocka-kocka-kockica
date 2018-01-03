#include "headers/mouse.h"
#include "headers/utility.h"
#include "headers/global_variables.h"
#include "headers/camera.h"
#include "headers/collision.h"
#include <stdio.h>
#include <limits>
#include <array>

#define INF (std::numeric_limits<int>::max())


struct Mouse {
	float sensitivity;
	bool right_button_down;
	bool left_button_down;

	Mouse(float sensitivity) : sensitivity(sensitivity), right_button_down(false), left_button_down(false) {}
};

static Mouse mouse(0.01);


float objX, objY, objZ;

static void get_axes_of_near_clipping_plane(Vector3f& x_axis, Vector3f& y_axis) {
	x_axis = camera.view * Vector3f(0,1,0);
	x_axis.normalize();

	y_axis = x_axis * camera.view;
	y_axis.normalize();

	float fovy_radians = fovy * PI / 180;

	float y_length = std::tan(fovy_radians/2) * near_clipping_distance;
	float x_length = y_length * ((float)window_width/window_height);
	
	y_axis = y_length * y_axis;
	x_axis = x_length * x_axis;
}

/* transform origin from the top left corner to the center of the screen */
static void transform_coordinates(float& x, float& y) {
	x -= window_width/2.0;
	y = window_height/2.0 - y;

	y /= window_height/2.0;
	x /= window_width/2.0;
}

static Vector3f get_point_at_near_clipping_plane(float& x, float& y) {
	Vector3f x_axis, y_axis;
	get_axes_of_near_clipping_plane(x_axis, y_axis);

	transform_coordinates(x, y);

	return camera.position + near_clipping_distance*camera.view + x*x_axis + y*y_axis;
}

/* get a direction of mouse picking ray */
static Vector3f get_ray_direction(float x, float y) {

	Vector3f point_at_clipping_plane = get_point_at_near_clipping_plane(x, y);
	
	Vector3f direction = point_at_clipping_plane - camera.position;

	return direction;
}

/* ax+by+cz+d=0 */
struct Plane {
	int a, b, c, d;
	Plane(int a, int b, int c, int d) : a(a), b(b), c(c), d(d) {};
};

/* intersect ray with plane in the space */
static Vector3f	intersect_ray_and_plane(int x, int y, const Plane& plane) {
	Vector3f direction = get_ray_direction(x, y);
	Vector3f plane_up = Vector3f(plane.a, plane.b, plane.c);
	float k = (-plane.d-camera.position.dot(plane_up)) / direction.dot(plane_up);

	Vector3f A = camera.position + k*direction;

	if (std::isnan(k) || k < near_clipping_distance || A.x < 0 || A.x > space.size || A.y < 0 || A.y > space.size || A.z > 0 || A.z < -space.size)
		A.set_nan();

	return A;
}

static Vector3f get_intersection_vector(const Vector3f& A) {

	Vector3f intersection_vector = (A.is_nan()) ? A :
									A-camera.position;

	return intersection_vector;
}


enum Plane_family {
	X, //planes with up vector (1, 0, 0) 
	Y, //planes with up vector (0, 1, 0)
	Z  //planes with up vector (0, 0, 1)
};

struct PlanesXYZ {
	float min_distance;
	int id;
	PlanesXYZ() : min_distance(INF), id(-1) {}

	void reset() {
		min_distance = INF;
		id = -1;
	}

	/* try to update distance and id */
	bool update(float distance, int id) {
		if (id==0 || id==255 || id == -1) return false;

		if (distance < min_distance) {
			this->id = id;
			min_distance = distance;
			return true;
		}

		return false;
	}
};

static std::array<PlanesXYZ, 3> planesXYZ = {{PlanesXYZ(), PlanesXYZ(), PlanesXYZ()}};

static Plane get_plane(const Plane_family& plane_family, int d) {
/* for plane_family=X, returns Plane x = d */
/* for plane_family=Y, returns Plane y = d */
/* for plane_family=Z, returns Plane z = -d */

	return Plane(
			plane_family == X,
			plane_family == Y,
			plane_family == Z,
			(plane_family == Z) ? d : -d
	);
}

void on_mouse_click(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON &&  state == GLUT_DOWN) { 

		mouse.left_button_down = true;

		x = window_width/2;
		y = window_height/2;


		for (Plane_family plane_family : {X, Y, Z}) {
			planesXYZ[plane_family].reset();

			for (int d = 0; d <= space.size; d++) {
				/* this is a vector OA, or just a point A */
				Vector3f A = intersect_ray_and_plane(x, y, get_plane(plane_family, d));

				Vector3f intersection_vector = get_intersection_vector(A);
				if (intersection_vector.is_nan())
					continue;
				float distance = intersection_vector.norm_squared();

				/* matrix indices */
				/* important to set d instead of A.* because of float precision */
				int x = (plane_family == X) ? d : A.x;
				int y = (plane_family == Y) ? d : A.y;
				int z = (plane_family == Z) ? d : -A.z;

				int id;
				if (plane_family == X)
					id = space.get_matrix_field(x-1,z,y);
				if (plane_family == Y)
					id = space.get_matrix_field(x,z,y-1);
				if (plane_family == Z)
					id = space.get_matrix_field(x,z-1,y);

				if (planesXYZ[plane_family].update(distance, id)) {
					objX = A.x;
					objY = A.y+0.2;
					objZ = A.z;
				}

				if (planesXYZ[plane_family].update(distance, space.get_matrix_field(x,z,y))) {
					objX = A.x;
					objY = A.y+0.2;
					objZ = A.z;
				}

			}
		}

		
		int plane_family_with_min_distance = ut_index_of_minimum(planesXYZ[X].min_distance, planesXYZ[Y].min_distance, planesXYZ[Z].min_distance);

		int id = planesXYZ[plane_family_with_min_distance].id;

		space.pick(id);

		if (space.selected_brick != -1) {
			camera.view.x = objX-camera.position.x;
			camera.view.z = objZ-camera.position.z;
			camera.view.y = objY-camera.position.y;

			float modultheta = std::sqrt(camera.view.x*camera.view.x+camera.view.z*camera.view.z+camera.view.y*camera.view.y);

			camera.view.x /= modultheta;
			camera.view.z /= modultheta;
			camera.view.y /= modultheta;

		}

		glutPostRedisplay();
	}	


	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		mouse.left_button_down=false;
		space.put_down();

		camera.recover_angles();

		glutPostRedisplay();
	}

	if (button == GLUT_RIGHT_BUTTON &&  state == GLUT_DOWN) { 
		mouse.right_button_down=true;
	}

	if (button == GLUT_RIGHT_BUTTON &&  state == GLUT_UP) { 
		mouse.right_button_down=false;
	}

}

static bool mouse_at_center(int x, int y) {
	return x == window_width/2 && y == window_height/2;
}

void mouse_look(int x, int y) {
	float theta_step = (x - window_width/2) * 0.0015;
	float phi_step = - (y - window_height/2) * 0.0015;

	if (theta_step)
		camera.rotate(theta_step, phi_step);

	if (phi_step)
		camera.rotate(theta_step, phi_step);

	if (!mouse_at_center(x, y))
		glutWarpPointer(window_width/2, window_height/2);
}


void on_mouse_active_move(int x, int y) {

	if (space.selected_brick == NONE) {
		mouse_look(x,y);
		glutPostRedisplay();
		return;
	}
	
	if (mouse_at_center(x, y)) return;

	Brick& current_brick = space.bricks[space.selected_brick];

	float delta_x = x-window_width/2;
	delta_x *= mouse.sensitivity;

	float delta_y = -y+window_height/2;
	delta_y *= mouse.sensitivity;


	if (mouse.left_button_down && !mouse.right_button_down) {
		move_delta(delta_x, delta_y, current_brick);
	}

	if (mouse.left_button_down && mouse.right_button_down) {
		if (delta_y > 0.9) delta_y = 0.9;
		if (delta_y < -0.9) delta_y = -0.9;
		if (delta_y > 0)
			move_brick(Up, current_brick, delta_y);
		else if (delta_y < 0)
			move_brick(Down, current_brick, -delta_y);
	}

	if (!mouse_at_center(x, y))
		glutWarpPointer(window_width/2, window_height/2);

	glutPostRedisplay();
}


void on_mouse_passive_move(int x, int y) {
	mouse_look(x, y);
	glutPostRedisplay();
}
