#include "shape_generator_3D.h"
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

namespace mme {
	namespace graphics {

		Shape ShapeGenerator3D::makeCube() {

			Shape cube;
			cube.clean = false;

			VertexC vertices[] = {
				// Vertex 0, Front
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
				mme::math::vec3(0.0f, 0.0f, 1.0f),

				// Vertx 4, Left
				mme::math::vec3(-0.5f, -0.5f, -1.0f),
				mme::math::vec3(1.0f, 1.0f, 0.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 5
				mme::math::vec3(-0.5f, 0.5f, -1.0f),
				mme::math::vec3(0.0f, 1.0f, 1.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 6
				mme::math::vec3(-0.5f, 0.5f, 0.0f),
				mme::math::vec3(0.0f, 1.0f, 0.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 7
				mme::math::vec3(-0.5f, -0.5f, 0.0f),
				mme::math::vec3(1.0f, 0.0f, 0.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),

				// Vertex 8, Right
				mme::math::vec3(0.5f, -0.5f, 0.0f),
				mme::math::vec3(0.5f, 0.0f, 0.5f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 9
				mme::math::vec3(0.5f, 0.5f, 0.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 10
				mme::math::vec3(0.5f, 0.5f, -1.0f),
				mme::math::vec3(1.0f, 0.0f, 1.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 11
				mme::math::vec3(0.5f, -0.5f, -1.0f),
				mme::math::vec3(0.0f, 0.5f, 0.5f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),

				// Vertex 12, Top
				mme::math::vec3(-0.5f, 0.5f, 0.0f),
				mme::math::vec3(0.0f, 1.0f, 0.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 13
				mme::math::vec3(-0.5f, 0.5f, -1.0f),
				mme::math::vec3(0.0f, 1.0f, 1.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 14
				mme::math::vec3(0.5f, 0.5f, -1.0f),
				mme::math::vec3(1.0f, 0.0f, 1.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 15
				mme::math::vec3(0.5f, 0.5f, 0.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),

				// Vertex 16, Bottom
				mme::math::vec3(-0.5f, -0.5f, -1.0f),
				mme::math::vec3(1.0f, 1.0f, 0.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 17
				mme::math::vec3(-0.5f, -0.5f, 0.0f),
				mme::math::vec3(1.0f, 0.0f, 0.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 18
				mme::math::vec3(0.5f, -0.5f, 0.0f),
				mme::math::vec3(0.5f, 0.0f, 0.5f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 19
				mme::math::vec3(0.5f, -0.5f, -1.0f),
				mme::math::vec3(0.0f, 0.5f, 0.5f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),

				// Vertex 20, Back
				mme::math::vec3(-0.5f, -0.5f, -1.0f),
				mme::math::vec3(1.0f, 1.0f, 0.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 21
				mme::math::vec3(-0.5f, 0.5f, -1.0f),
				mme::math::vec3(0.0f, 1.0f, 1.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 22
				mme::math::vec3(0.5f, 0.5f, -1.0f),
				mme::math::vec3(1.0f, 0.0f, 1.0f),
				mme::math::vec3(0.0f, 0.0f, 1.0f),
				// Vertex 23
				mme::math::vec3(0.5f, -0.5f, -1.0f),
				mme::math::vec3(0.0f, 0.5f, 0.5f),
				mme::math::vec3(0.0f, 0.0f, 1.0f)
			};

			cube.num_vertices = NUM_ARRAY_ELEMENTS(vertices);		// set number of vertices
			cube.vertices = new VertexC[cube.num_vertices];
			memcpy(cube.vertices, vertices, sizeof(vertices));	// copy vertices into the shape

			GLuint indices[] = {
				// Front
				0, 1, 2,
				2, 3, 0,
				// Left
				4, 5, 6,
				6, 7, 4,
				// Right
				8, 9, 10,
				10, 11, 8,
				// Top
				12, 13, 14,
				14, 15, 12,
				// Bottom
				16, 17, 18,
				18, 19, 16,
				// Back
				20, 21, 22,
				22, 23, 20
			};

			cube.num_indices = NUM_ARRAY_ELEMENTS(indices);
			cube.indices = new GLuint[cube.num_indices];
			memcpy(cube.indices, indices, sizeof(indices));

			cube.col_box3D.max = &(cube.vertices[22].pos);
			cube.col_box3D.min = &(cube.vertices[0].pos);

			//cube.col_sphere.center = &(cube.pos);
			cube.col_sphere.radius = 0.5f;

			return cube;
		}

		Shape ShapeGenerator3D::makeBox() {

			Shape cube;
			cube.clean = false;

			VertexC vertices[] = {
	
			// Vertx 0, Left
			mme::math::vec3(-0.5f, -0.5f, -1.0f),
			mme::math::vec3(1.0f, 1.0f, 0.0f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),
			// Vertex 1
			mme::math::vec3(-0.5f, 0.5f, -1.0f),
			mme::math::vec3(0.0f, 1.0f, 1.0f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),
			// Vertex 2
			mme::math::vec3(-0.5f, 0.5f, 0.0f),
			mme::math::vec3(0.0f, 1.0f, 0.0f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),
			// Vertex 3
			mme::math::vec3(-0.5f, -0.5f, 0.0f),
			mme::math::vec3(1.0f, 0.0f, 0.0f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),

			// Vertex 4, Right
			mme::math::vec3(0.5f, -0.5f, 0.0f),
			mme::math::vec3(0.5f, 0.0f, 0.5f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),
			// Vertex 5
			mme::math::vec3(0.5f, 0.5f, 0.0f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),
			// Vertex 6
			mme::math::vec3(0.5f, 0.5f, -1.0f),
			mme::math::vec3(1.0f, 0.0f, 1.0f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),
			// Vertex 7
			mme::math::vec3(0.5f, -0.5f, -1.0f),
			mme::math::vec3(0.0f, 0.5f, 0.5f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),

			// Vertex 8, Top
			mme::math::vec3(-0.5f, 0.5f, 0.0f),
			mme::math::vec3(0.0f, 1.0f, 0.0f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),
			// Vertex 9
			mme::math::vec3(-0.5f, 0.5f, -1.0f),
			mme::math::vec3(0.0f, 1.0f, 1.0f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),
			// Vertex 10
			mme::math::vec3(0.5f, 0.5f, -1.0f),
			mme::math::vec3(1.0f, 0.0f, 1.0f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),
			// Vertex 11
			mme::math::vec3(0.5f, 0.5f, 0.0f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),

			// Vertex 12, Bottom
			mme::math::vec3(-0.5f, -0.5f, -1.0f),
			mme::math::vec3(1.0f, 1.0f, 0.0f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),
			// Vertex 13
			mme::math::vec3(-0.5f, -0.5f, 0.0f),
			mme::math::vec3(1.0f, 0.0f, 0.0f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),
			// Vertex 14
			mme::math::vec3(0.5f, -0.5f, 0.0f),
			mme::math::vec3(0.5f, 0.0f, 0.5f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),
			// Vertex 15
			mme::math::vec3(0.5f, -0.5f, -1.0f),
			mme::math::vec3(0.0f, 0.5f, 0.5f),
			mme::math::vec3(0.0f, 0.0f, 1.0f),

			};

			cube.num_vertices = NUM_ARRAY_ELEMENTS(vertices);		// set number of vertices
			cube.vertices = new VertexC[cube.num_vertices];
			memcpy(cube.vertices, vertices, sizeof(vertices));	// copy vertices into the shape

			GLuint indices[] = {
				// Left
				0, 1, 2,
				2, 3, 0,
				// Right
				4, 5, 6,
				6, 7, 4,
				// Top
				8, 9, 10,
				10, 11, 8,
				// Bottom
				12, 13, 14,
				14, 15, 12,
			};

			cube.num_indices = NUM_ARRAY_ELEMENTS(indices);
			cube.indices = new GLuint[cube.num_indices];
			memcpy(cube.indices, indices, sizeof(indices));

			return cube;
		}
	}
}