#include "buffer.h"
#include <iostream>

namespace mme {
	namespace graphics {

		// Constructor generates buffer id, binds it, and generates the buffer on the GPU with specified size, sz.
		Buffer::Buffer(GLsizeiptr sz) {
			generate(0, 0, sz);
			m_bound = false;
			//printf("vbo name: %d\n", m_bufferID);
		}

		// Constructor generates buffer id, binds it, generates the buffer on the GPU with specified size, sz, and sends data into buffer.
		Buffer::Buffer(GLvoid *vertices, GLsizeiptr buf, GLsizeiptr sz) {
			generate(vertices, buf, sz);
			m_bound = false;
			//printf("vbo name: %d\n", m_bufferID);
		}

		// Destructor that deletes buffer on GPU.
		Buffer::~Buffer() {
			m_bound = false;
			release();
		}

		// Gerates buffer if not already generated.
		void Buffer::generate(GLvoid *vertices, GLsizeiptr buf, GLsizeiptr sz) {
			
			if (m_bufferID != 0) {
				std::cout << "Buffer ID already generated." << std::endl;
				return;
			}

			m_bufferSz = sz;
			m_dataSz = buf;
			glGenBuffers(1, &m_bufferID);	// Generate vertex buffer id (name), just and unsigned int.
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);	// Bind buffer "vbo" as current in context
			glBufferData(GL_ARRAY_BUFFER, m_bufferSz, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// Adds vertex data into buffer at offset m_dataSz if m_dataSz + sz does not exceed buffer size.
		bool Buffer::add(GLvoid *vertex, GLsizeiptr sz) {

			if (m_dataSz + sz > m_bufferSz) {
				std::cout << "Memory out of bounds, buffer" << std::endl;
				return false;
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
			glBufferSubData(GL_ARRAY_BUFFER, m_dataSz, sz, vertex);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
			m_dataSz += sz;
			return true;
		}

		// Bind buffer, make current to OpenGL state.
		void Buffer::bind() {
			m_bound = true;
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
		}

		// Unbind buffer.
		void Buffer::unbind() {
			m_bound = false;
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// Delete buffer on GPU.
		void Buffer::release() {
			m_bound = false;
			glDeleteBuffers(1, &m_bufferID);
		}

	}
}