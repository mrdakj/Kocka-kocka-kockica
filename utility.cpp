#include "utility.h"

void draw_cuboid_u(float width, float depth, float height) {
	glBegin(GL_QUADS);

    //Front
	glNormal3f(0,0,1);
    glVertex3f(0, 0, 0);
    glVertex3f(width, 0, 0);
    glVertex3f(width, height, 0);
    glVertex3f(0, height, 0);

    //Right
	glNormal3f(1,0,0);
    glVertex3f(width, 0, 0);
    glVertex3f(width, 0, -depth);
    glVertex3f(width, height, -depth);
    glVertex3f(width, height, 0);

    //Back
	glNormal3f(0,0,-1);
    glVertex3f(width, 0, -depth);
    glVertex3f(width, height, -depth);
    glVertex3f(0, height, -depth);
    glVertex3f(0, 0, -depth);

    //Left
	glNormal3f(-1,0,0);
    glVertex3f(0, 0, -depth);
    glVertex3f(0, height, -depth);
    glVertex3f(0, height, 0);
    glVertex3f(0, 0, 0);

	//Top
	glNormal3f(0,1,0);
    glVertex3f(0, height, 0);
    glVertex3f(width, height, 0);
    glVertex3f(width, height, -depth);
    glVertex3f(0, height, -depth);

	//Bottom
	glNormal3f(0,-1,0);
    glVertex3f(0, 0, 0);
    glVertex3f(width, 0, 0);
    glVertex3f(width, 0, -depth);
    glVertex3f(0, 0, -depth);

    glEnd();
}

int getIndexOfMinimum(const float& a, const float& b, const float& c) {
	if (a <= b) {
		if (a <= c)
			return 0;
		return 2;
	}

	if (b <= c)
		return 1;
	return 2;
}
