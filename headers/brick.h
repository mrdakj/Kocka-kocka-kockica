#ifndef _BRICK_H_
#define _BRICK_H_

#include "vector3f.h"
#include <stdio.h>

enum Direction {
	Up_direction, Down_direction,
	Left_direction, Right_direction,
	Forward_direction, Backward_direction
};

struct Color {
	float r, g, b, a;
	/* red is a default color */
	Color(float r = 1, float g = 0, float b = 0, float a = 1) : r(r), g(g), b(b), a(a) {}
};

struct Size {
	int width, height, depth;
	Size(int width = 1, int height = 1, int depth = 1) :  width(width), height(height), depth(depth) {}
};

class Brick {

friend class Room;

public:
	/* attributes */
	ut::Vector3f pos;
	Size size;
	Color color;

	/* constructors */
	Brick(Color color);
	Brick(Size size, Color color = Color());
	Brick(ut::Vector3f pos = ut::Vector3f(), Size size = Size(), Color color = Color());

	/* draw a brick of a given size and color at a given position */
	void draw_brick() const;

	ut::Vector3f get_world_coordinates() const;

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
