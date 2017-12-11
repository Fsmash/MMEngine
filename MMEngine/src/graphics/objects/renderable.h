/*	Code by Bryant Dinh for Senior Sem 2017 Project : Misunderstood Magician Engine

	This struct, Renderable, is the base class for all "renderables". Structs that will hold interleaved attribute data in either arrays of
	Vertex_T or Vertex_S stucts. It will also hold useful information during the rendering process.
	
*/

#pragma once
#include "GL/glew.h"
#include <iostream>
#include "../../math/mat4.h"
#include "../../physics/bounding/collision_geometry.h"

namespace mme {
	namespace graphics {

		struct Renderable {

			GLuint num_vertices;	// number of vertices
			GLuint *indices;		// pointer to array of indices for index rendering
			GLuint num_indices;		// number of indices
			
			math::vec3 pos;
			math::vec3 vel;
			math::mat4 model_matrix;

			// Used for collision detection
			physics::Plane col_plane;
			physics::BoundingSphere col_sphere;
			physics::AABB2D col_box2D;
			physics::AABB3D col_box3D;

			Renderable() : num_vertices(0), indices(nullptr), num_indices(0) { 
				model_matrix = math::mat4::identity();
			}

			virtual ~Renderable() { }

			// Offsetting indices so data my be stored in one buffer
			void offsetIdx(GLuint offset) {

				if (indices != nullptr) {		
					for (int i = 0; i < num_indices; i++)
						indices[i] += offset;
				}
			
			}
			
		};
	}
}