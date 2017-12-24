#ifndef _SPACE_H_
#define _SPACE_H_


/* car ID */
#define CAR 255

#include "brick.h"
#include "car.h"
#include <vector>
#include <cstdio>
#include <cmath>
#include <cstring>


class Space {
	private:
		/* returns false if some index is out of range, otherwise sets field (i,j,k) to value and returns true */
		bool set_matrix_field(int i, int j, int k, unsigned char value);

		/* returns -1 if some index is out of range, otherwise gets field (i,j,k) */
		int get_matrix_field(int i, int j, int k);

		/* fill matrix with id=number (ranges from 1 to 255) at position determied by c */
		void update_matrix(unsigned char number, Brick& c);

		/* help function for move function */
		bool check_sides(bool x, bool y, bool z, int lowb1, int upb1, int lowb2, int upb2, int a, float cposz);

		void print_matrix();
		void check_above(Brick& c);

	public:
		Car car;
		/* number of bricks */
		int num;

		/* size of matrix */
		int size;

		/* index of selected brick brick in vector bricks, -1 if nothing is selected */
		int selected_brick;

		/* index of brick with id is id-1 */
		std::vector<Brick> bricks;

		/* matrix filled with bricks' ids */
		std::vector<std::vector<std::vector<unsigned char>>> matrix;

		/* draw a grid */
		void draw_grid(Color c) const;

		/* constructors */
		Space();
		Space(int size);


		/* add a brick to the space */
		void add(Brick& c);

		/* render all bricks and a grid */
		void render();
		void draw_car();

		/* move a brick indexed with index in direction d */
		bool move(int index, Direction d);

		/* clear a part of the matrix where selected_brick brick is */
		void select();

		/* fill a part of the matrix where brick is left */
		void deselect();

		/* put down selected_brick brick */
		void put_down();

		/* pick a brick with index=id-1 */
		void pick(int id);

	
};

#endif
