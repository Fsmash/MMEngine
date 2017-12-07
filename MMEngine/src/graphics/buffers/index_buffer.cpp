#include "index_buffer.h"
#include <iostream>

namespace mme {
	namespace graphics {

		IndexBuffer::IndexBuffer(GLsizeiptr sz) {
			m_bufferSz = sz;
			m_dataSz = 0;
			glGenBuffers(1, &m_bufferID);	// Generate vertex buffer id (name), just and unsigned int.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);	// Bind buffer "vbo" as current in context
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_bufferSz, 0, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			m_bound = false;
			//printf("ibo name: %d\n", m_bufferID);
		}

		IndexBuffer::IndexBuffer(GLvoid *vertices, GLsizeiptr sz) {
			m_bufferSz = sz;
			m_dataSz = sizeof(vertices);
			glGenBuffers(1, &m_bufferID);	// Generate vertex buffer id (name), just and unsigned int.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);	// Bind buffer "vbo" as current in context
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_bufferSz, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			m_bound = false;
			//printf("ibo name: %d\n", m_bufferID);
		}

		IndexBuffer::~IndexBuffer() {
			glDeleteBuffers(1, &m_bufferID);
		}

		bool IndexBuffer::add(GLvoid *vertex, GLsizeiptr sz) {

			if (m_dataSz + sz > m_bufferSz) {
				std::cout << "memory out of bounds buffer" << std::endl;
				return false;
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_dataSz, sz, vertex);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			m_dataSz += sz;
			return true;
		}

		void IndexBuffer::bind() {
			m_bound = true;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
		}

		void IndexBuffer::unbind() {
			m_bound = false;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void IndexBuffer::release() {
			m_bound = false;
			glDeleteBuffers(1, &m_bufferID);
		}

	}
}