#include "globalVariables.h"
#include "keyboard.h"
#include "vector3f.h"
#include "animate.h"


void buttonChangeState(int key) {
	if (key == bLeft.key) { 
		bLeft.changeState();
		return;
	}

	if (key == bRight.key) { 
		bRight.changeState();
		return;
	}

	if (key == bUp.key) { 
		bUp.changeState();
		return;
	}

	if (key == bDown.key) { 
		bDown.changeState();
		return;
	}

	if (key == bBackward.key) { 
		bBackward.changeState();
		return;
	}

	if (key == bForward.key) { 
		bForward.changeState();
		return;
	}
}

bool somethingIsPressed() {
	return bLeft.pressed || bRight.pressed || bDown.pressed || bUp.pressed || bForward.pressed || bBackward.pressed;
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'j')
		phiStep=0.02;
	if (key == 'l')
		phiStep=-0.02;
	if (key == 'i')
		thetaStep=-0.02;
	if (key=='k')
		thetaStep=0.02;

	if (key == bSelectDeselect.key) {
	  if (space.selected!=-1) {
		space.cuboids[space.selected].pos.z -= 0.2;
		space.cuboids[space.selected].round();
		space.deselect();
		space.selected = -1;
		glutPostRedisplay();
	  }
	}



	buttonChangeState(key);
	 if (!animation_ongoing) {
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            animation_ongoing = 1;
     }
}

void keyBoardSpecialUp( int key, int x, int y ) { 
	buttonChangeState(key);
	if (!somethingIsPressed())
		animation_ongoing=0;
	if (key == GLUT_KEY_UP) {
		moveFront = 0;
	
	}
	if (key == GLUT_KEY_DOWN) {
		moveFront = 0;
	}

	if (key == GLUT_KEY_LEFT) {
		moveLeftRight = 0;
	}
	if (key == GLUT_KEY_RIGHT) {
		moveLeftRight = 0;
	}

}

void keyBoardUp(unsigned char key, int x, int y) {
	if (key == 'j' || key=='l')
		phiStep=0;
	if (key == 'i' || key=='k')
		thetaStep=0;

	buttonChangeState(key);
	if (!somethingIsPressed())
		animation_ongoing=0;
}

void keyboardSpecial(int key, int x, int y) {
	buttonChangeState(key);

	if (key == GLUT_KEY_UP) {
		moveFront = 0.5;
	}
	if (key == GLUT_KEY_DOWN) {
		moveFront = -0.5;
	}
	if (key == GLUT_KEY_LEFT) {
		moveLeftRight = 0.5;
	}
	if (key == GLUT_KEY_RIGHT) {
		moveLeftRight = -0.5;
	}
}


