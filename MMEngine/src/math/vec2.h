#pragma once
#include <iostream>

namespace mme {
	namespace math {

		struct vec2
		{
			float x, y;

			vec2();
			vec2(const float x, const float y);

			float normal();
			vec2& normalize();
			float dot(const vec2 &other);
			vec2& add(const vec2 &other);
			vec2& subtract(const vec2 &other);
			vec2& multiply(const vec2 &other);
			vec2& scale(const float scale);
			inline float sum() { return x + y; }
			vec2& divide(const vec2 &other);

			friend vec2 operator+(vec2 left, const vec2 &right);
			friend vec2 operator-(vec2 left, const vec2 &right);
			friend vec2 operator*(vec2 left, const vec2 &right);
			friend vec2 operator/(vec2 left, const vec2 &right);

			bool operator==(const vec2 &other);
			bool operator!=(const vec2 &other);

			vec2& operator+=(const vec2 &other);
			vec2& operator-=(const vec2 &other);
			vec2& operator*=(const vec2 &other);
			vec2& operator/=(const vec2 &other);

			friend std::ostream& operator<<(std::ostream &stream, const vec2 &vector);

		};
	}
}