#include <GL/glut.h>
#include "headers/brick.h"
#include "headers/loadModel.h"
#include "headers/utility.h"

/* constuctors */

Brick::Brick() : Brick(Vector3f(), Size()) {}

Brick::Brick(Vector3f pos, Size size) : pos(pos), size(size) {}

Brick::Brick(Vector3f pos, Size size, Color color) : Brick(pos, size) { 
	this->color = color;
}

/* end of constuctors */


void Brick::draw_cuboid() const {
	float eps = 0.001;

	glPushMatrix();
	/* draw a cuboid smaller because of flickering when camera is inside it */
	glTranslatef(eps/2, eps/2, -eps/2);
	ut_draw_cuboid(size.width - eps, size.depth - eps, size.height - eps);
	glPopMatrix();
}

void Brick::draw_cylinder() const {

	for (int i = 0; i < size.width; i++) {
		for (int j = 0; j < size.depth; j++) {
			glPushMatrix();
			glTranslatef(0.5 + i, size.height + 0.1, -0.5 - j);
			renderModel();
			glPopMatrix();
		}
	}
}


void Brick::draw_transparent_brick() const {
	
	glEnable(GL_CULL_FACE);

	/* draw a brick inside */
	glCullFace(GL_FRONT);
	draw_normal_brick();

	/* draw a brick outside */
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
	pos = (pos + 0.5).coordinates_to_integer();
}
