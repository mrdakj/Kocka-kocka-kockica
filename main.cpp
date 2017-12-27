#include <GL/glut.h>
#include <cmath>
#include <stdio.h>
#include "vector3f.h"

#include "globalVariables.h"
#include "animate.h"
#include "space.h" 
#include "keyboard.h"
#include "mouse.h"
#include "camera.h"
#include "loadModel.h"
#include "utility.h"
#include "collision.h"

// define global variables
int window_width=1100;
int window_height=600;
Space space;
Vector3f to(0,0,-1);
Vector3f view, horizontal_vector, vertical_vector;

GLdouble objX=0;
GLdouble objY=0;
GLdouble objZ=0;
//--------------------------------

void set_window();
void on_display(void);
void on_reshape(int w, int h);
void get_vectors();
void set_light();
void draw_cursor(const ut_Point& A, const ut_Point& B, const ut_Point& C);

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


static double projection_inverse[16]={0.0};

void on_display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* set light at camera position */
	GLfloat light_position[] = {0,0,0,1};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	gluLookAt(camera_position.x,camera_position.y,camera_position.z,
			  camera_position.x+to.x, camera_position.y+to.y, camera_position.z+to.z,
			  0, 1, 0);


	/* draw space */
	space.render();


	/* draw cursor */
	draw_cursor(ut_Point(-0.02, -0.02, 0),
				ut_Point(0.02, -0.02, 0),
				ut_Point());

	glutSwapBuffers();
}

void on_reshape(int w, int h) {
	if (h == 0)
		h = 1;

	window_width=w;
	window_height=h;


	glutWarpPointer(window_width/2,window_height/2);

	float ratio = 1.0 * w / h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(40, ratio, 1, 400);



	get_vectors();

	/*
	Based off http://bookofhook.com/mousepick.pdf
	OpenGL matrix order
	m0 m4 m8 m12
	m1 m5 m9 m13
	m2 m6 m10 m14
	m3 m7 m11 m15
	Projection matrix
	a 0 0 0
	0 b 0 0
	0 0 c d
	0 0 e 0
	Inverse projection
	1/a, 0, 0, 0,
	0, 1/b, 0, 0,
	0, 0, 0, 1/e,
	0, 0, 1/d, -c/(d*e)
	*/
	double op[16]; // original projection matrix
	glGetDoublev(GL_PROJECTION_MATRIX, op);

	double a = op[0];
	double b = op[5];
	double c = op[10];
	double d = op[14];
	double e = op[11];

	projection_inverse[0] = 1.0 / a;
	projection_inverse[5] = 1.0 / b;
	projection_inverse[11] = 1.0 / d;
	projection_inverse[14] = 1.0 / e;
	projection_inverse[15] = -c / (d * e);
}


void create_bricks() {
	/* TODO  destructor will not be called for this because of loop */
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
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixd(projection_inverse);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor3f(0.6,0.4,0.8);
	ut_draw_triangle(A, B, C);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}
