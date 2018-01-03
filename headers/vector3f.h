#ifndef _VECTOR3F_
#define _VECTOR3F_

#include <cmath>
#include <stdio.h>
#include <limits>

class Vector3f {
	public: 
		float x, y, z;

	public:
		Vector3f() : x(0), y(0), z(0) {}
		Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
		Vector3f(const Vector3f& v) : x(v.x), y(v.y), z(v.z) {}

		void set_nan() {
			x = std::numeric_limits<float>::quiet_NaN();
			y = std::numeric_limits<float>::quiet_NaN();
			z = std::numeric_limits<float>::quiet_NaN();
		}


		bool is_nan() const {
			return std::isnan(x) || std::isnan(y) || std::isnan(z);
		}

		void print() const {
			printf("x = %f\n", x);
			printf("y = %f\n", y);
			printf("z = %f\n", z);
		}

		void operator=(const Vector3f& v) {
			x = v.x;
			y = v.y;
			z = v.z;
		}


		Vector3f operator+(const Vector3f& v) const {
			Vector3f result(x+v.x, y+v.y, z+v.z);
			return result;
		}

		Vector3f operator+=(const Vector3f& v) {
			x += v.x;
			y += v.y;
			z += v.z;
		}

		Vector3f operator-(const Vector3f& v) const {
			Vector3f result(x-v.x, y-v.y, z-v.z);
			return result;
		}

		Vector3f set(Vector3f v) {
			x = v.x;
			y = v.y;
			z = v.z;
		}

		void set(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		float norm_squared() const {
			return x*x+y*y+z*z;
		}
		
		float dot(Vector3f& v) const {
			return x*v.x+y*v.y+z*v.z;
		}

		float norm() const {
			return (float)std::sqrt(x*x+y*y+z*z);
		}


		Vector3f operator*(const Vector3f& v) const {
			Vector3f result(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
			return result;
		}

		Vector3f operator*(const float& scalar) const {
			Vector3f result(x*scalar, y*scalar, z*scalar);
			return result;
		}
		
		friend Vector3f operator*(float scalar, Vector3f v) {
			return v*scalar;
		}

		void normalize() {
			float length = norm();
			x /= length;
			y /= length;
			z /= length;
		}

		float get(int coordinate) const {
			if (coordinate==0) return x;
			if (coordinate==1) return y;
			if (coordinate==2) return z;
			return -1;
		}
};

#endif
