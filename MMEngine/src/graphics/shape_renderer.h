/*	Code by Bryant Dinh for Senior Sem 2017 Project : Misunderstood Magician Engine
	
	This class, ShapeRenderer, inherits from Renderer.
	It handles the submission of Shape data into a buffer and the flushing (drawing) of that data.
	Must Override (have own implementation) of all the virtual functions in Renderer.
	Shape inherits from Renderable and are basic 2D and 3D shapes. 
	Default Shader initialization and use assumes/enforces that shader files be located in "src/shaders"
	and be named as defined. Also assumes names for uniforms named exactly as accessed in ShapeRenderer. 

*/

#pragma once
#include "renderer.h"
#include "objects/shape.h"

namespace mme {
	namespace graphics {

		class ShapeRenderer : public Renderer {

		private:

			GLuint m_numShapes;	// number of shape objects being rendered
			Shape *m_ptr;

		public:

			ShapeRenderer() : Renderer(), m_numShapes(0), m_ptr(nullptr) { }
			ShapeRenderer(Shape *shapes, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx);
			ShapeRenderer(Shape &shapes);
			~ShapeRenderer();

			void submit(Shape *shapes, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx);
			void submit(Shape &shape);
			void add(Shape *shapes, const GLuint num);
			void flush() const override;
			void flushInstanced() const override;
			void flushDynamic(const char *uniform) const;
			void clean() override;
			
		};
	}
}