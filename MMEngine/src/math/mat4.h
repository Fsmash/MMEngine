#pragma once
#include "vec4.h"
#include <math.h>
#define RADIANS (3.14159265f / 180.0f)

namespace mme {
	namespace math {

		struct mat4 {

			union {	// share the same memory location. should be same size as vec4 is just 4 floats.
				float matrix[16];
				vec4 columns[4];
			};

			mat4();
			mat4(const vec4 col1, const vec4 col2, const vec4 col3, const vec4 col4);
			mat4(const float diagonal);
			mat4(float a, float b, float c, float d, float e, float f, float g, float h,
				 float i, float j, float k, float l, float m, float n, float o, float p);

			//mat4& invert();
			mat4& multiply(const mat4 &other);
			mat4& operator*=(const mat4 &other);
			
			static float determinant(const mat4 &mm);
			static mat4 identity();
			static mat4 translationMatrix(const float x, const float y, const float z);
			static mat4 rotationMatrixX(const float angle);
			static mat4 rotationMatrixY(const float angle);
			static mat4 rotationMatrixZ(const float angle);
			static mat4 scalingMatrix(const float x, const float y, const float z);
			static mat4 inverseMatrix(const mat4 &mm);
		
			friend mat4 operator*(mat4 left, const mat4 &right);
			friend vec4 operator*(mat4 left, const vec4 &right);
			friend std::ostream& operator<<(std::ostream &stream, const mat4 &matrix);

		};

	}
}