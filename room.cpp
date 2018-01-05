#include "headers/room.h"
#include <GL/glut.h>
#include <cmath>
#include "headers/utility.h"
#include "headers/collision.h"
#include "headers/textures.h"

/* constuctors */

Room::Room() : Room(20) {}

Room::Room(int size) {
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


void Room::init_matrix() {
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

void Room::matrix_set_car() {
	set_matrix_field(car.position_x, car.position_y, 0, CAR);
	set_matrix_field(car.position_x-1, car.position_y, 0, CAR);
	set_matrix_field(car.position_x+car.width, car.position_y, 0, CAR);
	set_matrix_field(car.position_x+car.width-1, car.position_y, 0, CAR);
	set_matrix_field(car.position_x+car.width, car.position_y+car.depth, 0, CAR);
	set_matrix_field(car.position_x+car.width-1, car.position_y+car.depth, 0, CAR);
	set_matrix_field(car.position_x, car.position_y+car.depth, 0, CAR);
	set_matrix_field(car.position_x-1, car.position_y+car.depth, 0, CAR);
}


void Room::add(Brick& c) {
	num++;
	bricks.push_back(c);
	update_matrix(bricks.size(), c);
}


bool Room::set_matrix_field(int i, int j, int k, unsigned char value) {
	if (i<0 || i>=size || j<0 || j>=size || k<0 || k>=size) return false;

	matrix[i][j][k] = value;
	return true;
}

int Room::get_matrix_field(int i, int j, int k) const {
	if (i<0 || i>=size || j<0 || j>=size || k<0 || k>=size) return -1;

	return matrix[i][j][k];
}

void Room::draw_wall() const {

	glColor3f(0.7, 0, 0);
    glBindTexture(GL_TEXTURE_2D, texture_names[0]);

	glPushMatrix();
	glTranslatef(size, -0.5, 0);
	ut_draw_rectangle_with_texture_YZ(size/2, size);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.5, -size);
	ut_draw_rectangle_with_texture_XY(size, size/2);
	glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Room::render_room() const {

	draw_wall();
	
	draw_grid(Color(1,1,1));

	draw_floor();

	car.draw_base();

	for (const Brick& b : bricks) {
		b.draw_brick();
	}
}

void Room::dfs(Brick& c, std::vector<bool>& dfs_visited) {
	int h = c.pos.z + c.size.height;
	if (h>=size) return;

	for (int i = c.pos.x; i < c.pos.x+c.size.width; i++) {
		for (int j = c.pos.y; j < c.pos.y+c.size.depth; j++) {
			int id = get_matrix_field(i,j,h);

			if (id != -1 && id != 0 && id != CAR) {
				if (dfs_visited[id-1] == false) {
					dfs(bricks[id-1], dfs_visited);
				}
				dfs_visited[id-1] = true;
			}
		}
	}
}

void Room::draw_car() {

	/* FIX dont do this every time */

	std::vector<bool> dfs_visited(num);
	std::fill(dfs_visited.begin(), dfs_visited.end(), false);


	for (int i = car.position_x; i < car.position_x+car.width; i++) {
		for (int j = car.position_y+1; j < car.position_y+car.depth; j++) {
			int id = get_matrix_field(i,j,0);
			if (id != -1 && id != 0 && id != 255) {
				if (dfs_visited[id-1] == false) {
					dfs(bricks[id-1], dfs_visited);
				}
				dfs_visited[id-1] = true;
			}
		}
	}

	
	car.draw_base();

	glPushMatrix();
	glTranslatef(car.get_position(), 0, 0);
	for (int i = 0; i < num; i++) {
		if (dfs_visited[i])
			bricks[i].draw_brick();
	}

	glPopMatrix();
}

void Room::render() {
	if (car.is_going)
		draw_car();
	else
		render_room();
}

void Room::draw_floor() const {

    glBindTexture(GL_TEXTURE_2D, texture_names[1]);

	ut_draw_rectangle_with_texture_XZ(car.position_x-1, size);

	glPushMatrix();
	glTranslatef(car.position_x-1, 0, 0);
	ut_draw_rectangle_with_texture_XZ(car.width+2, car.position_y);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(car.position_x-1+car.width+2, 0, 0);
	ut_draw_rectangle_with_texture_XZ(size-car.position_x-car.width-1, size);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(car.position_x-1, 0, -car.position_y-car.depth-1);
	ut_draw_rectangle_with_texture_XZ(car.width+2, size-car.position_y-car.depth-1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	ut_draw_rectangle_with_texture_XY(size, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	ut_draw_rectangle_with_texture_YZ(0.5, size);
	glPopMatrix();
	
    glBindTexture(GL_TEXTURE_2D, 0);

}

void Room::draw_grid(Color c) const {
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

void Room::update_matrix(unsigned char number, Brick& c) {
	for (int x = c.pos.x; x < c.pos.x+c.size.width; x++) {
		for (int y = c.pos.y; y < c.pos.y+c.size.depth; y++) {
			for (int z = c.pos.z; z < c.pos.z+c.size.height; z++) {
				set_matrix_field(x, y, z, number);
			}
		}
	}
}

void Room::select() {
	update_matrix(0 , bricks[selected_brick]);
}

void Room::deselect() {
	update_matrix(selected_brick+1 , bricks[selected_brick]);
}

bool Room::check_sides(bool x, bool y, bool z, int lowb1, int upb1, int lowb2, int upb2, int a, float cposz) const {
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

bool Room::move(int index, Direction d) const {

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


void Room::put_down() {
	if (selected_brick == NONE)
		return;

	bricks[selected_brick].pos.z -= 0.2;
	bricks[selected_brick].round();
	deselect();
	selected_brick = NONE;

}

void Room::pick(int id) {
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

bool Room::nothing_selected() {
	return selected_brick == NONE;
}
