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

			// Default constructor
			Shape() : Renderable(), vertices(nullptr) { }
			
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
			void updateVertices(const float x, const float y, const float z) {

				if (vertices == nullptr) return;

				pos.x = x;
				pos.y = y;
				pos.z = z;

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

			void setColor(const float r, const float g, const float b) {
				
				for (int i = 0; i < num_vertices; i++)
					vertices[i].color = math::vec3(r, g, b);

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