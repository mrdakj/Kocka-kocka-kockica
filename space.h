#ifndef _CUBODIS_H_
#define _CUBODIS_H_

#include "cuboid.h"
#include <vector>
#include <cstdio>
#include <cmath>
#include <cstring>


class Space {
	private:
		// fill matrix with id=number at position determied by c
		void updateMatrix(unsigned char number, Cuboid& c);

		// help function for move function
		bool checkSides(bool x, bool y, bool z, int lowb1, int upb1, int lowb2, int upb2, int a, float cposz);

		void printMatrix();

	public:
		// number of cuboids
		int num;

		// size of matrix
		int size;

		// index of selected cuboid in vector cuboids, -1 if nothing is selected
		int selected;

		std::vector<Cuboid> cuboids;

		// matrix filled with cuboids ids
		std::vector<std::vector<std::vector<unsigned char>>> matrix;

		// draw a grid at y=h
		void drawGrid(float h, Color c);

		// constructors
		Space();
		Space(int size);
		~Space(void);


		// add a cuboid to the space
		void add(Cuboid& c);

		// render all cuboids and a grid
		void render();

		// move a cuboid indexed with index in direction d
		bool move(int index, Direction d);

		// clear a part of the matrix where selected cuboid is
		void select();

		// fill a part of the matrix where cuboid is left
		void deselect();

		// put down selected cuboid
		void putDown();

		// pick a cuboid with index=id-1
		void pick(int id);

	
};

#endif
