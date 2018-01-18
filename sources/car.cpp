#include "../headers/car.h"
#include "../headers/utility.h"
#include "../headers/models.h"

Car::Car(int position_x, int position_y, int width, int depth) :
	position_x(position_x), position_y(position_y),
	width(width), depth(depth), is_going(false),
	wheel_rotation_angle(0), translation_x(0) {}


float Car::get_position() const
{
	return -translation_x;
}

void Car::draw_base() const {

	glPushMatrix();

	glTranslatef(-translation_x,0,0);

	draw_brick();

	/* wheel 1 */
	glPushMatrix();
	glTranslatef(position_x,0,+0.5-position_y);
	glRotatef(wheel_rotation_angle, 0,0,1);
	wheel.render();
	glPopMatrix();

	/* wheel 2 */
	glPushMatrix();
	glTranslatef(position_x+width,0,+0.5-position_y);
	glRotatef(wheel_rotation_angle, 0,0,1);
	wheel.render();
	glPopMatrix();

	/* wheel 3 */
	glPushMatrix();
	glTranslatef(position_x+width,0,-0.5-position_y-depth);
	glRotatef(wheel_rotation_angle, 0,0,1);
	glRotatef(180,1,0,0);
	wheel.render();
	glPopMatrix();

	/* wheel 4 */
	glPushMatrix();
	glTranslatef(position_x,0,-0.5-position_y-depth);
	glRotatef(wheel_rotation_angle, 0,0,1);
	glRotatef(180,1,0,0);
	wheel.render();
	glPopMatrix();

	glPopMatrix();
}


void Car::draw_cylinder() const
{
	int w = width;
	float h = 0.2;
	int d = depth;

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < d; j++) {
			glPushMatrix();
			glTranslatef(0.5 + i, h+0.1, -0.5-j);
			cylinder.render();
			glPopMatrix();
		}
	}
}

void Car::draw_brick() const
{
	glPushMatrix();
	glTranslatef(position_x,-0.2,-position_y);

	glColor3f(0, 1, 0);

	ut::draw_cuboid(width, depth, 0.2);
	draw_cylinder();

	glPopMatrix();
}

void Car::stop()
{
	is_going = false;
	wheel_rotation_angle = 0;
	translation_x = 0;
}

void Car::go()
{
	is_going = true;
	wheel_rotation_angle += 1;
	translation_x += 0.02;
}

float Car::wheel_left(Wheel w) const
{
	switch (w) {
		case Wheel_1:
		case Wheel_4:
			return position_x - 1;
			break;
		case Wheel_2:
		case Wheel_3:
			return position_x + width - 1;
			break;
	}

	return -1;
}

float Car::wheel_right(Wheel w) const
{
	switch (w) {
		case Wheel_1:
		case Wheel_4:
			return position_x + 1;
			break;
		case Wheel_2:
		case Wheel_3:
			return position_x + width + 1;
			break;
	}

	return -1;

}

float Car::wheel_front(Wheel w) const
{
	switch (w) {
		case Wheel_1:
		case Wheel_2:
			return position_y - 0.8;
			break;
		case Wheel_3:
		case Wheel_4:
			return position_y + depth + 0.2;
			break;
	}

	return -1;

}

float Car::wheel_back(Wheel w) const
{
	switch (w) {
		case Wheel_1:
		case Wheel_2:
			return position_y - 0.2;
			break;
		case Wheel_3:
		case Wheel_4:
			return position_y + depth + 0.8;
			break;
	}
	return -1;

}

float Car::center_x(Wheel w) const
{
	switch (w) {
		case Wheel_1:
		case Wheel_4:
			return position_x;
			break;
		case Wheel_2:
		case Wheel_3:
			return position_x + width;
			break;
	}

	return -1;

}

int Car::wheels_top() const
{
	return 1;
}
