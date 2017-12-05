#pragma once

#include "GL/glew.h"
#include "../../math/vec3.h"

namespace mme {
	namespace graphics {

		struct VertexT {

			mme::math::vec3 pos;
			mme::math::vec2 uv;
			mme::math::vec3 normal;

			void updatePos(const float x, const float y, const float z) {
				pos.add(mme::math::vec3(x, y, z));
			}

			static GLsizeiptr vertexSize() {
				return sizeof(float) * 8;
			}

			static const void* offset1() {
				return (const void *)(sizeof(float) * 3);
			}

			static const void* offset2() {
				return (const void *)(sizeof(float) * 5);
			}

			static void components(GLuint &a1, GLuint &a2, GLuint &a3) {
				a1 = 3;
				a2 = 2;
				a3 = 3;
			}

		};
	}
}