#include "../headers/utility.h"

namespace ut {
	void draw_line(const Point& A,const Point& B)
	{
		glBegin(GL_LINES);
		glVertex3f(A.x, A.y, A.z);
		glVertex3f(B.x, B.y, B.z);
		glEnd();
	}

	void draw_triangle(const Point& A, const Point& B, const Point& C)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(A.x, A.y, A.z);
		glVertex3f(B.x, B.y, B.z);
		glVertex3f(C.x, C.y, C.z);
		glEnd();
	}

	Point world_to_screen_coordinates(const Point& world_point)
	{
		GLdouble modelview_matrix[16];
		GLdouble projection_matrix[16];
		GLint viewport[4];

		glGetDoublev(GL_MODELVIEW_MATRIX,modelview_matrix);
		glGetDoublev(GL_PROJECTION_MATRIX,projection_matrix);
		glGetIntegerv(GL_VIEWPORT,viewport); 

		int window_width = viewport[2];
		int window_height = viewport[3];

		Point screen_point;

		gluProject(world_point.x,world_point.y,world_point.z,modelview_matrix,projection_matrix,viewport,&screen_point.x, &screen_point.y, &screen_point.z);

		screen_point.y = screen_point.y-window_height/2;
		screen_point.x = screen_point.x - window_width/2;
		screen_point.x  /= window_width/2;
		screen_point.y /= window_height/2;

		return screen_point;
	}

	void draw_cuboid(float width, float depth, float height)
	{
		glEnable(GL_NORMALIZE);

		glPushMatrix();
		glTranslatef(width/2, height/2, -depth/2);
		glScalef(width, height, depth);
		glutSolidCube(1);
		glPopMatrix();

		glDisable(GL_NORMALIZE);
	}

	/* draw a rectangle with texture in plane y = 0, with bottom left corner at (0, 0) */
	void draw_rectangle_with_texture_XZ(float width, float depth)
	{
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glTexCoord2f(0, 0);
		glVertex3f(0, 0, 0);
		glTexCoord2f(width, 0);
		glVertex3f(width, 0, 0);
		glTexCoord2f(width, depth);
		glVertex3f(width, 0, -depth);
		glTexCoord2f(0, depth);
		glVertex3f(0, 0, -depth);
		glEnd();
	}

	/* draw a rectangle with texture in plane z = 0, with bottom left corner at (0, 0) */
	void draw_rectangle_with_texture_XY(float width, float height)
	{
		glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0, 0);
		glVertex3f(0, 0, 0);
		glTexCoord2f(width, 0);
		glVertex3f(width, 0, 0);
		glTexCoord2f(width, height);
		glVertex3f(width, height, 0);
		glTexCoord2f(0, height);
		glVertex3f(0, height, 0);
		glEnd();
	}

	/* draw a rectangle with texture in plane x = 0, with bottom right corner at (0, 0) */
	void draw_rectangle_with_texture_YZ(float height, float depth)
	{
		glBegin(GL_QUADS);
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0, 0);
		glVertex3f(0, 0, -depth);
		glTexCoord2f(depth, 0);
		glVertex3f(0, 0, 0);
		glTexCoord2f(depth, height);
		glVertex3f(0, height, 0);
		glTexCoord2f(0, height);
		glVertex3f(0, height, -depth);
		glEnd();

	}

	int index_of_minimum(const float& a, const float& b, const float& c)
	{
		if (a <= b) {
			if (a <= c) return 0;
			return 2;
		}

		if (b <= c) return 1;
		return 2;
	}
}
