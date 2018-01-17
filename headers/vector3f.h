#ifndef _VECTOR3F_
#define _VECTOR3F_

namespace ut {
	class Vector3f {
		public: 
			float x, y, z;

		public:
			/* constructors */
			Vector3f(float x = 0, float y = 0, float z = 0);
			Vector3f(const Vector3f& v);

			Vector3f operator+(const Vector3f& v) const;
			Vector3f operator+(const float scalar) const;

			Vector3f operator-(const Vector3f& v) const;
			void operator+=(const Vector3f& v);

			Vector3f operator*(const Vector3f& v) const;
			Vector3f operator*(const float& scalar) const;
			friend Vector3f operator*(float scalar, Vector3f v);

			void set(Vector3f v);
			void set(float x, float y, float z);

			/* set every coordinate to nan */
			void set_nan();
			/* returns true if some coordinate is nan */
			bool is_nan() const;

			void print() const;

			/* convert each coordiante to integer */
			Vector3f coordinates_to_integer();

			/* dot product */
			float dot(Vector3f& v) const;

			float norm_squared() const;
			float norm() const;
			void normalize();

			/* get x, y or z coordinate */
			float get(int coordinate) const;
	};
}
#endif
