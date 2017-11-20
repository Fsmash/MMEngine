#include "buffer.h"

namespace mme {
	namespace graphics {

		Buffer::Buffer(GLvoid *vertices, GLsizeiptr sz) {
			glGenBuffers(1, &m_bufferID);	// Generate vertex buffer id (name), just and unsigned int.
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);	// Bind buffer "vbo" as current in context
			glBufferData(GL_ARRAY_BUFFER, sz, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		Buffer::~Buffer() {
			glDeleteBuffers(1, &m_bufferID);
		}

		void Buffer::bind() {
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
		}

		void Buffer::unbind() {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void Buffer::release() {
			glDeleteBuffers(1, &m_bufferID);
		}

	}
}