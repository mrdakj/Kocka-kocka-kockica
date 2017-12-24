#include "utility.h"

void ut_draw_line(const ut_Point& A,const ut_Point& B) {
	glBegin(GL_LINES);
	glVertex3f(A.x, A.y, A.z);
	glVertex3f(B.x, B.y, B.z);
	glEnd();
}

void ut_draw_cuboid(float width, float depth, float height) {
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

int ut_index_of_minimum(const float& a, const float& b, const float& c) {
	if (a <= b) {
		if (a <= c) return 0;
		return 2;
	}

	if (b <= c) return 1;
	return 2;
}
