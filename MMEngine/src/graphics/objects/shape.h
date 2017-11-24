#pragma once
#include "renderable.h"
#include "vertex_c.h"

namespace mme {
	namespace graphics {

		struct Shape : public Renderable {

			VertexC *vertices;
			bool clean;

			Shape() : Renderable(), vertices(nullptr), clean(true) { }

			~Shape() {

				if (clean) {
										
					if (vertices != nullptr) {
						delete[] vertices;
						vertices = nullptr;
					}

					if (indices != nullptr) {
						delete[] indices;
						indices = nullptr;
					}
				}

				num_vertices = num_indices = 0;
			}
			
			//void loader(const char *file);

			GLsizeiptr vertexBufferSize() const {
				return num_vertices * sizeof(VertexC);
			}

			GLsizeiptr indexBufferSize() const {
				return num_indices * sizeof(GLuint);
			}

			void updatePos(const float x, const float y, const float z) {
				if (vertices == nullptr) return;
				for (int i = 0; i < num_vertices; i++) {
					vertices[i].updatePos(x, y, z);
				}
			}

			void cleanUp() {

				if (vertices != nullptr) {
					delete[] vertices;
					vertices = nullptr;
				}

				if (indices != nullptr) {
					delete[] indices;
					indices = nullptr;
				}

				num_vertices = num_indices = 0;
			}

		};
	}
}