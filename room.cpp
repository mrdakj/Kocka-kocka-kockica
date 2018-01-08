#include <cmath>

#include "headers/room.h"
#include "headers/utility.h"
#include "headers/textures.h"

/* constuctors */

Room::Room() : Room(20) {}

Room::Room(int size) : size(size)
{
	selected_brick_id = NONE;
	bricks.reserve(MAX);
	number_of_bricks = 0;

	refresh_car = true;
	car_display_list = NOT_SET;

	/* set matrix */
	init_matrix();
	matrix_set_car();
}

/* end of constuctors */

Room::~Room()
{
	if (car_display_list != (GLuint)NOT_SET)
		glDeleteLists(car_display_list, 1);

	free_textures();
}

void Room::init_matrix()
{
	matrix.resize(size);

	for (int i = 0; i < size; i++) {
		matrix[i].resize(size);
		for (int j = 0; j < size; j++) {
		   matrix[i][j].resize(size, EMPTY);
		}
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
	ut_draw_rectangle_with_texture_YZ(size/2, size);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.5, -size);
	ut_draw_rectangle_with_texture_XY(size, size/2);
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
		else
			glDeleteLists(car_display_list, 1);

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

	ut_draw_rectangle_with_texture_XZ(car.position_x-1, size);

	glPushMatrix();
	glTranslatef(car.position_x-1, 0, 0);
	ut_draw_rectangle_with_texture_XZ(car.width+2, car.position_y-1);
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

void Room::draw_grid() const
{
	glDisable(GL_LIGHTING);

	glColor3f(1, 1, 1);
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


bool Room::check_sides(bool x, bool y, bool z, int lowb1, int upb1, int lowb2, int upb2, int a, float cposz) const {
	for (int i = lowb1; i < upb1; i++) {

		if (cposz+0.01<(int)cposz+0.2) {
			if (!x && is_brick(get_matrix_field(a,i,(int)cposz-1))) return false;
			if (!y && is_brick(get_matrix_field(i,a,(int)cposz-1))) return false;
		}

		for (int j = lowb2; j < upb2; j++) {
				if (!x && (is_brick(get_matrix_field(a,i,j)) || get_matrix_field(a,i,j) == ERROR)) return false;
				if (!y && (is_brick(get_matrix_field(i,a,j)) || get_matrix_field(i,a,j) == ERROR)) return false;
				if (!z && (is_brick(get_matrix_field(i,j,a)) || get_matrix_field(i,j,a) == ERROR)) return false;
		}
	}

	return true;
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
		case Left:
			return check_sides(false, true, true, miny, maxy+depth, minz, maxz+height, x-1, c.pos.z);
		case Right:
			return check_sides(false, true, true, miny, maxy+depth, minz, maxz+height, x+width, c.pos.z);
		case Forward:
			return check_sides(true, false, true, minx, maxx+width, minz, maxz+height, y-1, c.pos.z);
		case Backward:
			return check_sides(true, false, true, minx, maxx+width, minz, maxz+height, y+depth, c.pos.z);
		case Down:
			return check_sides(true, true, false, minx, maxx+width, miny, maxy+depth, z-1, c.pos.z);
		case Up:
			return check_sides(true, true, false, minx, maxx+width, miny, maxy+depth, z+height, c.pos.z);
	}

	return true;
}

float Room::collision_wheel(Direction d, float brick_move_speed, float car_corner_x, float car_corner_y) {
	Brick& c = bricks[selected_brick_id - 1];
	float limit = 1.2 * brick_move_speed; // 1.2>1 so we are sure the brick cannot go inside

	float eps = 0.001;

	if (d == Down && c.pos.z <= 1+limit) {

		bool cond_x = (c.pos.x < car_corner_x-1 && c.pos.x+c.size.width >= car_corner_x-1)
					|| (c.pos.x >= car_corner_x-1 && c.pos.x <= car_corner_x+1);

		bool cond_y = (c.pos.y < car_corner_y-0.8 && c.pos.y+c.size.depth >= car_corner_y-0.8)
					|| (c.pos.y >= car_corner_y-0.8 && c.pos.y <= car_corner_y-0.2);

		if (cond_x && cond_y) {
			float suggested_position = c.pos.z - brick_move_speed;
			float limit_position;
			if (c.pos.x >= car_corner_x)
				limit_position = std::sqrt(1-(c.pos.x-car_corner_x)*(c.pos.x-car_corner_x));
			else if (c.pos.x+c.size.width <= car_corner_x)
					limit_position = std::sqrt(1-(c.pos.x+c.size.width-car_corner_x)*(c.pos.x+c.size.width-car_corner_x));
			else
				limit_position=1;

			return (suggested_position > limit_position) ? suggested_position : limit_position;
		}
	}

	if (d == Left && c.pos.x>=car_corner_x && c.pos.x<=car_corner_x+1+limit) {
		bool cond_y = (c.pos.y < car_corner_y-0.8+eps && c.pos.y+c.size.depth > car_corner_y-0.8+eps)
					|| (c.pos.y > car_corner_y-0.8+eps && c.pos.y < car_corner_y-0.2-eps);
		bool cond_z = c.pos.z < 1;

		if (cond_y && cond_z) {
			float suggested_position = c.pos.x - brick_move_speed;
			float limit_position = car_corner_x+std::sqrt(1-c.pos.z*c.pos.z);


			return (suggested_position > limit_position) ? suggested_position : limit_position;
		}
		
	}

	if (d == Right && c.pos.x+c.size.width<=car_corner_x && c.pos.x+c.size.width>=car_corner_x-1-limit) {
		bool cond_y = (c.pos.y < car_corner_y-0.8+eps && c.pos.y+c.size.depth >= car_corner_y-0.8+eps)
					|| (c.pos.y >= car_corner_y-0.8+eps && c.pos.y <= car_corner_y-0.2-eps);
		bool cond_z = c.pos.z < 1;

		if (cond_y && cond_z) {
			float suggested_position = c.pos.x + brick_move_speed;
			float limit_position = car_corner_x-std::sqrt(1-c.pos.z*c.pos.z)-c.size.width;


			return (suggested_position < limit_position) ? suggested_position : limit_position;
		}

	}

	if (d==Backward && c.pos.y+c.size.depth < car_corner_y-0.8+eps && c.pos.y+c.size.depth>=car_corner_y-0.8-limit) {
		bool cond_1 = (car_corner_x-c.pos.x)*(car_corner_x-c.pos.x) + c.pos.z*c.pos.z <= 1;
		bool cond_2 = (car_corner_x-(c.pos.x+c.size.width))*(car_corner_x-(c.pos.x+c.size.width)) + c.pos.z*c.pos.z <= 1;
		bool cond_3 = c.pos.x <= car_corner_x && c.pos.x+c.size.width >= car_corner_x && c.pos.z <= 1;

		if (cond_3 || cond_1 || cond_2) {
			float suggested_position = c.pos.y + brick_move_speed;
			float limit_position = car_corner_y-0.8-c.size.depth;

			

			return (suggested_position < limit_position) ? suggested_position : limit_position;
		}
	}

	if (d==Forward && c.pos.y > car_corner_y-0.2-eps && c.pos.y<=car_corner_y-0.2+limit) {
		bool cond_1 = (car_corner_x-c.pos.x)*(car_corner_x-c.pos.x) + c.pos.z*c.pos.z <= 1;
		bool cond_2 = (car_corner_x-(c.pos.x+c.size.width))*(car_corner_x-(c.pos.x+c.size.width)) + c.pos.z*c.pos.z <= 1;
		bool cond_3 = c.pos.x <= car_corner_x && c.pos.x+c.size.width >= car_corner_x && c.pos.z <= 1;

		if (cond_3 || cond_1 || cond_2) {

			float suggested_position = c.pos.y - brick_move_speed;
			float limit_position = car_corner_y-0.2;


			return (suggested_position > limit_position) ? suggested_position : limit_position;
		}
	}

	return -1;

}

Vector3f Room::move_selected_brick(Direction d, float brick_move_speed)
{
	if (std::fabs(brick_move_speed)>1)
		return Vector3f();


	/* car collision */
	float new_value = -1;
	if (new_value == -1)
		new_value = collision_wheel(d, brick_move_speed, car.position_x, car.position_y);
	if (new_value == -1)
		new_value = collision_wheel(d, brick_move_speed, car.position_x+car.width, car.position_y);
	if (new_value == -1)
		new_value = collision_wheel(d, brick_move_speed, car.position_x+car.width, car.position_y+car.depth+1);
	if (new_value == -1)
		new_value = collision_wheel(d, brick_move_speed, car.position_x, car.position_y+car.depth+1);
	/* end of car collision*/

	Brick& c = bricks[selected_brick_id - 1];

	Vector3f init_pos = bricks[selected_brick_id - 1].get_world_coordinates();
	float limit = 1.2 * brick_move_speed; // 1.2>1 so we are sure the brick cannot go inside

	float& coordinate = (d == Left || d == Right) ?
						c.pos.x : (d == Forward || d == Backward) ?
						c.pos.y : c.pos.z;

	int z = (d == Left || d == Forward || d == Down) ?
			-1 : 1;

	int line = (z == 1) ?
			std::ceil(coordinate) : std::floor(coordinate);

	if (d == Down)
		limit += 0.2;

	float distance = std::fabs(line - coordinate);

	if (distance <= limit) {
		if (d != Down)
			coordinate = line;

		if (can_move(d)) {
			if (d == Down)
				coordinate += z*brick_move_speed;
			else
				coordinate += z*(brick_move_speed-distance);
		}
		else {
			if (d == Down)
				coordinate = line+0.2;
		}
	}
	else
		coordinate += z*brick_move_speed;



	if (new_value != -1) {
		if (d == Left && new_value > coordinate)
			coordinate = new_value;
		if (d == Right && new_value < coordinate)
			coordinate = new_value;
		if (d == Forward && new_value > coordinate)
			coordinate = new_value;
		if (d == Backward && new_value < coordinate)
			coordinate = new_value;
		if (d == Down && new_value > coordinate)
			coordinate = new_value;
	}
	
	if (new_value>18)
	{
		printf("new_value = %f\n", new_value);
		exit(0);
	}

	return c.get_world_coordinates() - init_pos;
}


Vector3f Room::move_selected_brick(Vector3f direction, float delta_x, float delta_z)
{

	float d = std::sqrt(direction.x*direction.x+direction.z*direction.z);


	float ax=0,bx=0,ay=0,by=0;

	/* project normal vector of (projection of TO vector in XZ plane) of length delta_x camera.view x and z axes */
	if (delta_x) {
		bx=-direction.z*(delta_x/d);
		ax=direction.x*(delta_x/d);
	}


	/* project (projection of TO) vector of length delta_z camera.view x and z axes */
	if (delta_z) {
		by=direction.x*(delta_z/d);
		ay=direction.z*(delta_z/d);
	}

	float a = ax+ay;
	float b = bx+by;

	/* if this is > 1 collision will not work properly because we cannot skip a field */
	if (a>0.9)
		a = 0.9;
	if (a<-0.9)
		a = -0.9;
	if (b>0.9)
		b = 0.9;
	if (b<-0.9)
		b = -0.9;

	Vector3f delta;

	if (b>0)
		delta += move_selected_brick(Right, b);
	if (b<0)
		delta += move_selected_brick(Left,-b);
	if (a>0)
		delta += move_selected_brick(Forward,a);
	if (a<0)
		delta += move_selected_brick(Backward,-a);

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

	if (c.pos.z+c.size.height<size) {
		for (int i = 0; i < c.size.width; i++) {
			for (int j = 0; j < c.size.depth; j++) {
				if (get_matrix_field(c.pos.x+i, c.pos.y+j, c.pos.z+c.size.height) != 0) {
					selected_brick_id = NONE;
					return;
				}
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

