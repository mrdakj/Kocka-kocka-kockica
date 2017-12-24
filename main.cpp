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

// define global variables
float translate_x=0.01;
bool go = false;
int windowWidth=1200;
int windowHeight=700;
float speed = 0.08;
Space space;
Button bUp('e');
Button bDown('q');
Button bLeft('a');
Button bRight('d');
Button bForward('s');
Button bBackward('w');
Button bSelectDeselect(32);
Vector3f cameraPosition(0,3,4);
Vector3f to(0,0,-1);
Vector3f view, hvector, v;
float fovy = 40;
int nearClippingPlaneDistance=1;
float moveFront = 0;
float moveLeftRight = 0;
float moveUpDown=0;
float theta=-3.141592/2;
float phi=3.141592/2;
float thetaStep=0;
float phiStep=0;

GLdouble objX=0;
GLdouble objY=0;
GLdouble objZ=0;
//--------------------------------

void setWindow();
void renderScene(void);
void changeScene(int w, int h);
int animation_ongoing;
void getVectors();



int main(int argc, char** argv) {
	animation_ongoing=0;

	/* init glut */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	setWindow();


	GLfloat blue[] = {0.0, 0.0, 1, 1};
	GLfloat green[] = {0.0, 1.0, 0.0, 1};
	GLfloat red[] = {1, 0.24, 0.14, 1};

	/* TODO  destructor will not be called for this because of loop */
    Brick c(Position(4,5,0), Size(2,1,2), Color(1,0,0));
    Brick c3(Position(0,0,0), Size(1,2,1), Color(0,1,0));
    Brick cx(Position(0,5,0), Size(2,1,1), Color(0,0,1));
    Brick c2(Position(5,0,0), Size(4,1,2), Color(1,0,0));

	space.add(cx);
	space.add(c3);
	space.add(c);
	space.add(c2);

	glEnable(GL_DEPTH_TEST);

	/* Light and material */
	GLfloat light_ambient[]={0.1, 0.1, 0.1 ,1};
	GLfloat light_diffuse[]={1, 1, 1, 1};
	GLfloat light_specular[]={0.3, 0.3, 0.3, 1};
	GLfloat linear_attenuation = 0.005;

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	/* glEnable(GL_COLOR_MATERIAL); */
	/* glEnable(GL_NORMALIZE); */

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linear_attenuation);
	/* glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15.0); */


	glClearColor(0.16,0.16,0.16,1);
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



double inverseProject[16]={0.0};

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


	if (space.selected_brick != -1) {
		Brick& selected_brickBrick = space.bricks[space.selected_brick];
		if (bUp.pressed) {
			if (move(Up,selected_brickBrick,speed)) {
				to.x = objX-cameraPosition.x;
				to.z = objZ-cameraPosition.z;
				to.y = objY-cameraPosition.y;
				float modultheta = sqrt(to.x*to.x+to.z*to.z+to.y*to.y);
				/* for polar coordinates */
				/* float modultheta = sqrt(to.x*to.x+to.z*to.z); */

				to.x /= modultheta;
				to.z /= modultheta;
				to.y /= modultheta;
			}
		}

		if (bDown.pressed) {
			if (move(Down,selected_brickBrick,speed)) {
				to.x = objX-cameraPosition.x;
				to.z = objZ-cameraPosition.z;
				to.y = objY-cameraPosition.y;
				float modultheta = sqrt(to.x*to.x+to.z*to.z+to.y*to.y);
				/* for polar coordinates */
				/* float modultheta = sqrt(to.x*to.x+to.z*to.z); */

				to.x /= modultheta;
				to.z /= modultheta;
				to.y /= modultheta;
			}
		}
	}


	glLoadIdentity();

	/* following camera because it's before lookat*/
	GLfloat light_position[] = {0,0,0,1};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	/* GLfloat spot_direction[]={0,0,-1}; */
	/* glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction); */

	gluLookAt(cameraPosition.x,cameraPosition.y,cameraPosition.z, cameraPosition.x + to.x,cameraPosition.y + to.y,cameraPosition.z + to.z, 0, 1, 0);


	glTranslatef(-translate_x,0,0);

	if (go) {
		space.car.wheel_rotation_angle+=1;
		translate_x += 0.02;
		space.draw_car();
	}
	else {
		space.car.wheel_rotation_angle=0;
		translate_x = 0;
		space.render();
	}



	double modelMatrix[16];
	double projMatrix[16];
	GLint viewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
	glGetIntegerv(GL_VIEWPORT,viewport); 

	GLdouble winX;
	GLdouble winY;
	GLdouble winZ;

	if (space.selected_brick == -1) {
		glPushMatrix();
			glLoadIdentity();
			glMultMatrixd(inverseProject);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);
			glBegin(GL_TRIANGLES);
				glColor3f(0.6,0.4,0.8);
				glVertex3f(-0.02, -0.02, 0);
				glVertex3f(0.02, -0.02, 0);
				glVertex3f(0, 0, 0);
			glEnd();
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);
		glPopMatrix();
	} else {
		gluProject(objX,objY,objZ,modelMatrix,projMatrix,viewport,&winX, &winY, &winZ);
			winY = winY-windowHeight/2;
			winX = winX - windowWidth/2;
			winX  /= windowWidth/2;
			winY /= windowHeight/2;

		glPushMatrix();
			glLoadIdentity();
			glMultMatrixd(inverseProject);
		
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);
			glBegin(GL_TRIANGLES);
				glColor3f(0.6,0.4,0.8);
				glVertex3f(winX-0.02, winY-0.02, 0);
				glVertex3f(winX+0.02, winY-0.02, 0);
				glVertex3f(winX, winY, 0);
			glEnd();
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);

		glPopMatrix();
	} 

	glutSwapBuffers();
}

void changeScene(int w, int h) {
	if (h == 0)
		h = 1;

	windowWidth=w;
	windowHeight=h;


	glutWarpPointer(windowWidth/2,windowHeight/2);

	float ratio = 1.0 * w / h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(fovy, ratio, nearClippingPlaneDistance, 400);

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


