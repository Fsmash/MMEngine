#include "shape_generator_2D.h"
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

namespace mme {
	namespace graphics {

		Shape ShapeGenerator2D::makeTriangle() {
			Shape triangle;
			// Array of vertex structs, (really just 3 vec3). vertex position, color and normal.
			VertexC vertices[] = {
				// Vertex 0
				mme::math::vec3(-0.5f, -0.5f, 0.0f),	// pos
				mme::math::vec3(1.0f, 0.0f, 0.0f),		// color
				mme::math::vec3(0.0f, 0.0f, 1.0f),		// normal
				// Vertex 1
				mme::math::vec3(0.0f, 0.5f, 0.0f),
				mme::math::vec3(0.0f, 1.0f, 0.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 2
				mme::math::vec3(0.5f, -0.5f, 0.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
			};

			triangle.num_vertices = NUM_ARRAY_ELEMENTS(vertices);		// set number of vertices
			triangle.vertices = new VertexC[triangle.num_vertices];
			memcpy(triangle.vertices, vertices, sizeof(vertices));	// copy vertices into the shape

			GLuint indices[] = {
				0, 1, 2
			};

			triangle.num_indices = NUM_ARRAY_ELEMENTS(indices);
			triangle.indices = new GLuint[triangle.num_indices];
			memcpy(triangle.indices, indices, sizeof(indices));

			return triangle;
		}

		Shape ShapeGenerator2D::makeSquare() {
			Shape square;
			// Array of vertex structs, (really just 3 vec3). vertex position, color and normal.
			VertexC vertices[] = {
				// Vertex 0
				mme::math::vec3(-0.5f, -0.5f, 0.0f),	// pos
				mme::math::vec3(1.0f, 0.0f, 0.0f),		// color
				mme::math::vec3(0.0f, 0.0f, 1.0f),		// normal
				// Vertex 1
				mme::math::vec3(-0.5f, 0.5f, 0.0f),
				mme::math::vec3(0.0f, 1.0f, 0.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 2
				mme::math::vec3(0.5f, 0.5f, 0.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 3
				mme::math::vec3(0.5f, -0.5f, 0.0f),
				mme::math::vec3(0.5f, 0.0f, 0.5f),
				mme::math::vec3(0.0f, 0.0f, 1.0f)
			};

			square.num_vertices = NUM_ARRAY_ELEMENTS(vertices);		// set number of vertices
			square.vertices = new VertexC[square.num_vertices];
			memcpy(square.vertices, vertices, sizeof(vertices));	// copy vertices into the shape

			GLuint indices[] = {
				0, 1, 2,
				2, 3, 0
			};

			square.num_indices = NUM_ARRAY_ELEMENTS(indices);
			square.indices = new GLuint[square.num_indices];
			memcpy(square.indices, indices, sizeof(indices));

			return square;
		}
	}
}