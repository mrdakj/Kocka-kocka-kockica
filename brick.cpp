#include "headers/brick.h"
#include <GL/glut.h>
#include "headers/loadModel.h"
#include "headers/utility.h"

/* constuctors */

Brick::Brick() : Brick(Vector3f(0,0,0), Size(1,1,1)) {}

Brick::Brick(Vector3f pos, Size size) : pos(pos), size(size) {
	in_car=false;
}

Brick::Brick(Vector3f pos, Size size, Color color) : Brick(pos, size) { 
	this->color = color;
}

/* end constuctors */


void Brick::draw_cuboid() const {
	int w = size.width;
	int h = size.height;
	int d = size.depth;

	glPushMatrix();
	/* draw a cuboid smaller because of flickering when camera is inside it */
	float eps = 0.001;
	glTranslatef(eps/2, eps/2, -eps/2);
	/* call from utility */
	ut_draw_cuboid(w-eps, d-eps, h-eps);
	glPopMatrix();
}

void Brick::draw_cylinder() const {
	for (int i = 0; i < size.width; i++) {
		for (int j = 0; j < size.depth; j++) {
			glPushMatrix();
			glTranslatef(0.5 + i, size.height+0.1, -0.5-j);
			renderModel();
			glPopMatrix();
		}
	}
}


void Brick::draw_transparent_brick() const {
	
	glEnable(GL_CULL_FACE);

	glCullFace(GL_FRONT);
	draw_normal_brick();

	glCullFace(GL_BACK);
	draw_normal_brick();

	glDisable(GL_CULL_FACE);
}

void Brick::draw_normal_brick() const {

	glPushMatrix();
	glTranslatef(pos.x, pos.z, -pos.y);
	draw_cuboid();
	draw_cylinder();
	glPopMatrix();

}

void Brick::draw_brick() const {

	glColor4f(color.r, color.g, color.b, color.a);

	if (color.a != 1)
		draw_transparent_brick();
	else
		draw_normal_brick();
}

void Brick::round() {
	pos.x = (int)(pos.x+0.5);
	pos.y = (int)(pos.y+0.5);
	pos.z = (int)(pos.z+0.5);
}
