#include "space.h"

Space::Space(int size) {
	this->size = size;
	this->selected = -1;

	num = 0;

	matrix.resize(size);
	for (int i = 0; i < size; i++) {
		matrix[i].resize(size);
		for (int j = 0; j < size; j++) {
		   matrix[i][j].resize(size);
		   for (int k = 0; k < size; k++) {
		   		matrix[i][j][k] = 0;
		   }
		}
	}
}

void Space::add(Cuboid& c) {
	num++;
	cuboids.push_back(c);
	updateMatrix(cuboids.size(), c);
}


void Space::printMatrix() {
	for (int z = size-1; z >= 0; z--) {
		printf("------------------------------------ z=%d\n", z);
		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				printf("%d ", matrix[x][y][z]);
			}
			printf("\n");
		}
	}
}

void Space::render() {
	drawGrid(0, Color(1,1,1));

	for (int i = 0; i < num; i++)
		cuboids[i].render();	
}

void Space::drawGrid(float h, Color c) {
	for (int i = 0; i < 2*size+2; i++) {
		glPushMatrix();

		if (i < size+1)
			glTranslatef(0, 0, -i);

		if (i >= size+1) {
			glTranslatef(i-(size+1), 0, -size);
			glRotatef(-90, 0, 1, 0);
		}

		glBegin(GL_LINES);
			glColor3f(c.r, c.g, c.b);
			glLineWidth(1);
			glVertex3f(0, h, 0);
			glVertex3f(size, h, 0);
		glEnd();

		glPopMatrix();
	}
}

void Space::updateMatrix(unsigned char number, Cuboid& c) {
	for (int x = c.pos.x; x < c.pos.x+c.size.width; x++) {
		for (int y = c.pos.y; y < c.pos.y+c.size.depth; y++) {
			for (int z = c.pos.z; z < c.pos.z+c.size.height; z++) {
				matrix[x][y][z] = number;	
			}
		}
	}
}

void Space::select() {
	updateMatrix(0 , cuboids[selected]);
}

void Space::deselect() {
	updateMatrix(selected+1 , cuboids[selected]);
}

bool Space::checkSides(bool x, bool y, bool z, int lowb1, int upb1, int lowb2, int upb2, int a, float cposz) {
	for (int i = lowb1; i < upb1; i++) {
		for (int j = lowb2; j < upb2; j++) {
			if (i>=0 && i<size && j>=0 && j<size) {
				if (!x && matrix[a][i][j] != 0) return false;
				if (!y && matrix[i][a][j] != 0) return false;
				if (!z && matrix[i][j][a] != 0) return false;
			}
			if (i>=0 && i<size && j>=1 && j<size) {
				if (!x && matrix[a][i][j] == 0 && matrix[a][i][j-1] != 0 && cposz+0.01<j+0.2) return false;
				if (!y && matrix[i][a][j] == 0 && matrix[i][a][j-1] != 0 && cposz+0.01<j+0.2) return false;
			}
		}
	}

	return true;
}

bool Space::move(int index, Direction d) {

	Cuboid& c = cuboids[index];
	int x = c.pos.x;
	int y = c.pos.y;
	int z = c.pos.z;

	int minx = floor(c.pos.x);
	int maxx = ceil(c.pos.x);
	int miny = floor(c.pos.y);
	int maxy = ceil(c.pos.y);
	int minz = floor(c.pos.z);
	int maxz = ceil(c.pos.z);

	int width = c.size.width;
	int depth = c.size.depth;
	int height = c.size.height;

	switch(d) {
		case Left:
			if (x == 0) return false;
			return checkSides(false, true, true, miny, maxy+depth, minz, maxz+height, x-1, c.pos.z);
		case Right:
			if (x+width == size) return false;
			return checkSides(false, true, true, miny, maxy+depth, minz, maxz+height, x+width, c.pos.z);
		case Forward:
			if (y == 0) return false;
			return checkSides(true, false, true, minx, maxx+width, minz, maxz+height, y-1, c.pos.z);
		case Backward:
			if (y+depth == size) return false;
			return checkSides(true, false, true, minx, maxx+width, minz, maxz+height, y+depth, c.pos.z);
		case Down:
			if (z == 0) return false;
			return checkSides(true, true, false, minx, maxx+width, miny, maxy+depth, z-1, c.pos.z);
		case Up:
			if (z+height == size) return false;
			return checkSides(true, true, false, minx, maxx+width, miny, maxy+depth, z+height, c.pos.z);
	}

	return true;
}


void Space::putDown() {
	if (selected == -1)
		return;

	cuboids[selected].pos.z -= 0.2;
	cuboids[selected].round();
	deselect();
	selected = -1;
}

void Space::pick(int id) {
	if (id < 1 || id > num) 
		return;

	id--;
	selected = id;

	select();
	cuboids[selected].pos.z += 0.2;
}
