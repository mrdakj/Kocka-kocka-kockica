#include <GL/glut.h>
#include "headers/brick.h"
#include "headers/loadModel.h"
#include "headers/utility.h"
#include <cmath>

/* constuctors */

Brick::Brick() : Brick(Vector3f(), Size()) {}

Brick::Brick(Vector3f pos, Size size) : pos(pos), size(size) {}

Brick::Brick(Vector3f pos, Size size, Color color) : Brick(pos, size) { 
	this->color = color;
}

/* end of constuctors */


Vector3f Brick::get_world_coordinates() const
{
	return Vector3f(pos.x, pos.z, -pos.y);
}


float Brick::cylinder_front() const
{
	return pos.y + 0.2;
}

float Brick::cylinder_back() const
{
	return pos.y + size.depth - 0.2;
}

float Brick::cylinder_left() const
{
	return pos.x + 0.2;
}

float Brick::cylinder_right() const
{
	return pos.x + size.width - 0.2;
}

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


void Brick::move(Direction d, float delta)
{
	switch (d) {
		case Left:
			pos.x -= delta;
			break;
		case Right:
			pos.x += delta;
			break;
		case Forward:
			pos.y -= delta;
			break;
		case Backward:
			pos.y += delta;
			break;
		case Up:
			pos.z += delta;
			break;
		case Down:
			pos.z -= delta;
			break;
	}
}

void Brick::move_to_position(Direction d, float position)
{
	switch (d) {
		case Left:
		case Right:
			pos.x = position;
			break;
		case Forward:
		case Backward:
			pos.y = position;
			break;
		case Up:
		case Down:
			pos.z = position;
			break;
	}
}



float Brick::get_distance(Direction d) const
{
	switch (d) {
		case Left:
			return std::fabs(std::floor(pos.x)-pos.x);
			break;
		case Right:
			return std::fabs(std::ceil(pos.x)-pos.x);
			break;
		case Forward:
			return std::fabs(std::floor(pos.y)-pos.y);
			break;
		case Backward:
			return std::fabs(std::ceil(pos.y)-pos.y);
			break;
		case Up:
			return std::fabs(std::ceil(pos.z)-pos.z);
			break;
		case Down:
			return std::fabs(std::floor(pos.z)-pos.z);
			break;
	}

	return -1;
}
