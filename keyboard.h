#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <GL/glut.h>
#include <cmath>

struct Button {
	const int key;
	bool pressed;

	Button(int key) : key(key), pressed(false) {}
	void changeState() { pressed = !pressed; }
};

void keyboard(unsigned char key, int x, int y);
void keyboardSpecial(int key, int x, int y);
void keyBoardSpecialUp( int key, int x, int y );
void keyBoardUp(unsigned char key, int x,int y);
bool somethingIsPressed();
void mouse(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void passiveMouse(int x, int y);

#endif
