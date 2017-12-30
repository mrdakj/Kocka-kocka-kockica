#include <GL/glut.h>
#include "global_variables.h"

#include "mouse.h"
#include "keyboard.h"
#include "utility.h"

int window_width;
int window_height;
Space space;

extern Vector3f view, horizontal_vector, vertical_vector;
extern Vector3f camera_position, to;

static double projection_matrix_inverse[16]={0.0};

void set_window();
void on_display(void);
void on_reshape(int new_width, int new_height);
void set_light();
void draw_cursor(const ut_Point& A, const ut_Point& B, const ut_Point& C);
void get_projection_matrix_inverse();
extern void get_vectors();

/* crate bricks and add them to the space */
void create_bricks();

int main(int argc, char** argv) {

	/* init glut */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	set_window();


	glEnable(GL_DEPTH_TEST);
	glClearColor(0.16,0.16,0.16,1);

	/* hide cursor */
	glutSetCursor(GLUT_CURSOR_NONE);

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	/* Light */
	set_light();

	create_bricks();

	/* register callbacks */
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(keyboard_ascii_down);
	glutSpecialFunc(keyboard_special_down);
	glutSpecialUpFunc(keyboard_special_up);
	glutKeyboardUpFunc(keyboard_ascii_up);
	glutMouseFunc(mouse);
    glutMotionFunc(on_mouse_active_move);
    glutPassiveMotionFunc(on_mouse_passive_move);

	glutMainLoop();
	
	return 0;
}

void set_window() {
	window_width=1100;
	window_height=600;

	glutInitWindowPosition(0,0);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Kocka kocka kockica");
}

void set_light() {
	GLfloat light_ambient[]={0.1, 0.1, 0.1 ,1};
	GLfloat light_diffuse[]={1, 1, 1, 1};
	GLfloat light_specular[]={0.3, 0.3, 0.3, 1};
	GLfloat linear_attenuation = 0.005;

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linear_attenuation);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}



void on_display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* set light at camera position */
	GLfloat light_position[] = {0,0,0,1};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	gluLookAt(
		camera_position.x,camera_position.y,camera_position.z,
		camera_position.x+to.x, camera_position.y+to.y, camera_position.z+to.z,
		0, 1, 0
	);


	/* draw space */
	space.render();


	/* draw cursor */
	draw_cursor(
		ut_Point(-0.02, -0.02, 0),
		ut_Point(0.02, -0.02, 0),
		ut_Point()
	);

	glutSwapBuffers();
}

void on_reshape(int new_width, int new_height) {
	if (new_height == 0)
		new_height = 1;

	window_width = new_width;
	window_height = new_height;


	glutWarpPointer(window_width/2,window_height/2);

	float ratio = (float)new_width / new_height;

	glViewport(0, 0, new_width, new_height);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(40, ratio, 1, 400);

	get_vectors();

	get_projection_matrix_inverse();
}


void create_bricks() {
    Brick brick_1(Position(4,5,0), Size(2,1,2), Color(1,0,0));
    Brick brick_2(Position(0,0,0), Size(1,2,1), Color(0,1,0));
    Brick brick_3(Position(0,5,0), Size(2,1,1), Color(0,0,1));
    Brick brick_4(Position(5,0,0), Size(4,1,2), Color(1,0,0));

	space.add(brick_1);
	space.add(brick_2);
	space.add(brick_3);
	space.add(brick_4);
}

void draw_cursor(const ut_Point& A, const ut_Point& B, const ut_Point& C) {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glLoadIdentity();
	glMultMatrixd(projection_matrix_inverse);
	glColor3f(0.6,0.4,0.8);
	ut_draw_triangle(A, B, C);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void get_projection_matrix_inverse() {
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
