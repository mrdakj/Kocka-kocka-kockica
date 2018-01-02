#ifndef _VECTOR3F_
#define _VECTOR3F_

#include <cmath>

class Vector3f {
	public: 
		float x, y, z;

	public:
		Vector3f() : x(0), y(0), z(0) {}
		Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
		Vector3f(const Vector3f& v) : x(v.x), y(v.y), z(v.z) {}

		void operator=(const Vector3f& v) {
			x = v.x;
			y = v.y;
			z = v.z;
		}


		Vector3f operator+(const Vector3f& v) {
			Vector3f result(x+v.x, y+v.y, z+v.z);
			return result;
		}

		Vector3f operator+=(const Vector3f& v) {
			x += v.x;
			y += v.y;
			z += v.z;
		}

		Vector3f operator-(const Vector3f& v) {
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

		float norm_squared() {
			return x*x+y*y+z*z;
		}
		
		float dot(Vector3f& v) {
			return x*v.x+y*v.y+z*v.z;
		}

		float norm() {
			return (float)std::sqrt(x*x+y*y+z*z);
		}


		Vector3f operator*(const Vector3f& v) {
			Vector3f result(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
			return result;
		}

		Vector3f operator*(const float& scalar) {
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

		float get(int coordinate) {
			if (coordinate==0) return x;
			if (coordinate==1) return y;
			if (coordinate==2) return z;
			return -1;
		}
};

#endif
