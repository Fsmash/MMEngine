#pragma once

#include "vertex_c.h"

namespace mme {
	namespace graphics {

		struct Shape {

			Shape() : vertices(0), num_vertices(0), indices(0), num_indices(0) {}

			VertexC *vertices;
			GLuint num_vertices;
			GLuint *indices;
			GLuint num_indices;

			GLsizeiptr vertexBufferSize() const {
				return num_vertices * sizeof(VertexC);
			}

			GLsizeiptr indexBufferSize() const {
				return num_indices * sizeof(GLuint);
			}

			void cleanUp() {
				delete[] vertices;
				delete[] indices;
				num_vertices = num_indices = 0;
			}
		};
	}
}