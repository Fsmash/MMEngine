/*	Code by Daniel Medina for Senior Sem 2017 Project : Misunderstood Magician Engine

This model struct inherits from renderable and vertex_t and loads data from a model file to be rendered. This class utilizes Assimp library in order to support a large variety of model file types. 

supported file types can be found: http://assimp.org/main_features_formats.html
*/

#pragma once
#include "renderable.h"
#include "vertex_t.h"

namespace mme {
	namespace graphics {

		struct Model: public Renderable {
			
			VertexT *vertices;
			bool interleaved = false;
			const char *filePath;
			const char *texFilePath;
			
			// Default constructor
			Model() : Renderable(), vertices(nullptr), filePath(nullptr), texFilePath(nullptr) { }
			
			// Constructor
			Model(const char* file_name);

			// load mesh/model data from file
			bool Interleaved();

			// load texture from file
			void loadTexture(const char* tex_file);

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
		};
	}
}