#ifndef _CAR_H_
#define _CAR_H_

enum Wheel {
	wheel_1, wheel_2, wheel_3, wheel_4
};


class Car {

public:
	int position_x;
	int position_y;
	int width;
	int depth;

	bool is_going;

	Car();
	Car(int postion_x, int postion_y, int width, int depth);

	void draw_base() const;
	void stop();
	void go();
	float get_position() const;

	/* wheel clipping planes */
	float wheel_left(Wheel w) const;
	float wheel_right(Wheel w) const;
	float wheel_front(Wheel w) const;
	float wheel_back(Wheel w) const;
	int wheels_top() const;

	/* get x coordinate of the center */
	float center_x(Wheel w) const;

private:
	int wheel_rotation_angle;
	float translation_x;
	void draw_cylinder() const;
	void draw_brick() const;
};

#endif
