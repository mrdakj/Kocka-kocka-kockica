#ifndef _BRICK_H_
#define _BRICK_H_

#include "vector3f.h"
#include <stdio.h>

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

friend class Room;

public:
	/* attributes */
	Vector3f pos;
	Size size;
	Color color;

	/* constructors */
	Brick();
	Brick(Vector3f pos, Size size);
	Brick(Vector3f pos, Size size, Color color);

	/* draw a brick of a given size and color at a given position */
	void draw_brick() const;

	Vector3f get_world_coordinates() const;

	void move(Direction d, float delta);
	void move_to_position(Direction d, float position);


private:
	bool in_car;

	/* clipping planes of cylinder */
	float cylinder_front() const;
	float cylinder_back() const;
	float cylinder_left() const;
	float cylinder_right() const;

	float get_distance(Direction d) const;

	/* draw a cuboid of a given size with left down corner at (0, 0, 0) */
	void draw_cuboid() const;

	/* draw depth times width cylinders */
	void draw_cylinder() const;

	/* draw a transparent brick of a given size at a given position */
	void draw_transparent_brick() const;

	/* draw a brick of a given size at a given position */
	void draw_normal_brick() const;

	/* place a brick at the nearest matrix field */
	void round();

};

#endif
