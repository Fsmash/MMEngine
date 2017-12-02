/*	Code by Bryant Dinh for Senior Sem 2017 Project : Misunderstood Magician Engine
	
	This class, ShapeRenderer, inherits from Renderer.
	It handles the submission of Shape data into a buffer and the flushing (drawing) of that data.
	Shape inherits from Renderable and are basic 2D and 3D shapes. 

*/

#pragma once
#include "renderer.h"
#include "objects/shape.h"

namespace mme {
	namespace graphics {

		class ShapeRenderer : public Renderer {

		private:

			GLuint m_numShapes;

		public:

			ShapeRenderer() : Renderer(), m_numShapes(0) {}
			ShapeRenderer(Shape *shapes, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx);
			ShapeRenderer(Shape &shapes);
			~ShapeRenderer();

			void submit(Renderable *shapes, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx) override;
			void submit(Renderable &shape) override;
			void flush() const override;
			void flushInstanced() const override;
			void clean() override;

		};
	}
}