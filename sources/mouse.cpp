#include <limits>
#include <array>
#include <cmath>
#include "../headers/mouse.h"
#include "../headers/utility.h"
#include "../headers/global_variables.h"
#include "../headers/camera.h"

#define INF (std::numeric_limits<int>::max())

#define unused_function_arg(x) ((void)x)

struct Mouse {
	float sensitivity;
	bool right_button_down;
	bool left_button_down;

	Mouse(float sensitivity) :
		sensitivity(sensitivity),
		right_button_down(false),
		left_button_down(false) {}
};

static Mouse mouse(0.01);

/* world coordinates of selection */
Vector3f selection;

static void get_axes_of_near_clipping_plane(Vector3f& x_axis, Vector3f& y_axis)
{
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
static void transform_coordinates(float& x, float& y)
{
	x -= window_width/2.0;
	y = window_height/2.0 - y;

	y /= window_height/2.0;
	x /= window_width/2.0;
}

static Vector3f get_point_at_near_clipping_plane(float x, float y)
{
	Vector3f x_axis, y_axis;
	get_axes_of_near_clipping_plane(x_axis, y_axis);

	transform_coordinates(x, y);

	return camera.position + near_clipping_distance*camera.view + x*x_axis + y*y_axis;
}

/* get a direction of mouse picking ray */
static Vector3f get_ray_direction(float x, float y)
{
	Vector3f point_at_clipping_plane = get_point_at_near_clipping_plane(x, y);

	Vector3f direction = point_at_clipping_plane - camera.position;

	return direction;
}

/* ax+by+cz+d=0 */
struct Plane {
	int a, b, c, d;
	Plane(int a, int b, int c, int d) : a(a), b(b), c(c), d(d) {};
};

/* intersect ray with plane in the room and set distance from camera postion to intersection point */
static Vector3f	intersect_ray_and_plane(float x, float y, const Plane& plane, float& distance)
{
	Vector3f direction = get_ray_direction(x, y);
	Vector3f plane_up = Vector3f(plane.a, plane.b, plane.c);
	float k = (-plane.d-camera.position.dot(plane_up)) / direction.dot(plane_up);

	Vector3f A = camera.position + k*direction;

	if (std::isnan(k) || k < near_clipping_distance || A.x < 0 || A.x > room.size || A.y < 0 || A.y > room.size || A.z > 0 || A.z < -room.size)
		A.set_nan();

	distance = k;

	return A;
}


enum Plane_family {
	X, //planes with up vector (1, 0, 0)
	Y, //planes with up vector (0, 1, 0)
	Z  //planes with up vector (0, 0, 1)
};

struct PlanesXYZ {
	float min_distance;
	int id;
	Vector3f selection;
	PlanesXYZ() : min_distance(INF), id(-1), selection(Vector3f(0 ,0 ,0)) {}

	void reset() {
		selection = Vector3f(0, 0, 0);
		min_distance = INF;
		id = -1;
	}

	/* try to update distance, id and selection */
	bool update(float distance, int id, const Vector3f& A) {
		if (id==0 || id==255 || id == -1) return false;

		if (distance < min_distance) {
			selection = A;
			this->id = id;
			min_distance = distance;
			return true;
		}

		return false;
	}
};

static std::array<PlanesXYZ, 3> planesXYZ = {{PlanesXYZ(), PlanesXYZ(), PlanesXYZ()}};

static Plane get_plane(const Plane_family& plane_family, int d)
{
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

/* get id of the brick under the mouse cursor, if there is no such brick return -1 */
static int get_id(float mouse_x, float mouse_y)
{
	for (Plane_family plane_family : {X, Y, Z}) {
		planesXYZ[plane_family].reset();

		for (int d = 0; d <= room.size; d++) {
			/* this is a vector OA, or just a point A */
			float distance;
			Vector3f A = intersect_ray_and_plane(mouse_x, mouse_y, get_plane(plane_family, d), distance);

			if (A.is_nan()) continue;

			/* matrix indices */
			/* important to set d instead of A.* because of float precision */
			int x = (plane_family == X) ? d : A.x;
			int y = (plane_family == Y) ? d : A.y;
			int z = (plane_family == Z) ? d : -A.z;

			int id = room.get_matrix_field(
					x - (plane_family == X),
					z - (plane_family == Z),
					y - (plane_family == Y)
			);

			planesXYZ[plane_family].update(distance, id, A);

			id = room.get_matrix_field(x,z,y);

			planesXYZ[plane_family].update(distance, id, A);

		}
	}

	int plane_family_with_min_distance = ut_index_of_minimum(planesXYZ[X].min_distance, planesXYZ[Y].min_distance, planesXYZ[Z].min_distance);

	selection = planesXYZ[plane_family_with_min_distance].selection + Vector3f(0, 0.2, 0);

	return planesXYZ[plane_family_with_min_distance].id;
}


void on_mouse_click(int button, int state, int x, int y)
{
	unused_function_arg(x);
	unused_function_arg(y);

	if (room.car.is_going)
		return;

	if (button == GLUT_LEFT_BUTTON &&  state == GLUT_DOWN) {

		/* left click is pressed, try to select a brick */

		mouse.left_button_down = true;


		int id = get_id(window_width/2.0, window_height/2.0);

		room.pick(id);

		if (!room.nothing_selected()) {
			camera.look_at_point(selection);
			glutPostRedisplay();
		}
	}


	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {

		/* left button is released, put previously seleced brick down */

		mouse.left_button_down = false;

		room.put_down();
		camera.recover_angles();
		glutPostRedisplay();
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		mouse.right_button_down=true;

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		mouse.right_button_down=false;

}

void set_cursor_in_center()
{
	glutWarpPointer(window_width/2, window_height/2);
}

static bool mouse_in_center(int x, int y)
{
	return x == window_width/2 && y == window_height/2;
}

void mouse_look(int x, int y)
{
	float theta_step = (x - window_width/2) * 0.0015;
	float phi_step = - (y - window_height/2) * 0.0015;

	if (theta_step)
		camera.rotate(theta_step, phi_step);

	if (phi_step)
		camera.rotate(theta_step, phi_step);

	if (!mouse_in_center(x, y))
		set_cursor_in_center();
}


void on_mouse_active_move(int x, int y)
{
	if (room.nothing_selected()) {
		mouse_look(x,y);
		glutPostRedisplay();
		return;
	}

	if (mouse_in_center(x, y))
		return;

	float delta_x = x-window_width/2;
	delta_x *= mouse.sensitivity;

	float delta_y = -y+window_height/2;
	delta_y *= mouse.sensitivity;


	if (mouse.left_button_down && !mouse.right_button_down) {

		float distance =Vector3f(selection.x-camera.position.x, 0, selection.z-camera.position.z).norm_squared();

		if (distance<1*1 && delta_y<0)
			delta_y = 0;

		selection += room.move_selected_brick(camera.view, delta_x, delta_y);
	}

	if (mouse.left_button_down && mouse.right_button_down) {
		if (delta_y > 0.9) delta_y = 0.9;
		if (delta_y < -0.9) delta_y = -0.9;
		if (delta_y > 0)
			selection += room.move_selected_brick(Up, delta_y);
		else if (delta_y < 0)
			selection += room.move_selected_brick(Down, -delta_y);
	}

	if (!mouse_in_center(x, y))
		set_cursor_in_center();

	camera.look_at_point(selection);

	glutPostRedisplay();
}


void on_mouse_passive_move(int x, int y)
{
	mouse_look(x, y);
	glutPostRedisplay();
}
