/*	Code by Bryant Dinh for Senior Sem 2017 Project : Misunderstood Magician Engine
	
	This class, Renderer, is the base class for all renderers.
	A renderer "submits" data for rendering and then renders (draws) with "flush".
	All data submitted and flushed are instances Renderables.
*/

#pragma once
#include "objects/renderable.h"
#include "../math/mat4.h"
#include "GL/glew.h"

namespace mme {
	namespace graphics {

		class Renderer {

		protected:

			GLuint m_bufID[2];
			GLuint m_numIndices;
			GLuint m_numInstances;
			GLuint m_matrixID;
			GLint m_matAttribLoc;
			GLsizeiptr m_matBufSz;
			GLsizeiptr m_bufOffset[3];
			bool m_init;

		public:

			Renderer() : m_numIndices(0), m_numInstances(0), m_matrixID(0), m_matAttribLoc(0), m_matBufSz(0), m_init(false) {
				m_bufID[0] = m_bufID[1] = 0;
				m_bufOffset[0] = m_bufOffset[1] = m_bufOffset[2] = 0;
			}
			
			// All virtual functions must be implemented by derived class
			virtual void submit(Renderable *shapes, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx) = 0;
			virtual void submit(Renderable &shape) = 0;
			virtual void flush() const = 0;
			virtual void flushInstanced() const = 0;
			virtual void clean() = 0;

			void submitMat(const math::mat4 *matrices, const GLuint num, const GLint attribLoc, const GLsizeiptr buf);

		};
	}
}