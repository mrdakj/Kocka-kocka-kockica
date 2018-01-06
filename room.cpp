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

bool Room::is_brick(int id)
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

Vector3f Room::move_selected_brick(Direction d, float brick_move_speed)
{
	if (std::fabs(brick_move_speed)>1)
		return Vector3f();

	Vector3f init_pos = bricks[selected_brick_id - 1].get_world_coordinates();

	float& coordinate =
		(d == Left || d == Right) ? bricks[selected_brick_id - 1].pos.x : ((d == Forward || d == Backward) ? bricks[selected_brick_id - 1].pos.y : bricks[selected_brick_id - 1].pos.z);

	int z = (d == Left || d == Forward || d == Down) ? -1 : 1;

	int line = (z == 1) ? std::ceil(coordinate) : std::floor(coordinate);


	float limit = 1.2 * brick_move_speed; // 1.2>1 so we are sure the brick cannot go inside

	if (d == Down)
		limit += 0.2;


	if (std::fabs(line - coordinate) <= limit) {
		if (d != Down)
			coordinate = line;

		if (can_move(d))
			coordinate += z*brick_move_speed;
		else {
			if (d == Down)
				coordinate = line+0.2;
		}
	}
	else
		coordinate += z*brick_move_speed;

	return bricks[selected_brick_id - 1].get_world_coordinates() - init_pos;
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

