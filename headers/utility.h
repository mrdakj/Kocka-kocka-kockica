#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <GL/glut.h>

struct ut_Point {
	GLdouble x, y, z;

	ut_Point(GLdouble x, GLdouble y, GLdouble z) : x(x), y(y), z(z) {}
	ut_Point() : ut_Point(0, 0, 0) {}
};

void ut_draw_line(const ut_Point& A, const ut_Point& B);
void ut_draw_triangle(const ut_Point& A, const ut_Point& B, const ut_Point& C);
void ut_draw_cuboid(float width, float depth, float height);
ut_Point ut_world_to_screen_coordinates(const ut_Point& world_point);

/* draw a rectangle with texture in plane y = 0, with left down corner at (0, 0) */
void ut_draw_rectangle_with_texture_XZ(float width, float depth);
/* draw a rectangle with texture in plane z = 0, with left down corner at (0, 0) */
void ut_draw_rectangle_with_texture_XY(float width, float height);
/* draw a rectangle with texture in plane x = 0, with right down corner at (0, 0) */
void ut_draw_rectangle_with_texture_YZ(float depth, float height);

/* returns 0, 1 or 2 */
int ut_index_of_minimum(const float& a, const float& b, const float& c);


#endif
