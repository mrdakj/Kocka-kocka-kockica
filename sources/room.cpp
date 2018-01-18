#include <cmath>

#include "../headers/room.h"
#include "../headers/utility.h"
#include "../headers/textures.h"

#define eps 0.001

/* max number of bricks in the room */
/* 254 + 2 = 256 */
#define MAX 254

/* ID of car */
#define CAR 255
/* ID of nothing */
#define EMPTY 0

#define NONE -1
#define NOT_SET -1
#define ERROR -1

#define NO_COLLISION 0
#define COLLISION 1

/* constuctors */

Room::Room(int size) : refresh_car(true), car_display_list(NOT_SET), size(size), number_of_bricks(0), selected_brick_id(NONE) 
{
	bricks.reserve(MAX);

	/* set matrix */
	init_matrix();
	matrix_set_car();
}

/* end of constuctors */

Room::~Room()
{
	glDeleteLists(car_display_list, 1);
	free_textures();
}

void Room::init_matrix()
{
	matrix.resize(size);

	for (int i = 0; i < size; i++) {
		matrix[i].resize(size);
		for (int j = 0; j < size; j++)
			matrix[i][j].resize(size, EMPTY);
	}
}

void Room::matrix_set_car()
{
	/* wheel 1 */
	set_matrix_field(car.position_x, car.position_y - 1, 0, CAR);
	set_matrix_field(car.position_x - 1, car.position_y - 1, 0, CAR);

	/* wheel 2 */
	set_matrix_field(car.position_x + car.width, car.position_y - 1, 0, CAR);
	set_matrix_field(car.position_x + car.width - 1, car.position_y - 1, 0, CAR);

	/* wheel 3 */
	set_matrix_field(car.position_x+car.width, car.position_y + car.depth, 0, CAR);
	set_matrix_field(car.position_x+car.width - 1, car.position_y + car.depth, 0, CAR);

	/* wheel 4 */
	set_matrix_field(car.position_x, car.position_y + car.depth, 0, CAR);
	set_matrix_field(car.position_x - 1, car.position_y + car.depth, 0, CAR);
}

bool Room::can_add(const Brick& c) const
{
	for (int x = c.pos.x; x < c.pos.x + c.size.width; x++) {
		for (int y = c.pos.y; y < c.pos.y + c.size.depth; y++) {
			for (int z = c.pos.z; z < c.pos.z + c.size.height; z++) {
				if  (get_matrix_field(x, y, z) != EMPTY)
					return false;
			}
		}
	}

	return true;
}

void Room::add(const Brick& c)
{
	if (number_of_bricks == MAX || !can_add(c)) return;

	number_of_bricks++;
	bricks.push_back(c);
	update_matrix(number_of_bricks, c);
}


bool Room::set_matrix_field(int i, int j, int k, unsigned char value)
{
	if (i<0 || i>=size || j<0 || j>=size || k<0 || k>=size)
		return false;

	matrix[i][j][k] = value;
	return true;
}

int Room::get_matrix_field(int i, int j, int k) const
{
	if (i<0 || i>=size || j<0 || j>=size || k<0 || k>=size)
		return ERROR;

	return matrix[i][j][k];
}

void Room::draw_wall() const
{
	glColor3f(0.7, 0, 0);
    glBindTexture(GL_TEXTURE_2D, texture_names[0]);

	glPushMatrix();
	glTranslatef(size, -0.5, 0);
	ut::draw_rectangle_with_texture_YZ(size/2, size);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.5, -size);
	ut::draw_rectangle_with_texture_XY(size, size/2);
	glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Room::render_room() const
{
	draw_wall();
	draw_grid();
	draw_floor();

	car.draw_base();

	for (const Brick& b : bricks) {
		b.draw_brick();
	}
}

bool Room::is_brick(int id) const
{
	return id != ERROR && id != EMPTY && id != CAR;
}

void Room::dfs(Brick& c)
{
	int h = c.pos.z + c.size.height;
	if (h >= size) return;

	for (int i = c.pos.x; i < c.pos.x + c.size.width; i++) {
		for (int j = c.pos.y; j < c.pos.y + c.size.depth; j++) {
			int id = get_matrix_field(i, j, h);

			if (is_brick(id)) {
				if (!bricks[id-1].in_car)
					dfs(bricks[id-1]);

				bricks[id-1].in_car = true;
			}
		}
	}
}


void Room::dfs()
{
	for (int i = car.position_x; i < car.position_x + car.width; i++) {
		for (int j = car.position_y; j < car.position_y + car.depth; j++) {
			int id = get_matrix_field(i, j, 0);

			if (is_brick(id)) {
				if (!bricks[id-1].in_car)
					dfs(bricks[id-1]);

				bricks[id-1].in_car = true;
			}
		}
	}
}

void Room::draw_car()
{
	if (refresh_car) {
		for (Brick& c : bricks) {
			c.in_car = false;
		}

		dfs();

		if (car_display_list == (GLuint)(NOT_SET))
			car_display_list = glGenLists(1);

		glNewList(car_display_list, GL_COMPILE);
		for (const Brick& brick : bricks) {
			if (brick.in_car)
				brick.draw_brick();
		}
		glEndList();

		refresh_car = false;
	}

	car.draw_base();

	glPushMatrix();
	glTranslatef(car.get_position(), 0, 0);
	glCallList(car_display_list);
	glPopMatrix();
}

void Room::render()
{
	if (car.is_going)
		draw_car();
	else
		render_room();
}

void Room::draw_floor() const
{
    glBindTexture(GL_TEXTURE_2D, texture_names[1]);

	ut::draw_rectangle_with_texture_XZ(car.position_x-1, size);

	glPushMatrix();
	glTranslatef(car.position_x-1, 0, 0);
	ut::draw_rectangle_with_texture_XZ(car.width+2, car.position_y-1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(car.position_x-1+car.width+2, 0, 0);
	ut::draw_rectangle_with_texture_XZ(size-car.position_x-car.width-1, size);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(car.position_x-1, 0, -car.position_y-car.depth-1);
	ut::draw_rectangle_with_texture_XZ(car.width+2, size-car.position_y-car.depth-1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	ut::draw_rectangle_with_texture_XY(size, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	ut::draw_rectangle_with_texture_YZ(0.5, size);
	glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Room::draw_grid() const
{
	glDisable(GL_LIGHTING);

	glColor3f(1, 1, 1);
	glLineWidth(1);

	for (int i = 0; i < size+1; i++) {
		glPushMatrix();
		glTranslatef(0, 0, -i);
		ut::draw_line(ut::Point(), ut::Point(size, 0, 0));
		glPopMatrix();

		glPushMatrix();
		glTranslatef(i, 0, 0);
		ut::draw_line(ut::Point(), ut::Point(0, 0, -size));
		glPopMatrix();
	}

	glEnable(GL_LIGHTING);
}

void Room::update_matrix(unsigned char id, const Brick& c)
{
	for (int x = c.pos.x; x < c.pos.x + c.size.width; x++) {
		for (int y = c.pos.y; y < c.pos.y + c.size.depth; y++) {
			for (int z = c.pos.z; z < c.pos.z + c.size.height; z++) {
				set_matrix_field(x, y, z, id);
			}
		}
	}
}

void Room::select()
{
	update_matrix(EMPTY , bricks[selected_brick_id - 1]);
}

void Room::deselect()
{
	update_matrix(selected_brick_id , bricks[selected_brick_id - 1]);
}


bool Room::check_sides(Direction d, int lowb1, int upb1, int lowb2, int upb2, int a) const
{
	/* this function checks collisions with cuboid (not entire brick) and borders of the room */

	for (int i = lowb1; i < upb1; i++) {
		for (int j = lowb2; j < upb2; j++) {
			int id;

			if (d == Left_direction || d == Right_direction)
				id = get_matrix_field(a, i, j);
			else if (d == Forward_direction || d == Backward_direction)
				id = get_matrix_field(i, a, j);
			else if (d == Down_direction || d == Up_direction)
				id = get_matrix_field(i, j, a);

			/* cuboid or outside */
			if (is_brick(id) || id == ERROR)
				return COLLISION;
		}
	}

	return NO_COLLISION;
}


bool Room::can_move(Direction d) const
{
	const Brick& c = bricks[selected_brick_id - 1];

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
		case Left_direction:
			return check_sides(d, miny, maxy+depth, minz, maxz+height, x-1) == NO_COLLISION;
		case Right_direction:
			return check_sides(d, miny, maxy+depth, minz, maxz+height, x+width)  == NO_COLLISION;
		case Forward_direction:
			return check_sides(d, minx, maxx+width, minz, maxz+height, y-1) == NO_COLLISION;
		case Backward_direction:
			return check_sides(d, minx, maxx+width, minz, maxz+height, y+depth) == NO_COLLISION;
		case Down_direction:
			return check_sides(d, minx, maxx+width, miny, maxy+depth, z-1) == NO_COLLISION;
		case Up_direction:
			return check_sides(d, minx, maxx+width, miny, maxy+depth, z+height) == NO_COLLISION;
	}

	return true;
}


float Room::collision_wheel(Direction d, float brick_move_speed, Wheel wheel) const
{
	const Brick& c = bricks[selected_brick_id - 1];
	float limit = 1.2 * brick_move_speed; // 1.2>1 so we are sure the brick cannot go inside

	if (d == Down_direction) {
		if (!(c.pos.z < car.wheels_top() + limit))
			return NOT_SET;

		bool cond_x = (c.pos.x < car.wheel_left(wheel) + eps && c.pos.x + c.size.width > car.wheel_left(wheel) + eps)
					|| (c.pos.x > car.wheel_left(wheel) + eps && c.pos.x < car.wheel_right(wheel) - eps);

		bool cond_y = (c.pos.y < car.wheel_front(wheel) + eps && c.pos.y + c.size.depth > car.wheel_front(wheel) + eps)
					|| (c.pos.y > car.wheel_front(wheel) + eps && c.pos.y < car.wheel_back(wheel) - eps);

		if (cond_x && cond_y) {
			float suggested_position = c.pos.z - brick_move_speed;

			/* lower bound of z coordinate of brick bottom left corner */
			float limit_position_squared;

			if (c.pos.x >= car.center_x(wheel))
				limit_position_squared = 1-std::pow(c.pos.x-car.center_x(wheel), 2);
			else if (c.pos.x+c.size.width <= car.center_x(wheel))
					limit_position_squared = 1-std::pow(c.pos.x+c.size.width-car.center_x(wheel), 2);
			else
				limit_position_squared = 1;

			if (suggested_position*suggested_position < limit_position_squared)
				return std::sqrt(limit_position_squared);
		}
	}

	/* if the brick is above the wheel and direction is not down no collision is possible */
	if (c.pos.z >= car.wheels_top())
		return NOT_SET;


	if (d == Left_direction) {
		if (!(c.pos.x > car.center_x(wheel) && c.pos.x < car.wheel_right(wheel) + limit))
			return NOT_SET;

		bool cond_y = (c.pos.y < car.wheel_front(wheel) + eps && c.pos.y+c.size.depth > car.wheel_front(wheel) + eps)
					|| (c.pos.y > car.wheel_front(wheel) + eps && c.pos.y < car.wheel_back(wheel) - eps);

		if (cond_y) {
			float suggested_position = c.pos.x - brick_move_speed;

			/* lower bound of x coordinate of bottom left brick corner */
			float limit_position = car.center_x(wheel) + std::sqrt(1 - c.pos.z*c.pos.z);

			if (suggested_position < limit_position)
				return limit_position;
		}
	}

	if (d == Right_direction) {
		if (!(c.pos.x+c.size.width < car.center_x(wheel) && c.pos.x+c.size.width > car.wheel_left(wheel) - limit))
			return NOT_SET;

		bool cond_y = (c.pos.y < car.wheel_front(wheel) + eps && c.pos.y+c.size.depth > car.wheel_front(wheel) + eps)
					|| (c.pos.y > car.wheel_front(wheel) + eps && c.pos.y < car.wheel_back(wheel) - eps);

		if (cond_y) {
			float suggested_position = c.pos.x + brick_move_speed;

			/* upper bound of x coordinate of bottom left brick corner */
			float limit_position = car.center_x(wheel) - std::sqrt(1 - c.pos.z*c.pos.z) - c.size.width;

			if (suggested_position > limit_position)
				return limit_position;
		}
	}

	if (d == Backward_direction) {
		if (!(c.pos.y+c.size.depth < car.wheel_front(wheel) + eps && c.pos.y+c.size.depth > car.wheel_front(wheel) - limit))
			return NOT_SET;

		/* bottom left corner is inside wheel circle */
		bool cond_1 = (car.center_x(wheel)-c.pos.x)*(car.center_x(wheel)-c.pos.x) + c.pos.z*c.pos.z < 1;
		/* right down corner is inside wheel circle */
		bool cond_2 = (car.center_x(wheel)-(c.pos.x+c.size.width))*(car.center_x(wheel)-(c.pos.x+c.size.width)) + c.pos.z*c.pos.z < 1;
		bool cond_3 = c.pos.x < car.center_x(wheel) && c.pos.x + c.size.width > car.center_x(wheel);

		if (cond_3 || cond_1 || cond_2) {
			float suggested_position = c.pos.y + brick_move_speed;

			/* upper bound of y coordinate of brick bottom left corner */
			float limit_position = car.wheel_front(wheel) - c.size.depth;

			if (suggested_position > limit_position)
				return limit_position;
		}
	}

	if (d==Forward_direction) {
		if (!(c.pos.y > car.wheel_back(wheel) - eps && c.pos.y < car.wheel_back(wheel) + limit))
			return NOT_SET;

		/* bottom left corner is inside wheel circle */
		bool cond_1 = (car.center_x(wheel)-c.pos.x)*(car.center_x(wheel)-c.pos.x) + c.pos.z*c.pos.z < 1;
		/* right down corner is inside wheel circle */
		bool cond_2 = (car.center_x(wheel)-(c.pos.x+c.size.width))*(car.center_x(wheel)-(c.pos.x+c.size.width)) + c.pos.z*c.pos.z < 1;
		bool cond_3 = c.pos.x < car.center_x(wheel) && c.pos.x + c.size.width > car.center_x(wheel);

		if (cond_3 || cond_1 || cond_2) {
			float suggested_position = c.pos.y - brick_move_speed;

			/* lower bound of y coordinate of brick bottom left corner */
			float limit_position = car.wheel_back(wheel);

			if (suggested_position < limit_position)
				return limit_position;
		}
	}

	return NOT_SET;
}

float Room::collision_wheels(Direction d, float brick_move_speed) const
{
	float Wheel_limit = NOT_SET;

	if (Wheel_limit ==  NOT_SET)
		Wheel_limit = collision_wheel(d, brick_move_speed, Wheel_1);
	if (Wheel_limit == NOT_SET)
		Wheel_limit = collision_wheel(d, brick_move_speed, Wheel_2);
	if (Wheel_limit == NOT_SET)
		Wheel_limit = collision_wheel(d, brick_move_speed, Wheel_3);
	if (Wheel_limit == NOT_SET)
		Wheel_limit = collision_wheel(d, brick_move_speed, Wheel_4);

	return Wheel_limit;
}

bool Room::collision_cylinder(Brick& other, Direction d, float brick_move_speed)
{
	Brick& c = bricks[selected_brick_id - 1];

	if (d == Left_direction) {
		float m = other.cylinder_front() + eps;
		float M = other.cylinder_back() - eps;
		bool cond0 = c.pos.x+brick_move_speed >= other.cylinder_right() -eps;
		bool cond1 = c.pos.y < m && c.pos.y+c.size.depth > m;
		bool cond2 = c.pos.y > m && c.pos.y < M;
		if (cond0 && (cond1 || cond2)) {
			if (other.cylinder_right() > c.pos.x) {
				c.pos.x = other.cylinder_right();
				return COLLISION;
			}
		}
	}

	if (d==Right_direction) {
		float m = other.cylinder_front() + eps;
		float M = other.cylinder_back() -eps;
		bool cond0 = c.pos.x-brick_move_speed+c.size.width <= other.cylinder_left()+eps;
		bool cond1 = c.pos.y < m && c.pos.y+c.size.depth > m;
		bool cond2 = c.pos.y > m && c.pos.y < M;
		if (cond0 && (cond1 || cond2)) {
			if (other.cylinder_left()-c.size.width < c.pos.x) {
				c.pos.x = other.cylinder_left()-c.size.width;
				return COLLISION;
			}
		}
	}

	if (d==Forward_direction) {
		float m = other.cylinder_left()+eps;
		float M = other.cylinder_right()-eps;
		bool cond0 = c.pos.y+ brick_move_speed>= other.cylinder_back()-eps;
		bool cond1 = c.pos.x < m && c.pos.x+c.size.width > m;
		bool cond2 = c.pos.x > m && c.pos.x < M;
		if (cond0 && (cond1 || cond2)) {
			if (other.cylinder_back() > c.pos.y) {
				c.pos.y = other.cylinder_back();
				return COLLISION;
			}
		}
	}

	if (d==Backward_direction) {
		float m = other.cylinder_left()+eps;
		float M = other.cylinder_right()-eps;
		bool cond0 = c.pos.y+c.size.depth-brick_move_speed <= other.cylinder_front()+eps;
		bool cond1 = c.pos.x < m && c.pos.x+c.size.width > m;
		bool cond2 = c.pos.x > m && c.pos.x < M;
		if (cond0 && (cond1 || cond2)) {
			if (other.cylinder_front()-c.size.depth < c.pos.y) {
				c.pos.y = other.cylinder_front()-c.size.depth;
				return COLLISION;
			}
		}
	}

	if (d==Down_direction) {
		float mx = other.cylinder_left()+eps;
		float Mx = other.cylinder_right()-eps;
		bool cond1x = c.pos.x < mx && c.pos.x+c.size.width > mx;
		bool cond2x = c.pos.x > mx && c.pos.x < Mx;

		float my = other.cylinder_front()+eps;
		float My = other.cylinder_back()-eps;
		bool cond1y = c.pos.y < my && c.pos.y+c.size.depth > my;
		bool cond2y = c.pos.y > my && c.pos.y < My;
		if ((cond1y || cond2y) && (cond1x || cond2x)) {
			c.pos.z = other.pos.z+other.size.height+0.2;
			return COLLISION;
		}
	}

	return NO_COLLISION;
}

void Room::collision_cylinder(Direction d, float brick_move_speed)
{
	Brick& c = bricks[selected_brick_id - 1];

	if (d == Left_direction) {
		int miny = std::floor(c.pos.y);
		int maxy = std::ceil(c.pos.y);
		int prev_id = NONE;
		for (int i = miny; i < maxy+c.size.depth; i++) {
			int current_id = get_matrix_field((int)c.pos.x, i, (int)c.pos.z-1);
			if (is_brick(current_id) && current_id != prev_id) {
				prev_id = current_id;
				if (collision_cylinder(bricks[current_id-1], d, brick_move_speed) == COLLISION)
					return;
			}
		}
	}

	if (d==Right_direction) {
		int miny = std::floor(c.pos.y);
		int maxy = std::ceil(c.pos.y);
		int prev_id = NONE;
		for (int i = miny; i < maxy+c.size.depth; i++) {
			int current_id = get_matrix_field((int)c.pos.x+c.size.width, i, (int)c.pos.z-1);
			if (is_brick(current_id) && current_id != prev_id) {
				prev_id = current_id;
				if (collision_cylinder(bricks[current_id-1], d, brick_move_speed) == COLLISION)
					return;
			}
		}
	}

	if (d==Forward_direction) {
		int minx = std::floor(c.pos.x);
		int maxx = std::ceil(c.pos.x);
		int prev_id = NONE;
		for (int i = minx; i < maxx+c.size.width; i++) {
			int current_id = get_matrix_field(i, (int)c.pos.y, (int)c.pos.z-1);
			if (is_brick(current_id) && current_id != prev_id) {
				prev_id = current_id;
				if (collision_cylinder(bricks[current_id-1], d, brick_move_speed) == COLLISION)
					return;
			}
		}
	}

	if (d==Backward_direction) {
		int minx = std::floor(c.pos.x);
		int maxx = std::ceil(c.pos.x);
		int prev_id = NONE;
		for (int i = minx; i < maxx+c.size.width; i++) {
			int current_id = get_matrix_field(i, (int)c.pos.y+c.size.depth, (int)c.pos.z-1);
			if (is_brick(current_id) && current_id != prev_id) {
				prev_id = current_id;
				if (collision_cylinder(bricks[current_id-1], d, brick_move_speed) == COLLISION)
					return;
			}
		}
	}

	if (d==Down_direction) {
		int minx = std::floor(c.pos.x);
		int maxx = std::ceil(c.pos.x);
		int miny = std::floor(c.pos.y);
		int maxy = std::ceil(c.pos.y);
		for (int i = minx; i < maxx+c.size.width; i++) {
			int prev_id = NONE;
			for (int j=miny; j<maxy+c.size.depth; j++) {
				int current_id = get_matrix_field(i, j, (int)c.pos.z-1);
				if (is_brick(current_id) && current_id != prev_id) {
					prev_id = current_id;
					if (collision_cylinder(bricks[current_id-1], d, brick_move_speed) == COLLISION)
						return;
				}
			}
		}
	}
}



ut::Vector3f Room::move_selected_brick(Direction d, float brick_move_speed)
{
	Brick& c = bricks[selected_brick_id - 1];
	ut::Vector3f init_pos = bricks[selected_brick_id - 1].get_world_coordinates();

	if (brick_move_speed < 0.003)
		return ut::Vector3f();

	if (brick_move_speed > 0.5)
		brick_move_speed = 0.5;

	float limit = 1.2 * brick_move_speed; // 1.2>1 so we are sure the brick cannot go inside

	float Wheel_limit = collision_wheels(d, brick_move_speed);

	float distance = c.get_distance(d);

	bool moved = false;

	if (distance < limit) {
		c.move(d, distance);

		if (can_move(d)) {
			c.move(d, brick_move_speed - distance);
			moved = true;
		}
	}
	else {
		c.move(d, brick_move_speed);
		moved = true;
	}

	if (moved || d == Down_direction) {
		if (c.pos.z - (int)c.pos.z < 0.2 - eps)
			collision_cylinder(d, brick_move_speed);
	}

	if (moved && Wheel_limit != NOT_SET)
		c.move_to_position(d, Wheel_limit);

	if (c.pos.z < 0.2)
		c.pos.z = 0.2;

	return c.get_world_coordinates() - init_pos;
}


ut::Vector3f Room::move_selected_brick(ut::Vector3f direction, float delta_x, float delta_z)
{
	float d = std::sqrt(direction.x*direction.x + direction.z*direction.z);

	float ax = 0, bx = 0, ay = 0, by = 0;

	if (delta_x) {
		bx = -direction.z*(delta_x/d);
		ax = direction.x*(delta_x/d);
	}

	if (delta_z) {
		by = direction.x*(delta_z/d);
		ay = direction.z*(delta_z/d);
	}

	float a = ax + ay;
	float b = bx + by;

	ut::Vector3f delta;

	if (b > 0)
		delta += move_selected_brick(Right_direction, b);
	if (b < 0)
		delta += move_selected_brick(Left_direction, -b);
	if (a > 0)
		delta += move_selected_brick(Forward_direction, a);
	if (a < 0)
		delta += move_selected_brick(Backward_direction, -a);

	return delta;
}

void Room::put_down()
{
	if (selected_brick_id == NONE)
		return;

	bricks[selected_brick_id - 1].pos.z -= 0.2;
	bricks[selected_brick_id - 1].round();
	deselect();
	selected_brick_id = NONE;

	refresh_car = true;
}

void Room::pick(int id)
{
	if (id < 1 || id > number_of_bricks)
		return;

	selected_brick_id = id;

	Brick& c = bricks[selected_brick_id - 1];

	for (int i = 0; i < c.size.width; i++) {
		for (int j = 0; j < c.size.depth; j++) {
			int id = get_matrix_field(c.pos.x+i, c.pos.y+j, c.pos.z+c.size.height);
			if (id != EMPTY && id != ERROR) {
				selected_brick_id = NONE;
				return;
			}
		}
	}

	select();
	c.pos.z += 0.2;

	refresh_car = true;
}

bool Room::nothing_selected()
{
	return selected_brick_id == NONE;
}

