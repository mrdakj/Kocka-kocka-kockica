#include "headers/loadModel.h"
#include "headers/car.h"
#include "headers/utility.h"

Car::Car() : Car(3, 9, 12, 6) {}
Car::Car(int position_x, int position_y, int width, int depth) :
	position_x(position_x), position_y(position_y),
	width(width), depth(depth), is_going(false),
	wheel_rotation_angle(0), translation_x(0) {}


float Car::get_position() {
	return -translation_x;
}

void Car::draw_base() const {

	glPushMatrix();

	glTranslatef(-translation_x,0,0);

	draw_brick();

	glDisable(GL_COLOR_MATERIAL);
	/* wheel 1 */
	glPushMatrix();
	glTranslatef(position_x,0,+0.5-position_y);
	glRotatef(wheel_rotation_angle, 0,0,1);
	renderWheel();
	glPopMatrix();

	/* wheel 2 */
	glPushMatrix();
	glTranslatef(position_x+width,0,+0.5-position_y);
	glRotatef(wheel_rotation_angle, 0,0,1);
	renderWheel();
	glPopMatrix();

	/* wheel 3 */
	glPushMatrix();
	glTranslatef(position_x+width,0,-0.5-position_y-depth);
	glRotatef(wheel_rotation_angle, 0,0,1);
	glRotatef(180,1,0,0);
	renderWheel();
	glPopMatrix();

	/* wheel 4 */
	glPushMatrix();
	glTranslatef(position_x,0,-0.5-position_y-depth);
	glRotatef(wheel_rotation_angle, 0,0,1);
	glRotatef(180,1,0,0);
	renderWheel();
	glPopMatrix();

	glPopMatrix();

	glEnable(GL_COLOR_MATERIAL);

}


void Car::draw_cylinder() const {
	int w = width;
	float h = 0.2;
	int d = depth;
	
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < d; j++) {
			glPushMatrix();
			glTranslatef(0.5 + i, h+0.1, -0.5-j);
			renderModel();
			glPopMatrix();
		}
	}
}

void Car::draw_brick() const {
	glPushMatrix();
	glTranslatef(position_x,-0.2,-position_y);

	glColor3f(0, 1, 0);

	ut_draw_cuboid(width, depth, 0.2);
	draw_cylinder();

	glPopMatrix();
}

void Car::stop() {
	is_going = false;
	wheel_rotation_angle = 0;
	translation_x = 0;
}

void Car::go() {
	is_going = true;
	wheel_rotation_angle+=1;
	translation_x += 0.02;
}
