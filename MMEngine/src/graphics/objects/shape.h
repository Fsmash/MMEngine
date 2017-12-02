/*	Code by Bryant Dinh for Senior Sem 2017 Project : Misunderstood Magician Engine

	This struct, Shape, inerits from Renderable. This "renderable" will hold an array of VertexC vertices that will interleave attribute data for 
	basic 2D and 3D shapes. It also has useful member functions to perform operations on the vertices such as updating positions with translation,
	scaling, or rotation operations. 

*/

#pragma once
#include "renderable.h"
#include "vertex_c.h"

namespace mme {
	namespace graphics {

		struct Shape : public Renderable {

			VertexC *vertices;	// pointer to array of VertexC vertices
			bool clean;			// whether or not to clean data with destructor

			// Default constructor
			Shape() : Renderable(), vertices(nullptr), clean(true) { }

			// Destructor that frees up memory if clean is true
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
			
			// TO DO: have vertex data loaded in from file
			//void loader(const char *file);

			// Returns vertex buffer size
			GLsizeiptr vertexBufferSize() const {
				return num_vertices * sizeof(VertexC);
			}

			// Returns index buffer size
			GLsizeiptr indexBufferSize() const {
				return num_indices * sizeof(GLuint);
			}

			// Updates position attribute of vertex data
			void updatePos(const float x, const float y, const float z) {
				if (vertices == nullptr) return;
				for (int i = 0; i < num_vertices; i++) {
					vertices[i].updatePos(x, y, z);
				}
			}

			// Free up memory by deleting allocated memory
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