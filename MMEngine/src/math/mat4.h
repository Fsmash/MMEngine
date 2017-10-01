#pragma once
#include "vec4.h"

namespace mme {
	namespace math {

		struct mat4 {

			union {
				float matrix[16];
				vec4 columns[4];
			};

			mat4();
			mat4(const vec4 &col1, const vec4 &col2, const vec4 &col3, const vec4 &col4);
			mat4(const float &diagonal);

			mat4& translate(const vec3 &vec);
			mat4& translate(const float &x, const float &y, const float &z);

			mat4& scaling(const vec3 &vec);
			mat4& scaling(const float &x, const float &y, const float &z);

			mat4& rotation(const vec3 &vec);
			mat4& rotation(const float &x, const float &y, const float &z);


		};

	}
}