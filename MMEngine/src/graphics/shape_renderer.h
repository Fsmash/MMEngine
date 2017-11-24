#pragma once
#include "renderer.h"
#include <vector>
#include "objects/shape.h"

namespace mme {
	namespace graphics {

		class ShapeRenderer : public Renderer {

		private:
				
			//std::vector<Renderable> m_mem;
			GLuint m_numShapes;
			GLuint m_numIndices;

		public:

			ShapeRenderer() : Renderer(), m_numShapes(0), m_numIndices(0) {}
			void submit(Shape *shapes, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx);
			void flush() const override;
			void clean() override;

		};
	}
}