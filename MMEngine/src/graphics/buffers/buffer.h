#pragma once
#include "GL/glew.h"

namespace mme {
	namespace graphics {
		
		class Buffer {

		private:

			GLuint m_bufferID;
			GLsizeiptr m_bufferSz;

		public:
			
			Buffer(GLvoid *vertices, GLsizeiptr sz);
			~Buffer();
			void bind();
			void unbind();
			void release();

		};
	}
}