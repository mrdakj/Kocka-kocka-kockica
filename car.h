#ifndef _CAR_H_
#define _CAR_H_


class Car {

friend class space;

public:
	int position_x;
	int position_y;
	int width;
	int depth;
	int wheel_rotation_angle;

	Car();
	Car(int postion_x, int postion_y, int width, int depth);

	void draw_base();

private:
	void draw_cylinder();
	void draw_brick();
};

#endif