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
float theta=3.1415/2;
float phi=3.1415/2;
float thetaStep=0;
float phiStep=0;

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
	phi += phiStep;
	theta += thetaStep;

	to.x=std::sin(theta)*std::cos(phi);
	to.z=-std::sin(phi)*std::sin(theta);
	to.y=std::cos(theta);

	getVectors();
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	float ratio = 1.0 * w / h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(fovy, ratio, nearClippingPlaneDistance, 40);


	glMatrixMode(GL_MODELVIEW);

	getVectors();

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
