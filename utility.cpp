#include "utility.h"

void ut_draw_line(const ut_Point& A,const ut_Point& B) {
	glBegin(GL_LINES);
	glVertex3f(A.x, A.y, A.z);
	glVertex3f(B.x, B.y, B.z);
	glEnd();
}

void ut_draw_triangle(const ut_Point& A, const ut_Point& B, const ut_Point& C) {
	glBegin(GL_TRIANGLES);
	glVertex3f(A.x, A.y, A.z);
	glVertex3f(B.x, B.y, B.z);
	glVertex3f(C.x, C.y, C.z);
	glEnd();
}

ut_Point ut_world_to_screen_coordinates(const ut_Point& world_point) {
	GLdouble modelview_matrix[16];
	GLdouble projection_matrix[16];
	GLint viewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX,modelview_matrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projection_matrix);
	glGetIntegerv(GL_VIEWPORT,viewport); 

	int window_width = viewport[2];
	int window_height = viewport[3];

	ut_Point screen_point;

	gluProject(world_point.x,world_point.y,world_point.z,modelview_matrix,projection_matrix,viewport,&screen_point.x, &screen_point.y, &screen_point.z);

	screen_point.y = screen_point.y-window_height/2;
	screen_point.x = screen_point.x - window_width/2;
	screen_point.x  /= window_width/2;
	screen_point.y /= window_height/2;

	return screen_point;
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
