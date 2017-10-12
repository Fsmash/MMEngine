#include "mat4.h"

namespace mme {
	namespace math {

		mat4::mat4() {
			for (int i = 0; i < 16; i++) {
				matrix[i] = 0.0f;
			}
		}

		mat4::mat4(const vec4 col1, const vec4 col2, const vec4 col3, const vec4 col4) {
			columns[0] = col1;
			columns[1] = col2;
			columns[2] = col3;
			columns[3] = col4;
		}

		mat4::mat4(const float diagonal) {
			for (int i = 0; i < 16; i++) {
				matrix[i] = 0.0f;
			}

			matrix[0] = diagonal;
			matrix[5] = diagonal;
			matrix[10] = diagonal;
			matrix[15] = diagonal;
		}

		mat4 mat4::identity() {
			return mat4(1.0f);	// explicity calling constructor and returning a copy. not returning by reference.
		}

		mat4 mat4::translationMatrix(const float x, const float y, const float z) {
			mat4 tmp = identity();
			tmp.columns[3].x = x;
			tmp.columns[3].y = y;
			tmp.columns[3].z = z;
			return tmp;
		}
		
		mat4 mat4::rotationMatrixX(const float angle) {
			mat4 tmp = identity();
			tmp.matrix[5] = cos(angle * RADIANS);
			tmp.matrix[6] = sin(angle * RADIANS);
			tmp.matrix[9] = -sin(angle * RADIANS);
			tmp.matrix[10] = cos(angle * RADIANS);
			return tmp;
		}

		mat4 mat4::rotationMatrixY(const float angle) {
			mat4 tmp = identity();
			tmp.matrix[0] = cos(angle * RADIANS);
			tmp.matrix[2] = -sin(angle * RADIANS);
			tmp.matrix[8] = sin(angle * RADIANS);
			tmp.matrix[10] = cos(angle * RADIANS);
			return tmp;
		}

		mat4 mat4::rotationMatrixZ(const float angle) {
			mat4 tmp = identity();
			tmp.matrix[0] = cos(angle * RADIANS);
			tmp.matrix[1] = sin(angle * RADIANS);
			tmp.matrix[4] = -sin(angle * RADIANS);
			tmp.matrix[5] = cos(angle * RADIANS);
			return tmp;
		}

		mat4 mat4::scalingMatrix(const float x, const float y, const float z) {
			mat4 tmp = identity();
			tmp.matrix[0] = x;
			tmp.matrix[5] = y;
			tmp.matrix[10] = z;
			return tmp;
		}

		mat4& mat4::multiply(const mat4 &other) {
			float data[16];
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					float sum = 0.0f;
					for (int e = 0; e < 4; e++)
					{
						sum += matrix[x + e * 4] * other.matrix[e + y * 4];
					}
					data[x + y * 4] = sum;
				}
			}
			memcpy(matrix, data, 16 * sizeof(float));

			return *this;
		}

		mat4& mat4::operator*=(const mat4 &other) {
			return multiply(other);
		}

		mat4 operator*(mat4 left, const mat4 &right) {
			return left.multiply(right);
		}

		vec4 operator*(mat4 left, const vec4 &right) {
			float x = left.columns[0].scale(right.x).sum();
			float y = left.columns[1].scale(right.y).sum();
			float z = left.columns[2].scale(right.z).sum();
			float w = left.columns[3].scale(right.w).sum();
			return vec4(x, y, z, w);
		}

		std::ostream& operator<<(std::ostream &stream, const mat4 &matrix) {
			stream << "column 1: " << matrix.columns[0] << std::endl;
			stream << "column 2: " << matrix.columns[1] << std::endl;
			stream << "column 3: " << matrix.columns[2] << std::endl;
			stream << "column 4: " << matrix.columns[3] << std::endl;

			return stream;
		}

	}
}