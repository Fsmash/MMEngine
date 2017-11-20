#pragma once
#include "GL/glew.h"

namespace mme {
	namespace graphics {

		class IndexBuffer {

		private:

			GLuint m_bufferID;
			GLsizeiptr m_bufferSz;

		public:

			IndexBuffer(GLvoid *vertices, GLsizeiptr sz);
			~IndexBuffer();
			void bind();
			void unbind();
			void release();

		};
	}
}