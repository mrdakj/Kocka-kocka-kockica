#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <GL/glut.h>

namespace ut {

	struct Point {
		GLdouble x, y, z;

		Point(GLdouble x = 0, GLdouble y = 0, GLdouble z = 0) : x(x), y(y), z(z) {}
	};

	void draw_line(const Point& A, const Point& B);
	void draw_triangle(const Point& A, const Point& B, const Point& C);
	void draw_cuboid(float width, float depth, float height);
	Point world_to_screen_coordinates(const Point& world_point);

	/* draw a rectangle with texture in plane y = 0, with left down corner at (0, 0) */
	void draw_rectangle_with_texture_XZ(float width, float depth);
	/* draw a rectangle with texture in plane z = 0, with left down corner at (0, 0) */
	void draw_rectangle_with_texture_XY(float width, float height);
	/* draw a rectangle with texture in plane x = 0, with right down corner at (0, 0) */
	void draw_rectangle_with_texture_YZ(float depth, float height);

	/* returns 0, 1 or 2 */
	int index_of_minimum(const float& a, const float& b, const float& c);
}


#endif
