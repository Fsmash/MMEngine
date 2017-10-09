#pragma once
#include "vec4.h"

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

			static mat4 identity();
			/*
			static mat4 translationMatrix(const vec3 &vec);
			static mat4 rotationMatrix(const float angle, const vec3 &vec);
			static mat4 scalingMatrix(const vec3 &vec);


			mat4& translate(const vec3 &vec);
			mat4& translate(const float &x, const float &y, const float &z);
			mat4& scale(const vec3 &vec);
			mat4& scale(const float &x, const float &y, const float &z);
			mat4& rotatate(const float angle, const vec3 &vec);
			mat4& rotatate(const float angle, const float x, const float y, const float z);
			*/
			//mat4& multiplication(const mat4 &other);
			//mat4& orthographic();
			//mat4& perspective();

			//friend mat4& operator*(mat4 left, const mat4 &right);
			
			friend std::ostream& operator<<(std::ostream &stream, const mat4 &matrix);

		};

	}
}