#include "space.h"
#include <GL/glut.h>
#include <cmath>
#include "utility.h"
#include "collision.h"

/* constuctors */

Space::Space() : Space(20) {}

Space::Space(int size) {
	this->size = size;
	selected_brick = NONE;
	bricks.reserve(255);
	num = 0;
	brick_move_speed = 0.08;

	/* set matrix */
	init_matrix();
	matrix_set_car();
}

/* end constuctors */


void Space::init_matrix() {
	matrix.resize(size);

	for (int i = 0; i < size; i++) {
		matrix[i].resize(size);

		for (int j = 0; j < size; j++) {
		   matrix[i][j].resize(size);

		   for (int k = 0; k < size; k++) {
			   set_matrix_field(i, j, k, 0);
		   }
		}
	}
}

void Space::matrix_set_car() {
	set_matrix_field(car.position_x, car.position_y, 0, CAR);
	set_matrix_field(car.position_x-1, car.position_y, 0, CAR);
	set_matrix_field(car.position_x+car.width, car.position_y, 0, CAR);
	set_matrix_field(car.position_x+car.width-1, car.position_y, 0, CAR);
	set_matrix_field(car.position_x+car.width, car.position_y+car.depth, 0, CAR);
	set_matrix_field(car.position_x+car.width-1, car.position_y+car.depth, 0, CAR);
	set_matrix_field(car.position_x, car.position_y+car.depth, 0, CAR);
	set_matrix_field(car.position_x-1, car.position_y+car.depth, 0, CAR);
}


void Space::add(Brick& c) {
	num++;
	bricks.push_back(c);
	update_matrix(bricks.size(), c);
}


bool Space::set_matrix_field(int i, int j, int k, unsigned char value) {
	if (i<0 || i>=size || j<0 || j>=size || k<0 || k>=size) return false;

	matrix[i][j][k] = value;
	return true;
}

int Space::get_matrix_field(int i, int j, int k) const {
	if (i<0 || i>=size || j<0 || j>=size || k<0 || k>=size) return -1;

	return matrix[i][j][k];
}


void Space::render_space() const {
	car.draw_base();
	
	draw_grid(Color(1,1,1));

	for (const Brick& b : bricks) {
		b.draw_brick();
	}
}

void Space::dfs(Brick& c) {
	int h = c.pos.z + c.size.height;
	if (h>=size) return;

	for (int i = c.pos.x; i < c.pos.x+c.size.width; i++) {
		for (int j = c.pos.y; j < c.pos.y+c.size.depth; j++) {
			int id = get_matrix_field(i,j,h);

			if (id != -1 && id != 0 && id != CAR) {
				if (bricks[id-1].in_car == false) {
					dfs(bricks[id-1]);
				}
				bricks[id-1].in_car = true;
			}
		}
	}
}

void Space::draw_car() {

	car.draw_base();

	/* FIX do this only once */

	for (Brick& c : bricks) {
		c.in_car=false;
	}

	for (int i = car.position_x; i < car.position_x+car.width; i++) {
		for (int j = car.position_y+1; j < car.position_y+car.depth; j++) {
			int id = get_matrix_field(i,j,0);
			if (id != -1 && id != 0 && id != 255) {
				if (bricks[id-1].in_car == false) {
					dfs(bricks[id-1]);
				}
				bricks[id-1].in_car = true;
			}
		}
	}

	for (const Brick& c : bricks) {
		if (c.in_car)
			c.draw_brick();
	}
}

void Space::render() {
	glPushMatrix();
	glTranslatef(-car.translation_x,0,0);

	if (car.is_going) 
		draw_car();
	else
		render_space();
	glPopMatrix();
}

void Space::draw_grid(Color c) const {
	glDisable(GL_LIGHTING);

	glColor3f(c.r, c.g, c.b);
	glLineWidth(1);

	for (int i = 0; i < size+1; i++) {
		glPushMatrix();
		glTranslatef(0, 0, -i);
		ut_draw_line(ut_Point(), ut_Point(size, 0, 0));
		glPopMatrix();

		glPushMatrix();
		glTranslatef(i, 0, 0);
		ut_draw_line(ut_Point(), ut_Point(0, 0, -size));
		glPopMatrix();
	}

	glEnable(GL_LIGHTING);
}

void Space::update_matrix(unsigned char number, Brick& c) {
	for (int x = c.pos.x; x < c.pos.x+c.size.width; x++) {
		for (int y = c.pos.y; y < c.pos.y+c.size.depth; y++) {
			for (int z = c.pos.z; z < c.pos.z+c.size.height; z++) {
				set_matrix_field(x, y, z, number);
			}
		}
	}
}

void Space::select() {
	update_matrix(0 , bricks[selected_brick]);
}

void Space::deselect() {
	update_matrix(selected_brick+1 , bricks[selected_brick]);
}

bool Space::check_sides(bool x, bool y, bool z, int lowb1, int upb1, int lowb2, int upb2, int a, float cposz) const {
	for (int i = lowb1; i < upb1; i++) {
		for (int j = lowb2; j < upb2; j++) {
				if (!x && get_matrix_field(a,i,j) != 0) return false;
				if (!y && get_matrix_field(i,a,j) != 0) return false;
				if (!z && get_matrix_field(i,j,a) != 0) return false;

				if (j>=1) {
					if (!x && get_matrix_field(a,i,j-1) != 0 && cposz+0.01<j+0.2) return false;
					if (!y && get_matrix_field(i,a,j-1) != 0 && cposz+0.01<j+0.2) return false;
				}
		}
	}

	return true;
}

bool Space::move(int index, Direction d) const {

	const Brick& c = bricks[index];

	int x = c.pos.x;
	int y = c.pos.y;
	int z = c.pos.z;

	int minx = std::floor(c.pos.x);
	int maxx = std::ceil(c.pos.x);
	int miny = std::floor(c.pos.y);
	int maxy = std::ceil(c.pos.y);
	int minz = std::floor(c.pos.z);
	int maxz = std::ceil(c.pos.z);

	int width = c.size.width;
	int depth = c.size.depth;
	int height = c.size.height;

	switch(d) {
		case Left:
			if (x == 0) return false;
			return check_sides(false, true, true, miny, maxy+depth, minz, maxz+height, x-1, c.pos.z);
		case Right:
			if (x+width == size) return false;
			return check_sides(false, true, true, miny, maxy+depth, minz, maxz+height, x+width, c.pos.z);
		case Forward:
			if (y == 0) return false;
			return check_sides(true, false, true, minx, maxx+width, minz, maxz+height, y-1, c.pos.z);
		case Backward:
			if (y+depth == size) return false;
			return check_sides(true, false, true, minx, maxx+width, minz, maxz+height, y+depth, c.pos.z);
		case Down:
			if (z == 0) return false;
			return check_sides(true, true, false, minx, maxx+width, miny, maxy+depth, z-1, c.pos.z);
		case Up:
			if (z+height == size) return false;
			return check_sides(true, true, false, minx, maxx+width, miny, maxy+depth, z+height, c.pos.z);
	}

	return true;
}


void Space::put_down() {
	if (selected_brick == NONE)
		return;

	bricks[selected_brick].pos.z -= 0.2;
	bricks[selected_brick].round();
	deselect();
	selected_brick = NONE;
}

void Space::pick(int id) {
	if (id < 1 || id > num) 
		return;

	id--;
	selected_brick = id;

	Brick& c = bricks[selected_brick];

	if (c.pos.z+c.size.height<size) {
		for (int i = 0; i < c.size.width; i++) {
			for (int j = 0; j < c.size.depth; j++) {
				if (get_matrix_field(c.pos.x+i, c.pos.y+j, c.pos.z+c.size.height) != 0) {
					selected_brick = NONE;
					return;
				}
			}
		}
	}

	select();
	c.pos.z += 0.2;
}

/* bool Space::move_brick(Direction d, Brick& c,float brick_move_speed) { */
/* 	if (std::fabs(brick_move_speed)>1) */
/* 		return false; */

/* 	bool returnVal = false; */
/* 	float xstart = c.pos.x; */
/* 	float ystart = c.pos.y; */
/* 	float zstart = c.pos.z; */

/* 	float& coordinate = */
/* 		(d == Left || d == Right) ? c.pos.x : ((d == Forward || d == Backward) ? c.pos.y : c.pos.z); */

/* 	int z = (d == Left || d == Forward || d == Down) ? -1 : 1; */

/* 	int line = (z == 1) ? std::ceil(coordinate) : std::floor(coordinate); */


/* 	float limit = 1.2 * brick_move_speed; // 1.2>1 so we are sure the brick cannot go inside */
/* 	if (d == Down) */
/* 		limit += 0.2; */


/* 	if (std::fabs(line - coordinate) <= limit) { */
/* 		if (d != Down) */
/* 			coordinate = line; */

/* 		if (space.move(space.selected_brick, d)) { */
/* 			coordinate += z*brick_move_speed; */
/* 			returnVal =true; */
/* 		} */
/* 		else { */
/* 			if (d == Down) */
/* 				coordinate = line+0.2; */

/* 			returnVal=false; */
/* 		} */
/* 	} */
/* 	else { */
/* 		coordinate += z*brick_move_speed; */
/* 		returnVal=true; */
/* 	} */

	/* if (d==Left || d==Right) */
	/* 	objX += c.pos.x - xstart; */
	/* if (d==Forward || d==Backward) */
	/* 	objZ += -c.pos.y + ystart; */
	/* if (d==Up || d==Down) */
	/* 	objY += c.pos.z-zstart; */

	/* to.x = objX-position.x; */
	/* to.z = objZ-position.z; */
	/* to.y = objY-position.y; */
	/* float modultheta = std::sqrt(to.x*to.x+to.z*to.z+to.y*to.y); */

	/* to.x /= modultheta; */
	/* to.z /= modultheta; */
	/* to.y /= modultheta; */

	/* return returnVal; */
/* } */

/* void Space::move_delta(float delta_x, float delta_y, Brick& current_brick) { */

	/* float d = std::sqrt(to.x*to.x+to.z*to.z); */


	/* float ax=0,bx=0,ay=0,by=0; */

	/* /1* project normal vector of (projection of TO vector in XZ plane) of length delta_x to x and z axes *1/ */
	/* if (delta_x) { */
	/* 	bx=-to.z*(delta_x/d); */
	/* 	ax=to.x*(delta_x/d); */
	/* } */


	/* /1* project (projection of TO) vector of length delta_y to x and z axes *1/ */
	/* if (delta_y) { */
	/* 	by=to.x*(delta_y/d); */
	/* 	ay=to.z*(delta_y/d); */
	/* } */

	/* float a = ax+ay; */
	/* float b = bx+by; */

	/* /1* move only along tangent of circle *1/ */
	/* float distance =Vector3f(objX-position.x, 0, objZ-position.z).norm_squared(); */
	/* if (distance<1*1 && delta_y<0) { */
	/* 	a=ax; */
	/* 	b=bx; */
	/* } */

	/* /1* if this is > 1 collision will not work properly because we cannot skip a field *1/ */
	/* if (a>0.9) */
	/* 	a = 0.9; */
	/* if (a<-0.9) */
	/* 	a = -0.9; */
	/* if (b>0.9) */
	/* 	b = 0.9; */
	/* if (b<-0.9) */
	/* 	b = -0.9; */


	/* if (b>0) */
	/* 	move_brick(Right,current_brick,b); */
	/* if (b<0) */
	/* 	move_brick(Left,current_brick,-b); */
	/* if (a>0) */
	/* 	move_brick(Forward,current_brick,a); */
	/* if (a<0) */
	/* 	move_brick(Backward,current_brick,-a); */

/* } */
