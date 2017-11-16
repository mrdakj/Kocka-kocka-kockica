#ifndef _CUBODIS_H_
#define _CUBODIS_H_

#include "cuboid.h"
#include <vector>
#include <cstdio>
#include <cmath>
#include <cstring>

struct SelectionCube {
	Position pos;

	void render() {
		glPushMatrix();
		glColor3f(0,1,0);
		glTranslatef(0.5, 0.5, -0.5);
		glTranslatef(pos.x, pos.z, -pos.y);
		glutWireCube(1);
		glPopMatrix();
	}

	SelectionCube() {}
};

class Space {
	private:
		void updateMatrix(unsigned char number, Cuboid& c);
		SelectionCube sc;

	public:
		// number of cuboids
		int num;
		int size;
		void drawGrid(float h, Color c);
		std::vector<Cuboid> cuboids;
		void printMatrix();
		std::vector<std::vector<std::vector<unsigned char>>> matrix;

		Space();
		Space(int size);
		void add(Cuboid& c);
		void render(int selected);
		// move a cuboid indexed with index in direction d
		bool move(int index, Direction d);
		// clear a part of the matrix where selected cuboid is
		void select(int index);
		// fill a part of the matrix where cuboid is left
		void deselect(int index);
		// get element of matrix where selection cube is
		int getSelection();
};

#endif
