#include <GL/glut.h>
#include <cmath>
#include <stdio.h>
#include "vector3f.h"

#include "globalVariables.h"
#include "animate.h"
#include "space.h" 
#include "keyboard.h"
#include "mouse.h"


// define global variables
int windowWidth=1200;
int windowHeight=700;
float speed = 0.08;
int xcursor=0;
int ycursor=0;
Space space;
Button bUp('e');
Button bDown('q');
Button bLeft('a');
Button bRight('d');
Button bForward('s');
Button bBackward('w');
Button bSelectDeselect(32);
Vector3f cameraPosition(0,3,5);
Vector3f to(0,0,-1);
Vector3f view, hvector, v;
float fovy = 40;
int nearClippingPlaneDistance=1;
float moveFront = 0;
float moveLeftRight = 0;
float moveUpDown=0;
float theta=0;
float phi=0;
float thetaStep=0;
float phiStep=0;
int delta_x=0;
int delta_y=0;

//--------------------------------

void setWindow();
void renderScene(void);
void changeScene(int w, int h);
int animation_ongoing;
void getVectors();

int main(int argc, char** argv) {
	animation_ongoing=0;



	/* TODO  destructor will not be called for this because of loop */
    Cuboid cx(Position(0,5,0), Size(2,1,1), Color(0,0,1));
    Cuboid c(Position(4,5,0), Size(2,1,2), Color(0,1,0));
    Cuboid c3(Position(6,14,2), Size(1,1,1), Color(1,0,0));

	space.add(cx);
	space.add(c3);
	space.add(c);


	/* init glut */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	setWindow();

	glEnable(GL_DEPTH_TEST);

	glutSetCursor(GLUT_CURSOR_NONE);
	/* register callbacks */
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeScene);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutSpecialUpFunc(keyBoardSpecialUp);
	glutKeyboardUpFunc(keyBoardUp);
	glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(passiveMouse);
	glutIdleFunc(renderScene);

	glutMainLoop();
	
	return 0;
}

void setWindow() {
	glutInitWindowPosition(0,0);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Kocka kocka kockica");
}


void getFrontPosition() {
	cameraPosition.x += moveFront * to.x * 0.2;
	cameraPosition.z += moveFront * to.z * 0.2;
}

void getLeftRightPosition() {
	cameraPosition.x += moveLeftRight * to.z * 0.2;
	cameraPosition.z += moveLeftRight * -to.x * 0.2;
}

void getUpDownPosition() {

	cameraPosition.y += moveUpDown;
}

void calculateDirection() {
	theta += thetaStep;
	phi += phiStep;

	if (phi >= 0.9) {
		phi = 0.9;
	} else if (phi <= -0.9) {
		phi = -0.9;
	}

	to.x = std::sin(theta);
	to.y = std::sin(phi);
	to.z = -std::cos(theta);

	getVectors();
}

double inverseProject[16]={0.0};

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glPushMatrix();
		glLoadIdentity();
		glMultMatrixd(inverseProject);

		glBegin(GL_TRIANGLES);
			glVertex3f(-0.02, -0.02, 0);
			glVertex3f(0.02, -0.02, 0);
			glVertex3f(0, 0, 0);
		glEnd();

	glPopMatrix();


	if (moveUpDown)
		getUpDownPosition();

	if (moveFront)
		getFrontPosition();

	if (moveLeftRight)
		getLeftRightPosition();

	if (phiStep) {
		calculateDirection();
	}

	if (thetaStep) {
		calculateDirection();
	}


	glLoadIdentity();
	gluLookAt(cameraPosition.x,cameraPosition.y,cameraPosition.z, cameraPosition.x + to.x,cameraPosition.y + to.y,cameraPosition.z + to.z, 0, 1, 0);

	space.render();


	glutSwapBuffers();
}

void changeScene(int w, int h) {
	if (h == 0)
		h = 1;

	windowWidth=w;
	windowHeight=h;


	glutWarpPointer(windowWidth/2,windowHeight/2);
	delta_x=windowWidth/2;
	delta_y=windowHeight/2;

	float ratio = 1.0 * w / h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(fovy, ratio, nearClippingPlaneDistance, 40);

	glMatrixMode(GL_MODELVIEW);

	getVectors();

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

	inverseProject[0] = 1.0 / a;
	inverseProject[5] = 1.0 / b;
	inverseProject[11] = 1.0 / d;
	inverseProject[14] = 1.0 / e;
	inverseProject[15] = -c / (d * e);
}


void getVectors() {
	view = to;
	view.normalize();
	hvector = view * Vector3f(0,1,0);
	hvector.normalize();
	v=hvector*view;
	v.normalize();
	float rad = fovy * 3.14159 / 180;
	float vLength = tan( rad / 2 ) * nearClippingPlaneDistance;
	float hLength = vLength * ((float)windowWidth / windowHeight);
	v = v*vLength;
	hvector = hvector*hLength;
}

void passiveMouse(int x, int y) {
	if (x>=windowWidth-10 || x<=10) {
		glutWarpPointer(windowWidth/2,y);
		delta_x=windowWidth/2;
	}
	if (y>=windowHeight-10 || y<=10) {
		glutWarpPointer(x,windowHeight/2);
		delta_y=windowHeight/2;
	}

	y = windowHeight - y;

	/* if (x-delta_x>0) */
	/* 	thetaStep =  0.003; */
	/* else if (x-delta_x<0) */
	/* 	thetaStep=-0.003; */

	/* if (y-delta_y>0) */
	/* 	phiStep =0.003; */
	/* else if (y-delta_y<0) */
	/* 	phiStep=-0.003; */
	thetaStep = (x-delta_x)*0.002;
	if (abs(x-delta_x)>50) {
		glutWarpPointer(windowWidth/2,y);
		delta_x=windowWidth/2;
		thetaStep=0;
	}

	phiStep = (y - delta_y) * 0.002;
	if (abs(y-delta_y)>50)
	{
		glutWarpPointer(x,windowHeight/2);
		delta_y=windowHeight/2;
		phiStep=0;
	}

	if (thetaStep) {
		calculateDirection();
	}

	if (phiStep) {
		calculateDirection();
	}

	delta_x = x;
	delta_y = y;

	thetaStep = phiStep = 0;
}
