#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <GL/glut.h>

struct ut_Point {
	int x, y, z;

	ut_Point(int x, int y, int z) : x(x), y(y), z(z) {}
	ut_Point() : ut_Point(0, 0, 0) {}
};

void ut_draw_line(const ut_Point& A, const ut_Point& B);
void ut_draw_cuboid(float width, float depth, float height);

/* returns 0, 1 or 2 */
int ut_index_of_minimum(const float& a, const float& b, const float& c);


#endif
