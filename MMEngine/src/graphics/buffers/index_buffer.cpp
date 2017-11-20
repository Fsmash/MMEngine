#include "index_buffer.h"

namespace mme {
	namespace graphics {

		IndexBuffer::IndexBuffer(GLvoid *vertices, GLsizeiptr sz) {
			glGenBuffers(1, &m_bufferID);	// Generate vertex buffer id (name), just and unsigned int.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);	// Bind buffer "vbo" as current in context
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sz, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		IndexBuffer::~IndexBuffer() {
			glDeleteBuffers(1, &m_bufferID);
		}

		void IndexBuffer::bind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
		}

		void IndexBuffer::unbind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void IndexBuffer::release() {
			glDeleteBuffers(1, &m_bufferID);
		}

	}
}