#include "renderer.h"

namespace mme {
	namespace graphics {

		// Initialize shader 
		void Renderer::initShader(const char *vert, const char *frag) {
			m_shader = new Shader(vert, frag);
		}

		void Renderer::enableShader() {
			m_shader->enable();
		}

		void Renderer::disableShader() {
			m_shader->disable();
		}

		bool Renderer::reloadShader(const char *vert, const char *frag) {
			return m_shader->reloadShader(vert, frag);
		}

		void Renderer::setUniformMat4(const GLchar * name, const math::mat4 &matrix) {
			m_shader->setUniformMat4(name, matrix);
		}

		void Renderer::setUniform1f(const GLchar * name, const GLfloat value) {
			m_shader->setUniform1f(name, value);
		}

		// This generates a buffer for matrices used in mat4 attributes in instanced rendering. 
		void Renderer::submitMat(const math::mat4 *matrices, const GLuint num, const GLint attribLoc, const GLsizeiptr buf) {

			if (m_matrixID != 0) {
				std::cout << "Matrix Buffer already initialized" << std::endl;
				return;
			}

			m_matAttribLoc = attribLoc;
			m_matBufSz = buf;
			m_numInstances = num;
			GLsizeiptr bufSz = sizeof(math::mat4);

			for (int i = 0; i < 4; i++) {
				glEnableVertexAttribArray(m_matAttribLoc + i);
				glVertexAttribDivisor(m_matAttribLoc + i, 1);
			}

			glGenBuffers(1, &m_matrixID);
			//printf("matrix buffer object name: %d\n", m_matrixID);
			glBindBuffer(GL_ARRAY_BUFFER, m_matrixID);
			glBufferData(GL_ARRAY_BUFFER, m_matBufSz, 0, GL_DYNAMIC_DRAW);

			for (int i = 0; i < m_numInstances; i++) {

				if (m_bufOffset[2] + bufSz > buf) {
					//std::cout << "Memory out of bounds. Matrix buffer." << std::endl;
					clean();
					return;
				}

				glBufferSubData(GL_ARRAY_BUFFER, m_bufOffset[2], bufSz, matrices[i].matrix);
				//std::cout << "Matrix " << i << " " << matrices[i] << std::endl;
				m_bufOffset[2] += bufSz;
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			m_init = true;
		}

	}
}