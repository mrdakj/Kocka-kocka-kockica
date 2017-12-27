#ifndef _SPACE_H_
#define _SPACE_H_

/* car ID */
#define CAR 255
/* nothing is selected */
#define NONE -1

#include "brick.h"
#include "car.h"
#include <vector>


class Space {
	private:
		/* returns false if some index is out of range, otherwise sets field (i,j,k) to value and returns true */
		bool set_matrix_field(int i, int j, int k, unsigned char value);

		/* returns -1 if some index is out of range, otherwise gets field (i,j,k) */
		int get_matrix_field(int i, int j, int k) const;

		/* fill matrix with id=number (ranges from 1 to 255) at position determied by c */
		void update_matrix(unsigned char number, Brick& c);

		/* help function for move function */
		bool check_sides(bool x, bool y, bool z, int lowb1, int upb1, int lowb2, int upb2, int a, float cposz) const;

		/* visit all bricks connected with brick c from above */
		void dfs(Brick& c);

		/* resize matrix and set fields to 0 */
		void init_matrix();

		void matrix_set_car();

	public:
		Car car;

		/* number of bricks */
		int num;

		/* size of matrix */
		int size;

		/* index of selected brick brick in vector bricks, -1 if nothing is selected */
		int selected_brick;

		float brick_move_speed;

		/* index of brick with id is id-1 */
		std::vector<Brick> bricks;

		/* matrix filled with bricks' ids */
		std::vector<std::vector<std::vector<unsigned char>>> matrix;

		/* constructors */
		Space();
		Space(int size);

		/* draw a grid */
		void draw_grid(Color c) const;

		/* add a brick to the space */
		void add(Brick& c);

		/* render all bricks and a grid */
		void render_space() const;

		/* draw car base and all bricks in it */
		void draw_car();

		void render();

		/* move a brick indexed with index in direction d */
		bool move(int index, Direction d) const;

		/* clear a part of the matrix where selected brick brick is */
		void select();

		/* fill a part of the matrix where brick is left */
		void deselect();

		/* put down selected brick */
		void put_down();

		/* pick a brick with index=id-1 */
		void pick(int id);
};

#endif
