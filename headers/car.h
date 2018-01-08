#ifndef _CAR_H_
#define _CAR_H_


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
	float get_position();

private:
	int wheel_rotation_angle;
	float translation_x;
	void draw_cylinder() const;
	void draw_brick() const;
};

#endif
