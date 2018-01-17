#include <GL/glut.h>
#include <cmath>
#include "../headers/brick.h"
#include "../headers/utility.h"
#include "../headers/models.h"

#define eps 0.001

Brick::Brick(Color color) : color(color) {}
Brick::Brick(Size size, Color color) : size(size), color(color)  {}
Brick::Brick(ut::Vector3f pos, Size size, Color color) : pos(pos), size(size), color(color) {}

ut::Vector3f Brick::get_world_coordinates() const
{
	return ut::Vector3f(pos.x, pos.z, -pos.y);
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

void Brick::draw_cuboid() const
{
	glPushMatrix();
	/* draw a cuboid smaller because of flickering when camera is inside it */
	glTranslatef(eps/2, eps/2, -eps/2);
	ut::draw_cuboid(size.width - eps, size.depth - eps, size.height - eps);
	glPopMatrix();
}

void Brick::draw_cylinder() const
{
	for (int i = 0; i < size.width; i++) {
		for (int j = 0; j < size.depth; j++) {
			glPushMatrix();
			glTranslatef(0.5 + i, size.height + 0.1, -0.5 - j);
			cylinder.render();
			glPopMatrix();
		}
	}
}


void Brick::draw_transparent_brick() const
{
	glEnable(GL_CULL_FACE);

	/* draw a brick inside */
	glCullFace(GL_FRONT);
	draw_normal_brick();

	/* draw a brick outside */
	glCullFace(GL_BACK);
	draw_normal_brick();

	glDisable(GL_CULL_FACE);
}

void Brick::draw_normal_brick() const
{
	glPushMatrix();
	glTranslatef(pos.x, pos.z, -pos.y);
	draw_cuboid();
	draw_cylinder();
	glPopMatrix();
}

void Brick::draw_brick() const
{
	glColor4f(color.r, color.g, color.b, color.a);

	if (color.a != 1)
		draw_transparent_brick();
	else
		draw_normal_brick();
}

void Brick::round()
{
	pos = (pos + 0.5).coordinates_to_integer();
}


void Brick::move(Direction d, float delta)
{
	switch (d) {
		case Left_direction:
			pos.x -= delta;
			break;
		case Right_direction:
			pos.x += delta;
			break;
		case Forward_direction:
			pos.y -= delta;
			break;
		case Backward_direction:
			pos.y += delta;
			break;
		case Up_direction:
			pos.z += delta;
			break;
		case Down_direction:
			pos.z -= delta;
			break;
	}
}

void Brick::move_to_position(Direction d, float position)
{
	switch (d) {
		case Left_direction:
		case Right_direction:
			pos.x = position;
			break;
		case Forward_direction:
		case Backward_direction:
			pos.y = position;
			break;
		case Up_direction:
		case Down_direction:
			pos.z = position;
			break;
	}
}



float Brick::get_distance(Direction d) const
{
	switch (d) {
		case Left_direction:
			return std::fabs(std::floor(pos.x)-pos.x);
			break;
		case Right_direction:
			return std::fabs(std::ceil(pos.x)-pos.x);
			break;
		case Forward_direction:
			return std::fabs(std::floor(pos.y)-pos.y);
			break;
		case Backward_direction:
			return std::fabs(std::ceil(pos.y)-pos.y);
			break;
		case Up_direction:
			return std::fabs(std::ceil(pos.z)-pos.z);
			break;
		case Down_direction:
			return std::fabs(std::floor(pos.z)-pos.z);
			break;
	}

	return -1;
}
