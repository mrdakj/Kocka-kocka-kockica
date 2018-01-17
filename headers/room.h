#ifndef _SPACE_H_
#define _SPACE_H_

#include <GL/glut.h>
#include <vector>

#include "brick.h"
#include "car.h"


#define BRICK_SPEED 0.08

class Room {
	private:

		bool refresh_car;

		GLuint car_display_list;
		
		/* returns false if some index is out of range, otherwise sets field (i,j,k) to value and returns true */
		bool set_matrix_field(int i, int j, int k, unsigned char value);


		/* fill matrix with id=number (ranges from 1 to 255) at position determied by c */
		void update_matrix(unsigned char id, const Brick& c);

		/* help function for move function */
		bool check_sides(Direction d, int lowb1, int upb1, int lowb2, int upb2, int a) const;

		/* returns true if a brick indexed with index can be moved in direction d */
		bool can_move(Direction d) const;

		void draw_wall() const;

		void draw_floor() const;

		/* visit all bricks connected with brick c from above */
		void dfs(Brick& c);
		void dfs();

		/* resize matrix and set fields to EMPTY */
		void init_matrix();
		void matrix_set_car();

		/* returns true if the brick can be added to the room */
		bool can_add(const Brick& c) const;

		/* returns true if id belongs to some brick */
		bool is_brick(int id) const;

		float collision_wheel(Direction d, float brick_move_speed, Wheel wheel) const;
		float collision_wheels(Direction d, float brick_move_speed) const;
		void collision_cylinder(Direction d, float brick_move_speed);
		bool collision_cylinder(Brick& other, Direction d, float brick_move_speed);

	public:
		/* size of matrix */
		int size;

		Car car;

		int number_of_bricks;

		/* id of selected brick brick, NONE if nothing is selected */
		int selected_brick_id;

		/* index of brick with id is id-1 */
		std::vector<Brick> bricks;

		/* matrix filled with bricks' ids */
		std::vector<std::vector<std::vector<unsigned char>>> matrix;

		/* constructors */
		Room(int size = 20);
		~Room();


		/* returns ERROR if some index is out of range, otherwise gets field (i,j,k) */
		int get_matrix_field(int i, int j, int k) const;

		/* draw a grid */
		void draw_grid() const;

		/* add a brick to the room */
		void add(const Brick& c);

		/* render all bricks, car base, walls nad floor */
		void render_room() const;

		/* draw car base and all bricks in it */
		void draw_car();

		void render();

		/* move selected brick and return difference */
		ut::Vector3f move_selected_brick(Direction d, float brick_move_speed);
		
		/* move selected brick in plane y=0 */
		ut::Vector3f move_selected_brick(ut::Vector3f direction, float delta_x, float delta_z);

		/* clear a part of the matrix where selected brick brick is */
		void select();

		/* fill a part of the matrix where brick is left */
		void deselect();

		/* put down selected brick */
		void put_down();

		/* pick a brick with index=id-1 */
		void pick(int id);

		/* retruns true if nothing is selected */
		bool nothing_selected();


};

#endif
