#pragma once
#include "GL/glew.h"
#include <iostream>

namespace mme {
	namespace graphics {

		struct Renderable {

			GLuint num_vertices;
			GLuint *indices;
			GLuint num_indices;

			Renderable() : num_vertices(0), indices(nullptr), num_indices(0) { }

			virtual ~Renderable() { 
				std::cout << "virtual destructor called" << std::endl;
			}

			void offsetIdx(GLuint offset) {

				if (indices != nullptr) {		
					for (int i = 0; i < num_indices; i++)
						indices[i] += offset;
				}
			
			}
			
		};
	}
}