#pragma once
#include "GL/glew.h"

namespace mme {
	namespace graphics {

		class IndexBuffer {

		private:

			GLuint m_bufferID;
			GLsizeiptr m_bufferSz;
			GLsizeiptr m_dataSz;
			GLboolean m_bound;

		public:

			IndexBuffer() : m_bufferID(0), m_bufferSz(0), m_dataSz(0), m_bound(false) {}
			IndexBuffer(GLsizeiptr sz);
			IndexBuffer(GLvoid *vertices, GLsizeiptr sz);
			~IndexBuffer();
			bool add(GLvoid *vertex, GLsizeiptr sz);
			void bind();
			void unbind();
			void release();

		};
	}
}