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

		mat4::mat4(float a, float b, float c, float d, float e, float f, float g, float h,
			float i, float j, float k, float l, float m, float n, float o, float p) {
			matrix[0] = a;
			matrix[1] = b;
			matrix[2] = c;
			matrix[3] = d;
			matrix[4] = e;
			matrix[5] = f;
			matrix[6] = g;
			matrix[7] = h;
			matrix[8] = i;
			matrix[9] = j;
			matrix[10] = k;
			matrix[11] = l;
			matrix[12] = m;
			matrix[13] = n;
			matrix[14] = o;
			matrix[15] = p;
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

		void mat4::incrementX(float inc) {
			columns[3].x += inc;
		}

		void mat4::incrementY(float inc) {
			columns[3].y += inc;
		}

		void mat4::incrementZ(float inc) {
			columns[3].z += inc;
		}

		void mat4::rotationX(float angle) {
			matrix[5] = cos(angle * RADIANS);
			matrix[6] = sin(angle * RADIANS);
			matrix[9] = -sin(angle * RADIANS);
			matrix[10] = cos(angle * RADIANS);
		}
		
		void mat4::rotationY(float angle) {
			matrix[0] = cos(angle * RADIANS);
			matrix[2] = -sin(angle * RADIANS);
			matrix[8] = sin(angle * RADIANS);
			matrix[10] = cos(angle * RADIANS);
		}
		
		void mat4::rotationZ(float angle) {
			matrix[0] = cos(angle * RADIANS);
			matrix[1] = sin(angle * RADIANS);
			matrix[4] = -sin(angle * RADIANS);
			matrix[5] = cos(angle * RADIANS);
		}

		void mat4::scale(float x, float y, float z) {
			matrix[0] = x;
			matrix[5] = y;
			matrix[10] = z;
		}

		float mat4::determinant(const mat4 &mm) {
			return mm.matrix[12] * mm.matrix[9] * mm.matrix[6] * mm.matrix[3] -
				mm.matrix[8] * mm.matrix[13] * mm.matrix[6] * mm.matrix[3] -
				mm.matrix[12] * mm.matrix[5] * mm.matrix[10] * mm.matrix[3] +
				mm.matrix[4] * mm.matrix[13] * mm.matrix[10] * mm.matrix[3] +
				mm.matrix[8] * mm.matrix[5] * mm.matrix[14] * mm.matrix[3] -
				mm.matrix[4] * mm.matrix[9] * mm.matrix[14] * mm.matrix[3] -
				mm.matrix[12] * mm.matrix[9] * mm.matrix[2] * mm.matrix[7] +
				mm.matrix[8] * mm.matrix[13] * mm.matrix[2] * mm.matrix[7] +
				mm.matrix[12] * mm.matrix[1] * mm.matrix[10] * mm.matrix[7] -
				mm.matrix[0] * mm.matrix[13] * mm.matrix[10] * mm.matrix[7] -
				mm.matrix[8] * mm.matrix[1] * mm.matrix[14] * mm.matrix[7] +
				mm.matrix[0] * mm.matrix[9] * mm.matrix[14] * mm.matrix[7] +
				mm.matrix[12] * mm.matrix[5] * mm.matrix[2] * mm.matrix[11] -
				mm.matrix[4] * mm.matrix[13] * mm.matrix[2] * mm.matrix[11] -
				mm.matrix[12] * mm.matrix[1] * mm.matrix[6] * mm.matrix[11] +
				mm.matrix[0] * mm.matrix[13] * mm.matrix[6] * mm.matrix[11] +
				mm.matrix[4] * mm.matrix[1] * mm.matrix[14] * mm.matrix[11] -
				mm.matrix[0] * mm.matrix[5] * mm.matrix[14] * mm.matrix[11] -
				mm.matrix[8] * mm.matrix[5] * mm.matrix[2] * mm.matrix[15] +
				mm.matrix[4] * mm.matrix[9] * mm.matrix[2] * mm.matrix[15] +
				mm.matrix[8] * mm.matrix[1] * mm.matrix[6] * mm.matrix[15] -
				mm.matrix[0] * mm.matrix[9] * mm.matrix[6] * mm.matrix[15] -
				mm.matrix[4] * mm.matrix[1] * mm.matrix[10] * mm.matrix[15] +
				mm.matrix[0] * mm.matrix[5] * mm.matrix[10] * mm.matrix[15];
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

		
		mat4 mat4::inverseMatrix(const mat4 &mm) {
			
			float det = determinant(mm);
			
			if (0.0f == det) {
				fprintf(stderr, "WARNING. matrix has no determinant. can not invert\n");
				return mm;
			}
			
			float inv_det = 1.0f / det;

			return mat4(
				inv_det * (mm.matrix[9] * mm.matrix[14] * mm.matrix[7] - mm.matrix[13] * mm.matrix[10] * mm.matrix[7] +
					mm.matrix[13] * mm.matrix[6] * mm.matrix[11] - mm.matrix[5] * mm.matrix[14] * mm.matrix[11] -
					mm.matrix[9] * mm.matrix[6] * mm.matrix[15] + mm.matrix[5] * mm.matrix[10] * mm.matrix[15]),
				inv_det * (mm.matrix[13] * mm.matrix[10] * mm.matrix[3] - mm.matrix[9] * mm.matrix[14] * mm.matrix[3] -
					mm.matrix[13] * mm.matrix[2] * mm.matrix[11] + mm.matrix[1] * mm.matrix[14] * mm.matrix[11] +
					mm.matrix[9] * mm.matrix[2] * mm.matrix[15] - mm.matrix[1] * mm.matrix[10] * mm.matrix[15]),
				inv_det * (mm.matrix[5] * mm.matrix[14] * mm.matrix[3] - mm.matrix[13] * mm.matrix[6] * mm.matrix[3] +
					mm.matrix[13] * mm.matrix[2] * mm.matrix[7] - mm.matrix[1] * mm.matrix[14] * mm.matrix[7] -
					mm.matrix[5] * mm.matrix[2] * mm.matrix[15] + mm.matrix[1] * mm.matrix[6] * mm.matrix[15]),
				inv_det * (mm.matrix[9] * mm.matrix[6] * mm.matrix[3] - mm.matrix[5] * mm.matrix[10] * mm.matrix[3] -
					mm.matrix[9] * mm.matrix[2] * mm.matrix[7] + mm.matrix[1] * mm.matrix[10] * mm.matrix[7] +
					mm.matrix[5] * mm.matrix[2] * mm.matrix[11] - mm.matrix[1] * mm.matrix[6] * mm.matrix[11]),
				inv_det * (mm.matrix[12] * mm.matrix[10] * mm.matrix[7] - mm.matrix[8] * mm.matrix[14] * mm.matrix[7] -
					mm.matrix[12] * mm.matrix[6] * mm.matrix[11] + mm.matrix[4] * mm.matrix[14] * mm.matrix[11] +
					mm.matrix[8] * mm.matrix[6] * mm.matrix[15] - mm.matrix[4] * mm.matrix[10] * mm.matrix[15]),
				inv_det * (mm.matrix[8] * mm.matrix[14] * mm.matrix[3] - mm.matrix[12] * mm.matrix[10] * mm.matrix[3] +
					mm.matrix[12] * mm.matrix[2] * mm.matrix[11] - mm.matrix[0] * mm.matrix[14] * mm.matrix[11] -
					mm.matrix[8] * mm.matrix[2] * mm.matrix[15] + mm.matrix[0] * mm.matrix[10] * mm.matrix[15]),
				inv_det * (mm.matrix[12] * mm.matrix[6] * mm.matrix[3] - mm.matrix[4] * mm.matrix[14] * mm.matrix[3] -
					mm.matrix[12] * mm.matrix[2] * mm.matrix[7] + mm.matrix[0] * mm.matrix[14] * mm.matrix[7] +
					mm.matrix[4] * mm.matrix[2] * mm.matrix[15] - mm.matrix[0] * mm.matrix[6] * mm.matrix[15]),
				inv_det * (mm.matrix[4] * mm.matrix[10] * mm.matrix[3] - mm.matrix[8] * mm.matrix[6] * mm.matrix[3] +
					mm.matrix[8] * mm.matrix[2] * mm.matrix[7] - mm.matrix[0] * mm.matrix[10] * mm.matrix[7] -
					mm.matrix[4] * mm.matrix[2] * mm.matrix[11] + mm.matrix[0] * mm.matrix[6] * mm.matrix[11]),
				inv_det * (mm.matrix[8] * mm.matrix[13] * mm.matrix[7] - mm.matrix[12] * mm.matrix[9] * mm.matrix[7] +
					mm.matrix[12] * mm.matrix[5] * mm.matrix[11] - mm.matrix[4] * mm.matrix[13] * mm.matrix[11] -
					mm.matrix[8] * mm.matrix[5] * mm.matrix[15] + mm.matrix[4] * mm.matrix[9] * mm.matrix[15]),
				inv_det * (mm.matrix[12] * mm.matrix[9] * mm.matrix[3] - mm.matrix[8] * mm.matrix[13] * mm.matrix[3] -
					mm.matrix[12] * mm.matrix[1] * mm.matrix[11] + mm.matrix[0] * mm.matrix[13] * mm.matrix[11] +
					mm.matrix[8] * mm.matrix[1] * mm.matrix[15] - mm.matrix[0] * mm.matrix[9] * mm.matrix[15]),
				inv_det * (mm.matrix[4] * mm.matrix[13] * mm.matrix[3] - mm.matrix[12] * mm.matrix[5] * mm.matrix[3] +
					mm.matrix[12] * mm.matrix[1] * mm.matrix[7] - mm.matrix[0] * mm.matrix[13] * mm.matrix[7] -
					mm.matrix[4] * mm.matrix[1] * mm.matrix[15] + mm.matrix[0] * mm.matrix[5] * mm.matrix[15]),
				inv_det * (mm.matrix[8] * mm.matrix[5] * mm.matrix[3] - mm.matrix[4] * mm.matrix[9] * mm.matrix[3] -
					mm.matrix[8] * mm.matrix[1] * mm.matrix[7] + mm.matrix[0] * mm.matrix[9] * mm.matrix[7] +
					mm.matrix[4] * mm.matrix[1] * mm.matrix[11] - mm.matrix[0] * mm.matrix[5] * mm.matrix[11]),
				inv_det * (mm.matrix[12] * mm.matrix[9] * mm.matrix[6] - mm.matrix[8] * mm.matrix[13] * mm.matrix[6] -
					mm.matrix[12] * mm.matrix[5] * mm.matrix[10] + mm.matrix[4] * mm.matrix[13] * mm.matrix[10] +
					mm.matrix[8] * mm.matrix[5] * mm.matrix[14] - mm.matrix[4] * mm.matrix[9] * mm.matrix[14]),
				inv_det * (mm.matrix[8] * mm.matrix[13] * mm.matrix[2] - mm.matrix[12] * mm.matrix[9] * mm.matrix[2] +
					mm.matrix[12] * mm.matrix[1] * mm.matrix[10] - mm.matrix[0] * mm.matrix[13] * mm.matrix[10] -
					mm.matrix[8] * mm.matrix[1] * mm.matrix[14] + mm.matrix[0] * mm.matrix[9] * mm.matrix[14]),
				inv_det * (mm.matrix[12] * mm.matrix[5] * mm.matrix[2] - mm.matrix[4] * mm.matrix[13] * mm.matrix[2] -
					mm.matrix[12] * mm.matrix[1] * mm.matrix[6] + mm.matrix[0] * mm.matrix[13] * mm.matrix[6] +
					mm.matrix[4] * mm.matrix[1] * mm.matrix[14] - mm.matrix[0] * mm.matrix[5] * mm.matrix[14]),
				inv_det * (mm.matrix[4] * mm.matrix[9] * mm.matrix[2] - mm.matrix[8] * mm.matrix[5] * mm.matrix[2] +
					mm.matrix[8] * mm.matrix[1] * mm.matrix[6] - mm.matrix[0] * mm.matrix[9] * mm.matrix[6] -
					mm.matrix[4] * mm.matrix[1] * mm.matrix[10] + mm.matrix[0] * mm.matrix[5] * mm.matrix[10]));
		}

		mat4 operator*(mat4 left, const mat4 &right) {
			return left.multiply(right);
		}

		vec4 operator*(mat4 left, const vec4 &right) {
			vec4 col1 = left.columns[0].scale(right.x);
			vec4 col2 = left.columns[1].scale(right.y);
			vec4 col3 = left.columns[2].scale(right.z);
			vec4 col4 = left.columns[3].scale(right.w);

			float x = col1.x + col2.x + col3.x + col4.x;
			float y = col1.y + col2.y + col3.y + col4.y;
			float z = col1.z + col2.z + col3.z + col4.z;
			float w = col1.w + col2.w + col3.w + col4.w;
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