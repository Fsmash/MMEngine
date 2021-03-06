#pragma once
#include <iostream>
#include "vec2.h"

namespace mme {
	namespace math {

		struct vec3
		{
			float x, y, z;

			vec3();
			vec3(const float x, const float y, const float z);
			vec3(const vec2 &vec, const float z);

			float normal();
			vec3& normalize();
			float dot(const vec3 &other);
			vec3& add(const vec3 &other);
			vec3& subtract(const vec3 &other);
			vec3& multiply(const vec3 &other);
			vec3& scale(const float scale);
			inline float sum() { return x + y + z; }
			vec3& divide(const vec3 &other);

			friend vec3 operator+(vec3 left, const vec3 &right);
			friend vec3 operator-(vec3 left, const vec3 &right);
			friend vec3 operator*(vec3 left, const vec3 &right);
			friend vec3 operator/(vec3 left, const vec3 &right);

			bool operator==(const vec3 &other);
			bool operator!=(const vec3 &other);

			vec3& operator+=(const vec3 &other);
			vec3& operator-=(const vec3 &other);
			vec3& operator*=(const vec3 &other);
			vec3& operator/=(const vec3 &other);

			friend std::ostream& operator<<(std::ostream& stream, const vec3 &vector);

		};
	}
}