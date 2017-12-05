/*	Code by Daniel Meidna for Senior Sem 2017 Project : Misunderstood Magician Engine

This struct, Shape, inerits from Renderable. This "renderable" will hold an array of VertexC vertices that will interleave attribute data for
basic 2D and 3D shapes. It also has useful member functions to perform operations on the vertices such as updating positions with translation,
scaling, or rotation operations.

This class Model inherits from
*/

#pragma once
#include "renderable.h"
#include "vertex_t.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace mme {
	namespace graphics {

		struct Model: public Renderable {

			VertexT *vertices;
			bool clean;

		public:

			
			// Default constructor
			Model() : Renderable(), vertices(nullptr), clean(true) { }
			
			// Constructor
			Model(const char* file_name);

			// Destructor that frees up memory if clean is true
			~Model();

			// load mesh/model data from file
			bool loadModelFile(const char* file_name);

			// TO DO: have vertex data loaded in from file
			//void loader(const char *file);

			// Returns vertex buffer size
			GLsizeiptr vertexBufferSize() const {
				return num_vertices * sizeof(VertexT);
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
			
			//void bufferModel();
			//void flush();
			//void clean();
		};

		//bool loadMesh(const char *file_name, GLuint *vao, int *point_count);

	}
}