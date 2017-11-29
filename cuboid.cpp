#include  "cuboid.h"
#include <stdio.h>

Cuboid::Cuboid() {
	/* printf("const %f\n", pos.x); */
	obj = gluNewQuadric();
}

Cuboid::Cuboid(Position pos, Size size) : pos(pos), size(size) {
	/* printf("const %f\n",pos.x); */
	obj = gluNewQuadric();
}

Cuboid::Cuboid(Position pos, Size size, Color color) : Cuboid(pos, size) { 
	this->color = color;
}

Cuboid::Cuboid(Cuboid const& other) {
	pos=other.pos;
	size=other.size;
	color=other.color;
	obj=gluNewQuadric();
	/* printf("copy const %f\n", pos.x); */
}

Cuboid::~Cuboid(void) {
	/* printf("dest %f\n",pos.x); */
	gluDeleteQuadric(obj);
}

void Cuboid::renderCuboid() {
	int w = size.width;
	int h = size.height;
	int d = size.depth;

	glBegin(GL_QUADS);

    //Front
    glVertex3f(0, 0, 0);
    glVertex3f(w, 0, 0);
    glVertex3f(w, h, 0);
    glVertex3f(0, h, 0);

    //Right
    glVertex3f(w, 0, 0);
    glVertex3f(w, 0, -d);
    glVertex3f(w, h, -d);
    glVertex3f(w, h, 0);

    //Back
    glVertex3f(w, 0, -d);
    glVertex3f(w, h, -d);
    glVertex3f(0, h, -d);
    glVertex3f(0, 0, -d);

    //Left
    glVertex3f(0, 0, -d);
    glVertex3f(0, h, -d);
    glVertex3f(0, h, 0);
    glVertex3f(0, 0, 0);

	//Top
    glVertex3f(0, h, 0);
    glVertex3f(w, h, 0);
    glVertex3f(w, h, -d);
    glVertex3f(0, h, -d);

	//Bottom
    glVertex3f(0, 0, 0);
    glVertex3f(w, 0, 0);
    glVertex3f(w, 0, -d);
    glVertex3f(0, 0, -d);

    glEnd();
}

void Cuboid::renderCylinder() {
	glColor3f(0,1,1);
	for (int i = 0; i < size.width; i++) {
		for (int j = 0; j < size.depth; j++) {
			glPushMatrix();
			glTranslatef(0.5 + i, size.height, -0.5-j);
			glRotatef(-90,1,0,0);
			glBegin(GL_POLYGON);
			gluCylinder(obj, 0.25, 0.25, 0.2, 30,30);
			glPopMatrix();
		}
	}
}

void Cuboid::render() {
	glColor3f(color.r, color.g, color.b);
	
	glPushMatrix();
	glTranslatef(pos.x, pos.z, -pos.y);
	renderCuboid();
	renderCylinder();
	glPopMatrix();
}

void Cuboid::move(Direction d) {
	switch(d) {
		case Left:
				pos.x--;
				break;
		case Right:
				pos.x++;
				break;
		case Up:
				pos.z++;
				break;
		case Down:
				pos.z--;
				break;
		case Forward:
				pos.y--;
				break;
		case Backward:
				pos.y++;
				break;
	}
}

void Cuboid::round() {
		int left=pos.x; 
		if (fabs(pos.x-left)<fabs(pos.x-(left+1)))
			pos.x =	left;
		else
			pos.x =	left+1;

		int front=pos.y; 
		if (fabs(pos.y-front)<fabs(pos.y-(front+1)))
			pos.y =	front;
		else
			pos.y =	front+1;

		int bottom=pos.z; 
		if (fabs(pos.z-bottom)<fabs(pos.z-(bottom+1)))
			pos.z =	bottom;
		else
			pos.z =	bottom+1;
}
