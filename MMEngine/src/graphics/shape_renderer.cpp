#include "shape_renderer.h"

namespace mme {
	namespace graphics {

		/*
		void ShapeRenderer::init() {

			//glGenVertexArrays(1, &m_vao);
			//glBindVertexArray(m_vao);
			m_vbo.bind();
			glEnableVertexAttribArray(0); // enables generic vertex attribute array (attribute 0, position)
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), 0); // defines layout of buffer, "vbo", for attribute 0 (positions)

			glEnableVertexAttribArray(1); // enables attribute (attribute 1, color)
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset1()); // defines layout of buffer.
																										//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (const void *)12); // defines layout of buffer.

			glEnableVertexAttribArray(2); // enables attribute (attribute 2, normals)
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset2()); // defines layout of buffer.
			m_ibo.bind();
			//glBindVertexArray(0);
			std::cout << "initialized" << std::endl;

		}*/

		void ShapeRenderer::submit(Shape *shapes, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx) {
			
			//glGenVertexArrays(1, &m_vao);
			//glBindVertexArray(0);
			glGenBuffers(1, &m_vbo);
			printf("vbo name: %d\n", m_vbo);
			glGenBuffers(1, &m_ibo);
			printf("ibo name: %d\n", m_ibo);
			m_numShapes = num;
			
			GLuint bufOffset = 0;
			GLuint idxOffset = 0;

			GLuint bufSz = 0;
			GLuint idxSz = 0;

			GLuint elementOffset = 0;

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, buf, 0, GL_STATIC_DRAW);
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx, 0, GL_STATIC_DRAW);

			for (int i = 0; i < m_numShapes; i++) {
				
				bufSz = shapes[i].vertexBufferSize();

				if (bufOffset + bufSz > buf) {
					std::cout << "Memory out of bounds. VBO buffer." << std::endl;
					return;
				}

				idxSz = shapes[i].indexBufferSize();

				if (idxOffset + idxSz > idx) {
					std::cout << "Memory out of bounds. IBO buffer" << std::endl;
					return;
				}

				//glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
				glBufferSubData(GL_ARRAY_BUFFER, bufOffset, bufSz, shapes[i].vertices);
				//glBindBuffer(GL_ARRAY_BUFFER, 0);

				if (i != 0) {
					shapes[i].offsetIdx(elementOffset);
					std::cout << "offsetting by: " << elementOffset << std::endl;
				}

				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, idxOffset, idxSz, shapes[i].indices);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				bufOffset += bufSz;
				idxOffset += idxSz;		
				
				elementOffset += shapes[i].num_vertices;
				m_numIndices += shapes[i].num_indices;
				std::cout << "i: " << i << " offset: " << elementOffset << " num indices: " << m_numIndices << std::endl;
			}
		
		}

		void ShapeRenderer::flush() const {
			
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glEnableVertexAttribArray(0); // enables generic vertex attribute array (attribute 0, position)
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), 0); // defines layout of buffer, "vbo", for attribute 0 (positions)

			glEnableVertexAttribArray(1); // enables attribute (attribute 1, color)
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset1()); // defines layout of buffer.
																										//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (const void *)12); // defines layout of buffer.

			glEnableVertexAttribArray(2); // enables attribute (attribute 2, normals)
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset2()); // defines layout of buffer.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

			glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);
		}

		void ShapeRenderer::clean() {
			glDeleteBuffers(1, &m_vbo);
			glDeleteBuffers(1, &m_ibo);
		}

	}
}