#pragma once
#include "renderer.h"
#include "objects/shape.h"
#include "../math/mat4.h"

namespace mme {
	namespace graphics {

		class ShapeRenderer : public Renderer {

		private:

			//std::vector<Renderable> m_mem;
			GLuint m_numShapes;
			GLuint m_numInstances;
			GLuint m_matrixID;
			GLint m_matAttribLoc;
			GLsizeiptr m_matBufSz;
			GLsizeiptr m_bufOffset[3];

		public:

			ShapeRenderer() : Renderer(), m_numShapes(0), m_numInstances(0), m_matrixID(0), m_matAttribLoc(0), m_matBufSz(0) {
				m_bufOffset[0] = m_bufOffset[1] = m_bufOffset[2] = 0;
			}
			ShapeRenderer(Shape *shapes, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx);
			ShapeRenderer(Shape &shapes);
			~ShapeRenderer();
			void submit(Shape *shapes, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx);
			void submit(const Shape &shape);
			void submitMat(const math::mat4 *matrices, const GLuint num, const GLint attribLoc, const GLsizeiptr buf);
			void flush() const override;
			void flushInstanced() const;
			//void flushInstanced() const;
			void clean() override;

		};
	}
}