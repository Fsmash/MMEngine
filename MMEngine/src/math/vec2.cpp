#include "vec2.h"

namespace mme {
	namespace math {

		vec2::vec2() {
			x = 0.0f;
			y = 0.0f;
		}

		vec2::vec2(const float x, const float y) {
			this->x = x;
			this->y = y;
		}

		float vec2::normal() {
			return sqrt(x * x + y * y);
		}

		vec2& vec2::normalize() {
			x /= normal();
			y /= normal();

			return (*this);
		}

		float vec2::dot(const vec2 &other) {
			return (x * other.x) + (y * other.y);
		}

		vec2& vec2::add(const vec2 &other) {
			x += other.x;
			y += other.y;

			return (*this);
		}

		vec2& vec2::subtract(const vec2 &other) {
			x -= other.x;
			y -= other.y;

			return (*this);
		}

		vec2& vec2::multiply(const vec2 &other) {
			x *= other.x;
			y *= other.y;

			return (*this);
		}

		vec2& vec2::scale(const float scale) {
			x *= scale;
			y *= scale;

			return (*this);
		}

		vec2& vec2::divide(const vec2 &other) {
			x /= other.x;
			y /= other.y;

			return (*this);
		}

		vec2 operator+(vec2 left, const vec2 &right) {
			return left.add(right);
		}

		vec2 operator-(vec2 left, const vec2 &right) {
			return left.subtract(right);
		}

		vec2 operator*(vec2 left, const vec2 &right) {
			return left.multiply(right);
		}

		vec2 operator/(vec2 left, const vec2 &right) {
			return left.divide(right);
		}

		bool vec2::operator==(const vec2 &other) {
			return x == other.x && y == other.y;
		}

		bool vec2::operator!=(const vec2 &other) {
			return (*this) == other;
		}

		vec2& vec2::operator+=(const vec2 &other) {
			return add(other);
		}

		vec2& vec2::operator-=(const vec2 &other) {
			return subtract(other);
		}

		vec2& vec2::operator*=(const vec2 &other) {
			return multiply(other);
		}

		vec2& vec2::operator/=(const vec2 &other) {
			return divide(other);
		}

		std::ostream& operator<<(std::ostream &stream, const vec2 &vector) {
			stream << "vec2 coord: (" << vector.x << ", " << vector.y << ")";
			return stream;
		}

	}
}