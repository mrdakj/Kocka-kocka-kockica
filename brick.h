#ifndef _BRICK_H_
#define _BRICK_H_

/* FIX move this enum somewhere else */
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

class Brick {
	friend class Space;

	public:
		/* attributes */
		Position pos;
		Size size;
		Color color;

		/* constructors */
		Brick();
		Brick(Position pos, Size size);
		Brick(Position pos, Size size, Color color);

		/* FIX this should be private */
		/* place a brick at the nearest matrix field */
		void round();

		/* draw a brick of a given size at a given position */
		void draw_brick() const;

	private:
		bool in_car;

		/* draw a cuboid of a given size with left corner at (0, 0, 0) */
		void draw_cuboid() const;

		/* draw depth times width cylinders */
		void draw_cylinder() const;
};

#endif
