#ifndef _BRICK_H_
#define _BRICK_H_

#include "vector3f.h"

/* FIX move this enum somewhere else */
enum Direction {
	Up, Down,
	Left, Right,
	Forward, Backward
};

struct Color {
	float r, g, b, a;
	Color() : r(1), g(0), b(0), a(1) {}
	Color(float r, float g, float b) : r(r), g(g), b(b), a(1) {}
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
};


struct Size {
	int width, height, depth;
	Size() : width(1), height(1), depth(1) {}
	Size(int width, int height, int depth) :  width(width), height(height), depth(depth) {}
};

class Brick {
	friend class Space;

	public:
		/* attributes */
		Vector3f pos;
		Size size;
		Color color;

		/* constructors */
		Brick();
		Brick(Vector3f pos, Size size);
		Brick(Vector3f pos, Size size, Color color);

		/* FIX this should be private */
		/* place a brick at the nearest matrix field */
		void round();

		/* draw a brick of a given size at a given position */
		void draw_brick() const;
		void draw_transparent_brick() const;
		void draw_normal_brick() const;

	private:
		bool in_car;

		/* draw a cuboid of a given size with left corner at (0, 0, 0) */
		void draw_cuboid() const;

		/* draw depth times width cylinders */
		void draw_cylinder() const;
};

#endif
