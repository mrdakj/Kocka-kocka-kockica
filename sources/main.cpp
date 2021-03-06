#include <GL/glut.h>
#include <string>
#include "../headers/global_variables.h"
#include "../headers/mouse.h"
#include "../headers/keyboard.h"
#include "../headers/utility.h"
#include "../headers/textures.h"
#include "../headers/models.h"

int window_width = 1100;
int window_height = 600;
int near_clipping_distance = 1;
int fovy = 40;
Room room;
Camera camera;

Model cylinder(std::string("models/legoCylinder.obj"));
Model wheel(std::string("models/wheel.obj"),std::string("models/wheel.mtl"));

static double projection_matrix_inverse[16] = {0.0};

static void set_window();
static void on_display(void);
static void on_reshape(int new_width, int new_height);
static void set_light();
static void draw_cursor(const ut::Point& A, const ut::Point& B, const ut::Point& C);
static void get_projection_matrix_inverse();

/* register callbacks */
static void glut_callback_functions();
/* crate bricks and add them to the room */
static void create_bricks();
static void opengl_init();

int main(int argc, char** argv)
{
	/* init glut */
	glutInit(&argc, argv);
	glutInitDisplayMode(
			GLUT_RGBA |
			GLUT_DOUBLE |
			GLUT_DEPTH
	);

	set_window();

	/* hide cursor */
	glutSetCursor(GLUT_CURSOR_NONE);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	/* register callbacks */
	glut_callback_functions();

	opengl_init();
	set_light();
	create_bricks();
	load_textures();

	/* load models */
	cylinder.load();
	wheel.load();

	glutMainLoop();

	return 0;
}

void set_window()
{
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Kocka kocka kockica");
}

void set_light()
{
	GLfloat light_ambient[] = {0.1, 0.1, 0.1 ,1};
	GLfloat light_diffuse[] = {1, 1, 1, 1};
	GLfloat light_specular[] = {0.3, 0.3, 0.3, 1};
	GLfloat linear_attenuation = 0.005;

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linear_attenuation);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}


void on_display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* set light at camera position */
	GLfloat light_position[] = {0, 0, 0, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	camera.look_at();

	/* draw room */
	room.render();

	/* draw cursor */
	draw_cursor(
		ut::Point(-0.02, -0.02, 0),
		ut::Point(0.02, -0.02, 0),
		ut::Point()
	);

	/* send new picture to the screen */
	glutSwapBuffers();
}

void on_reshape(int new_width, int new_height)
{
	if (new_height == 0)
		new_height = 1;

	window_width = new_width;
	window_height = new_height;

	set_cursor_in_center();

	float ratio = (float)new_width / new_height;

	glViewport(0, 0, new_width, new_height);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(fovy, ratio, near_clipping_distance, 100);

	get_projection_matrix_inverse();
}


void create_bricks()
{
	Brick brick_1(ut::Vector3f(1,16,0), Size(2,1,2), Color(0,1,0, 0.7));
	Brick brick_2(ut::Vector3f(0,0,0), Size(1,1,1), Color(1,0,0, 0.7));
	Brick brick_3(ut::Vector3f(0,5,0), Size(2,1,1), Color(0,0,1, 0.7));
	Brick brick_4(ut::Vector3f(5,0,0), Size(4,1,2), Color(0.5,0,0));
	Brick brick_5(ut::Vector3f(5,7,0), Size(1,1,1), Color(0.5,1,0.5));
	Brick brick_6(ut::Vector3f(5,8,0), Size(1,1,2), Color(1,1,1));
	Brick brick_7(ut::Vector3f(0,8,0), Size(1,2,1), Color(0,1,1));
	Brick brick_8(ut::Vector3f(0,15,0), Size(1,1,1), Color(0.7,0.8,0.4));

	Brick brick_9(ut::Vector3f(10,5,3), Size(2,1,1), Color(0,0,1));
	Brick brick_10(ut::Vector3f(12,1,3), Size(2,1,2), Color(0,0.8,0));
	Brick brick_11(ut::Vector3f(9,10,3), Size(1,1,1), Color(1,1,0));
	Brick brick_12(ut::Vector3f(15,15,3), Size(3,1,1), Color(1,1,0,0.7));


	room.add(brick_4);
	room.add(brick_5);
	room.add(brick_6);
	room.add(brick_7);
	room.add(brick_8);
	room.add(brick_9);
	room.add(brick_10);
	room.add(brick_11);

	/* transparent bricks in the end (important because of depth test)*/
	room.add(brick_1);
	room.add(brick_2);
	room.add(brick_3);
	room.add(brick_12);

}

void draw_cursor(const ut::Point& A, const ut::Point& B, const ut::Point& C)
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glLoadIdentity();
	glMultMatrixd(projection_matrix_inverse);
	glColor3f(0.6, 0.4, 0.8);
	ut::draw_triangle(A, B, C);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void get_projection_matrix_inverse()
{
	/* copied from https://gist.github.com/gyng/8921328 */

	GLdouble projection_matrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);

	GLdouble a = projection_matrix[0];
	GLdouble b = projection_matrix[5];
	GLdouble c = projection_matrix[10];
	GLdouble d = projection_matrix[14];
	GLdouble e = projection_matrix[11];

	projection_matrix_inverse[0] = 1.0 / a;
	projection_matrix_inverse[5] = 1.0 / b;
	projection_matrix_inverse[11] = 1.0 / d;
	projection_matrix_inverse[14] = 1.0 / e;
	projection_matrix_inverse[15] = -c / (d * e);
}

static void glut_callback_functions()
{
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(keyboard_ascii_down);
	glutKeyboardUpFunc(keyboard_ascii_up);
	glutMouseFunc(on_mouse_click);
	glutMotionFunc(on_mouse_active_move);
	glutPassiveMotionFunc(on_mouse_passive_move);
}

static void opengl_init()
{
	glEnable(GL_DEPTH_TEST);

	/* background color */
	glClearColor(0.188, 0.231, 0.245,1);

	/* for transparent objects */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

	/* enable color with light */
	glEnable(GL_COLOR_MATERIAL);
}
