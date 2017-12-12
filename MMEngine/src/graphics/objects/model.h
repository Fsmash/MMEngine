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
			void updateVertices(const float x, const float y, const float z) {
				if (vertices == nullptr) return;
				for (int i = 0; i < num_vertices; i++) {
					vertices[i].updatePos(x, y, z);
				}
			}

			void rotateVertices(const float x, const float y, const float z) {

				if (vertices == nullptr) return;

				math::mat4 rot = math::mat4::identity();
				math::vec4 vec;

				if (x != 0) {
					rot = rot * math::mat4::rotationMatrixX(x);
				}

				if (y != 0) {
					rot = rot * math::mat4::rotationMatrixY(y);
				}

				if (z != 0) {
					rot = rot * math::mat4::rotationMatrixZ(z);
				}

				for (int i = 0; i < num_vertices; i++) {
					vec = rot * math::vec4(vertices[i].pos, 1);
					vertices[i].pos = math::vec3(vec.x, vec.y, vec.z);
					vec = rot * math::vec4(vertices[i].normal, 1);
					vertices[i].normal = math::vec3(vec.x, vec.y, vec.z);
				}

			}

			void scaleVertices(const float x, const float y, const float z) {

				if (vertices == nullptr) return;

				math::mat4 scale = math::mat4::scalingMatrix(x, y, z);
				math::vec4 vec;

				for (int i = 0; i < num_vertices; i++) {
					vec = scale * math::vec4(vertices[i].pos, 1);
					vertices[i].pos = math::vec3(vec.x, vec.y, vec.z);
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

			math::vec3 getCenter();
		};
	}
}