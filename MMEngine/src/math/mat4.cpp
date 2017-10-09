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

		std::ostream& operator<<(std::ostream &stream, const mat4 &matrix) {
			stream << "column 1: " << matrix.columns[0] << std::endl;
			stream << "column 2: " << matrix.columns[1] << std::endl;
			stream << "column 3: " << matrix.columns[2] << std::endl;
			stream << "column 4: " << matrix.columns[3] << std::endl;

			return stream;
		}

	}
}