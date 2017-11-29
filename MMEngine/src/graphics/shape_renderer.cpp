#include "shape_renderer.h"

namespace mme {
	namespace graphics {

		ShapeRenderer::ShapeRenderer(Shape *shapes, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx) {
			m_bufID[0] = m_bufID[1] = m_bufOffset[2] = 0;
			m_matrixID = 0;
			m_matAttribLoc = 0;
			m_matBufSz = 0;
			m_bufOffset[0] = m_bufOffset[1] = 0;
			m_numShapes = m_numInstances = m_numIndices = 0;
			m_init = false;
			submit(shapes, num, buf, idx);
		}

		ShapeRenderer::ShapeRenderer(Shape &shape) {
			m_bufID[0] = m_bufID[1] = m_bufOffset[2] = 0;
			m_matrixID = 0;
			m_matAttribLoc = 0;
			m_matBufSz = 0;
			m_bufOffset[0] = m_bufOffset[1] = 0;
			m_numShapes = m_numInstances = m_numIndices = 0;
			m_init = false;
			submit(shape);
		}

		ShapeRenderer::~ShapeRenderer() {
			if (m_init)
				clean();
		}

		void ShapeRenderer::submit(Shape *shapes, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx) {

			if (m_init) {
				std::cout << "Buffers already initialized" << std::endl;
				return;
			}

			m_numShapes = num;
			GLsizeiptr bufSz = 0;
			GLsizeiptr idxSz = 0;
			GLuint elementOffset = 0;

			glEnableVertexAttribArray(0); // enables generic vertex attribute array (attribute 0, position)
			glEnableVertexAttribArray(1); // enables generic vertex attribute array (attribute 1, color)
			glEnableVertexAttribArray(2); // enables generic vertex attribute array (attribute 2, normal)
			glGenBuffers(2, m_bufID);
			printf("vbo name: %d\n", m_bufID[0]);
			printf("ibo name: %d\n", m_bufID[1]);

			glBindBuffer(GL_ARRAY_BUFFER, m_bufID[0]);
			glBufferData(GL_ARRAY_BUFFER, buf, 0, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx, 0, GL_STATIC_DRAW);

			for (int i = 0; i < m_numShapes; i++) {

				bufSz = shapes[i].vertexBufferSize();

				if (m_bufOffset[0] + bufSz > buf) {
					std::cout << "Memory out of bounds. VBO buffer." << std::endl;
					clean();
					return;
				}

				idxSz = shapes[i].indexBufferSize();

				if (m_bufOffset[1] + idxSz > idx) {
					std::cout << "Memory out of bounds. IBO buffer" << std::endl;
					clean();
					return;
				}

				glBufferSubData(GL_ARRAY_BUFFER, m_bufOffset[0], bufSz, shapes[i].vertices);

				if (i != 0) {
					shapes[i].offsetIdx(elementOffset);
					std::cout << "offsetting by: " << elementOffset << std::endl;
				}

				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_bufOffset[1], idxSz, shapes[i].indices);

				m_bufOffset[0] += bufSz;
				m_bufOffset[1] += idxSz;

				elementOffset += shapes[i].num_vertices;
				m_numIndices += shapes[i].num_indices;
				std::cout << "i: " << i << " offset: " << elementOffset << " num indices: " << m_numIndices << std::endl;
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			m_init = true;
		}

		void ShapeRenderer::submit(const Shape &shape) {

			if (m_init) {
				std::cout << "Buffers already initialized" << std::endl;
				return;
			}
			m_numShapes = 1;
			m_numIndices = shape.num_indices;

			glEnableVertexAttribArray(0); // enables generic vertex attribute array (attribute 0, position)
			glEnableVertexAttribArray(1); // enables generic vertex attribute array (attribute 1, color)
			glEnableVertexAttribArray(2); // enables generic vertex attribute array (attribute 2, normal)

			glGenBuffers(2, m_bufID);
			printf("vbo name: %d\n", m_bufID[0]);
			printf("ibo name: %d\n", m_bufID[1]);

			glBindBuffer(GL_ARRAY_BUFFER, m_bufID[0]);
			glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			m_init = true;
		}

		void ShapeRenderer::submitMat(const math::mat4 *matrices, const GLuint num, const GLint attribLoc, const GLsizeiptr buf) {

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

		void ShapeRenderer::flush() const {

			if (m_init) {

				glBindBuffer(GL_ARRAY_BUFFER, m_bufID[0]);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), 0); // defines layout of buffer, "vbo", for attribute 0 (positions)
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset1()); // defines layout of buffer.
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset2()); // defines layout of buffer.
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);

				glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);
			}
		}

		void ShapeRenderer::flushInstanced() const {
			
			if (m_init && m_matrixID != 0) {

				glBindBuffer(GL_ARRAY_BUFFER, m_matrixID);

				for (int i = 0; i < 4; i++) {
					glVertexAttribPointer(m_matAttribLoc + i, 4, GL_FLOAT, GL_FALSE, sizeof(math::mat4), (void *)(sizeof(math::vec4) * i));
				}

				glBindBuffer(GL_ARRAY_BUFFER, m_bufID[0]);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), 0); // defines layout of buffer, "vbo", for attribute 0 (positions)
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset1()); // defines layout of buffer.
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset2()); // defines layout of buffer.
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);

				glDrawElementsInstanced(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr, m_numInstances);
			}
		}

		void ShapeRenderer::clean() {
			
			if (m_init) {

				glDeleteBuffers(2, m_bufID);
				if (m_matrixID == 0)
					glDeleteBuffers(1, &m_matrixID);
				
				m_bufID[0] = m_bufID[1] = m_matrixID = 0;
				m_init = false;
			}
		}

	}
}