#ifndef _CAR_H_
#define _CAR_H_

enum Wheel {
	Wheel_1, Wheel_2, Wheel_3, Wheel_4
};


class Car {

public:
	int position_x;
	int position_y;
	int width;
	int depth;

	bool is_going;

	Car(int postion_x = 4, int postion_y = 10, int width = 12, int depth = 6);

	void draw_base() const;
	void stop();
	void go();
	float get_position() const;

	/* wheel clipping planes */
	float Wheel_left(Wheel w) const;
	float Wheel_right(Wheel w) const;
	float Wheel_front(Wheel w) const;
	float Wheel_back(Wheel w) const;
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
