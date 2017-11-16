#ifndef _CUBOID_H_
#define _CUBOID_H_

#include <GL/glut.h>
#include <cmath>

enum Direction {
	Up, Down,
	Left, Right,
	Forward, Backward
};

struct Color {
	float r, g, b;
	Color() : r(1), g(0), b(0) {}
	Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

struct Position {
	float x, y, z;
	Position() : x(0), y(0), z(0) {}
	Position(int x, int y, int z) : x(x), y(y), z(z) {}
};

struct Size {
	int width, height, depth;
	Size() : width(1), height(1), depth(1) {}
	Size(int width, int height, int depth) :  width(width), height(height), depth(depth) {}
};

class Cuboid {
	friend class Space;

	public:
		Position pos;
		Size size;
		Color color;
		Cuboid();
		Cuboid(Position pos, Size size);
		Cuboid(Position pos, Size size, Color color);
		void round();

	private:
		void renderCuboid();
		void renderCylinder();
		void render();
		void move(Direction d);
};

#endif
