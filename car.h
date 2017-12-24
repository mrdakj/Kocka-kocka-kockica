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
	float translate_x;
	bool is_going;

	Car();
	Car(int postion_x, int postion_y, int width, int depth);

	void draw_base() const;
	void stop();
	void go();

private:
	void draw_cylinder() const;
	void draw_brick() const;
};

#endif
