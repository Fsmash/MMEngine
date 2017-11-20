#pragma once

#include "GL/glew.h"
#include "../../math/vec3.h"

namespace mme {
	namespace graphics {

		struct VertexC {
			mme::math::vec3 pos;
			mme::math::vec3 color;
			mme::math::vec3 normal;

			static GLsizeiptr vertexSize() {
				return sizeof(float) * 9;
			}

			static const void* offset1() {
				return (const void *)(sizeof(float) * 3);
			}

			static const void* offset2() {
				return (const void *)(sizeof(float) * 6);
			}
			
			static void components(GLuint &a1, GLuint &a2, GLuint &a3) {
				a1 = 3;
				a2 = 3;
				a3 = 3;
			}

		};

	}
}