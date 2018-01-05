#include "headers/vector3f.h"
#include <cmath>
#include <stdio.h>
#include <limits>

/* constructors */

Vector3f::Vector3f() : x(0), y(0), z(0) {}

Vector3f::Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}

Vector3f::Vector3f(const Vector3f& v) : x(v.x), y(v.y), z(v.z) {}

/* end of constructors */


void Vector3f::set_nan() {
	float not_a_number = std::numeric_limits<float>::quiet_NaN();
	set(not_a_number, not_a_number, not_a_number);
}


bool Vector3f::is_nan() const {
	return std::isnan(x) || std::isnan(y) || std::isnan(z);
}

void Vector3f::print() const {
	printf("x = %f\n", x);
	printf("y = %f\n", y);
	printf("z = %f\n", z);
}


Vector3f Vector3f::operator+(const Vector3f& v) const {
	return Vector3f(x+v.x, y+v.y, z+v.z);
}

Vector3f Vector3f::operator+(const float scalar) const {
	return Vector3f(x + scalar, y + scalar, z + scalar);
}

Vector3f Vector3f::coordinates_to_integer() {
	return Vector3f((int)x, (int)y, (int)z);
}

void Vector3f::operator+=(const Vector3f& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

Vector3f Vector3f::operator-(const Vector3f& v) const {
	return Vector3f(x-v.x, y-v.y, z-v.z);
}

void Vector3f::set(Vector3f v) {
	set(v.x, v.y, v.z);
}

void Vector3f::set(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3f::norm_squared() const {
	return x*x+y*y+z*z;
}

float Vector3f::norm() const {
	return (float)std::sqrt(norm_squared());
}

float Vector3f::dot(Vector3f& v) const {
	return x*v.x+y*v.y+z*v.z;
}



Vector3f Vector3f::operator*(const Vector3f& v) const {
	return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

Vector3f Vector3f::operator*(const float& scalar) const {
	return Vector3f(x*scalar, y*scalar, z*scalar);
}

Vector3f operator*(float scalar, Vector3f v) {
	return v*scalar;
}

void Vector3f::normalize() {
	float length = norm();
	x /= length;
	y /= length;
	z /= length;
}

float Vector3f::get(int coordinate) const {
	switch (coordinate) {
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		default:
			return -1;
	}
}
