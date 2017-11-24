#pragma once
#include "GL/glew.h"

namespace mme {
	namespace graphics {
		
		class Buffer {

		private:

			GLuint m_bufferID;
			GLsizeiptr m_bufferSz;
			GLsizeiptr m_dataSz;
			bool m_bound;

		public:
			
			Buffer() : m_bufferID(0), m_bufferSz(0), m_dataSz(0), m_bound(false) {}
			Buffer(GLsizeiptr sz);
			Buffer(GLvoid *vertices, GLsizeiptr sz);
			~Buffer();
			bool add(GLvoid *vertex, GLsizeiptr sz);
			void bind();
			void unbind();
			void release();

		};
	}
}