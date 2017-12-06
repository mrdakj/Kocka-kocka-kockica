#include  "cuboid.h"
#include <stdio.h>

void Cuboid::setMaterial() {
	GLfloat ambient_coeffs[] = {0.3, 0.3, 0.3, 1};
	GLfloat specular_coeffs[] = {0.8,0.8,0.8,1};
	GLfloat shininess = 20;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

Cuboid::Cuboid() {
	this->diffuse_coeffs[0] = 0.5;
	this->diffuse_coeffs[1] = 0.5;
	this->diffuse_coeffs[2] = 0.5;
	this->diffuse_coeffs[3] = 1;
	obj = gluNewQuadric();
}

Cuboid::Cuboid(Position pos, Size size) : pos(pos), size(size) {
	this->diffuse_coeffs[0] = 0.8;
	this->diffuse_coeffs[1] = 0.5;
	this->diffuse_coeffs[2] = 0.0;
	this->diffuse_coeffs[3] = 1;
	obj = gluNewQuadric();
}

Cuboid::Cuboid(Position pos, Size size, Color color) : Cuboid(pos, size) { 
	this->color = color;
}

Cuboid::Cuboid(Position pos, Size size, GLfloat* diffuse_coeffs) : Cuboid(pos, size) {
	this->diffuse_coeffs[0] = diffuse_coeffs[0];
	this->diffuse_coeffs[1] = diffuse_coeffs[1];
	this->diffuse_coeffs[2] = diffuse_coeffs[2];
	this->diffuse_coeffs[3] = diffuse_coeffs[3];
}

/* TODO move constructor */

Cuboid::Cuboid(Cuboid const& other) {
	this->diffuse_coeffs[0] = other.diffuse_coeffs[0];
	this->diffuse_coeffs[1] = other.diffuse_coeffs[1];
	this->diffuse_coeffs[2] = other.diffuse_coeffs[2];
	this->diffuse_coeffs[3] = other.diffuse_coeffs[3];
	pos=other.pos;
	size=other.size;
	color=other.color;
	obj=gluNewQuadric();
}

Cuboid::~Cuboid(void) {
	gluDeleteQuadric(obj);
}

void Cuboid::renderCuboid() {
	int w = size.width;
	int h = size.height;
	int d = size.depth;

	glBegin(GL_QUADS);

    //Front
	glNormal3f(0,0,1);
    glVertex3f(0, 0, 0);
    glVertex3f(w, 0, 0);
    glVertex3f(w, h, 0);
    glVertex3f(0, h, 0);

    //Right
	glNormal3f(1,0,0);
    glVertex3f(w, 0, 0);
    glVertex3f(w, 0, -d);
    glVertex3f(w, h, -d);
    glVertex3f(w, h, 0);

    //Back
	glNormal3f(0,0,-1);
    glVertex3f(w, 0, -d);
    glVertex3f(w, h, -d);
    glVertex3f(0, h, -d);
    glVertex3f(0, 0, -d);

    //Left
	glNormal3f(-1,0,0);
    glVertex3f(0, 0, -d);
    glVertex3f(0, h, -d);
    glVertex3f(0, h, 0);
    glVertex3f(0, 0, 0);

	//Top
	glNormal3f(0,1,0);
    glVertex3f(0, h, 0);
    glVertex3f(w, h, 0);
    glVertex3f(w, h, -d);
    glVertex3f(0, h, -d);

	//Bottom
	glNormal3f(0,-1,0);
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
			/* glBegin(GL_POLYGON); */
			gluCylinder(obj, 0.25, 0.25, 0.2, 30,30);
			glPopMatrix();
		}
	}
}

void Cuboid::render() {
	glColor3f(color.r, color.g, color.b);
	setMaterial();
	
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
