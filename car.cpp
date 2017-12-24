#include "globalVariables.h"
#include "loadModel.h"
#include "car.h"
#include "utility.h"

Car::Car() : Car(5, 10, 12, 8) {}
Car::Car(int position_x, int position_y, int width, int depth) : position_x(position_x), position_y(position_y), width(width), depth(depth), wheel_rotation_angle(0) {}


void Car::draw_base() {

	draw_brick();

	/* wheel 1 */
	glPushMatrix();
	glTranslatef(position_x,0,-0.5-position_y);
	glRotatef(wheel_rotation_angle, 0,0,1);
	renderWheel();
	glPopMatrix();

	/* wheel 2 */
	glPushMatrix();
	glTranslatef(position_x+width,0,-0.5-position_y);
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
}


void Car::draw_cylinder() {
	int w = width;
	float h = 0.2;
	int d = depth-1;
	
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < d; j++) {
			glPushMatrix();
			glTranslatef(0.5 + i, h+0.1, -0.5-j);
			renderModel();
			glPopMatrix();
		}
	}
}

void Car::draw_brick() {
	glPushMatrix();
	glTranslatef(position_x,-0.2,-1-position_y);

	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0, 1, 0);

	draw_cuboid_u(width, depth-1, 0.2);
	draw_cylinder();

	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
}
