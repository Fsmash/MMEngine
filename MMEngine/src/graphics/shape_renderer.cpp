#include "shape_renderer.h"

namespace mme {
	namespace graphics {

		ShapeRenderer::ShapeRenderer(Shape *shapes, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx) {
			m_bufID[0] = m_bufID[1] = 0;
			m_bufSz[0] = m_bufSz[1] = 0;
			m_matrixID = 0;
			m_matAttribLoc = 0;
			m_matBufSz = 0;
			m_bufOffset[0] = m_bufOffset[1] = m_bufOffset[2] = 0;
			m_numShapes = m_numInstances = m_numIndices = 0;
			m_init = false;
			m_ptr = nullptr;
			submit(shapes, num, buf, idx);
		}

		ShapeRenderer::ShapeRenderer(Shape &shape) {
			m_bufID[0] = m_bufID[1] = 0;
			m_bufSz[0] = m_bufSz[1] = 0;
			m_matrixID = 0;
			m_matAttribLoc = 0;
			m_matBufSz = 0;
			m_bufOffset[0] = m_bufOffset[1] = m_bufOffset[2] = 0;
			m_numShapes = m_numInstances = m_numIndices = 0;
			m_init = false;
			m_ptr = nullptr;
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

			m_ptr = shapes;
			m_numShapes = num;
			m_bufSz[0] = buf;
			m_bufSz[1] = idx;
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
					m_init = true;
					clean();
					return;
				}

				idxSz = shapes[i].indexBufferSize();

				if (m_bufOffset[1] + idxSz > idx) {
					std::cout << "Memory out of bounds. IBO buffer" << std::endl;
					m_init = true;
					clean();
					return;
				}

				glBufferSubData(GL_ARRAY_BUFFER, m_bufOffset[0], bufSz, shapes[i].vertices);

				if (i != 0) {
					shapes[i].offsetIdx(elementOffset);
					//std::cout << "offsetting by: " << elementOffset << std::endl;
				}

				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_bufOffset[1], idxSz, shapes[i].indices);

				m_bufOffset[0] += bufSz;
				m_bufOffset[1] += idxSz;

				elementOffset += shapes[i].num_vertices;
				m_numIndices += shapes[i].num_indices;
				//std::cout << "i: " << i << " offset: " << shapes[i].num_indices << " num indices: " << m_numIndices << std::endl;
			}

			m_numVertices = elementOffset;

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			m_init = true;
		}

		void ShapeRenderer::submit(Shape &shape) {

			if (m_init) {
				std::cout << "Buffers already initialized" << std::endl;
				return;
			}

			m_ptr = &shape;
			m_numShapes = 1;
			m_numIndices = shape.num_indices;
			m_bufSz[0] = shape.vertexBufferSize();
			m_bufSz[1] = shape.indexBufferSize();

			glEnableVertexAttribArray(0); // enables generic vertex attribute array (attribute 0, position)
			glEnableVertexAttribArray(1); // enables generic vertex attribute array (attribute 1, color)
			glEnableVertexAttribArray(2); // enables generic vertex attribute array (attribute 2, normal)

			glGenBuffers(2, m_bufID);
			printf("vbo name: %d\n", m_bufID[0]);
			printf("ibo name: %d\n", m_bufID[1]);

			glBindBuffer(GL_ARRAY_BUFFER, m_bufID[0]);
			glBufferData(GL_ARRAY_BUFFER, m_bufSz[0], shape.vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_bufSz[1], shape.indices, GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			m_init = true;
		}

		void ShapeRenderer::add(Shape *shapes, const GLuint num) {

			if (!m_init) {
				std::cout << "Buffers not initialized" << std::endl;
				return;
			}

			GLsizeiptr bufSz = 0;
			GLsizeiptr idxSz = 0;
			GLuint elementOffset = 0;
			
			glBindBuffer(GL_ARRAY_BUFFER, m_bufID[0]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);

			for (int i = 0; i < num; i++) {

				bufSz = shapes[i].vertexBufferSize();

				if (m_bufOffset[0] + bufSz > m_bufSz[0]) {
					std::cout << "Memory out of bounds. VBO buffer." << std::endl;
					m_init = true;
					clean();
					return;
				}

				idxSz = shapes[i].indexBufferSize();

				if (m_bufOffset[1] + idxSz > m_bufSz[1]) {
					std::cout << "Memory out of bounds. IBO buffer" << std::endl;
					m_init = true;
					clean();
					return;
				}

				glBufferSubData(GL_ARRAY_BUFFER, m_bufOffset[0], bufSz, shapes[i].vertices);

				if (i != 0) {
					shapes[i].offsetIdx(elementOffset);
					//std::cout << "offsetting by: " << elementOffset << std::endl;
				}

				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_bufOffset[1], idxSz, shapes[i].indices);

				m_bufOffset[0] += bufSz;
				m_bufOffset[1] += idxSz;
				
				elementOffset += shapes[i].num_vertices;
				m_numIndices += shapes[i].num_indices;
				m_numShapes++;

				//std::cout << "i: " << i << " offset: " << shapes[i].num_indices << " num indices: " << m_numIndices << std::endl;
			}

			m_numVertices = elementOffset;

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		}

		void ShapeRenderer::flush() const {
			
			if (m_shader == nullptr) {
				std::cout << "Shader has not been initialized" << std::endl;
				return;
			}

			if (!m_init) {
				std::cout << "Buffers not initialized" << std::endl;
				return;
			}

			GLint cur_program;
			glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

			if (cur_program != m_shader->getProgram()) {
				m_shader->enable();
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_bufID[0]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), 0); // defines layout of buffer, "vbo", for attribute 0 (positions)
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset1()); // defines layout of buffer.
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset2()); // defines layout of buffer.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);

			glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);
			
		}

		void ShapeRenderer::flushInstanced() const {

			if (m_shader == nullptr) {
				std::cout << "Shader has not been initialized" << std::endl;
				return;
			}

			if (!m_init || m_matrixID == 0) {
				std::cout << "Buffers not initialized" << std::endl;
				return;
			}

			GLint cur_program;
			glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

			if (cur_program != m_shader->getProgram()) {
				m_shader->enable();
			}
				
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

		void ShapeRenderer::flushDynamic(const char *uniform) const {
			
			if (m_shader == nullptr) {
				std::cout << "Shader has not been initialized" << std::endl;
				return;
			}

			if (!m_init) {
				std::cout << "Buffers not initialized" << std::endl;
				return;
			}

			GLint cur_program;
			glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

			if (cur_program != m_shader->getProgram()) {
				m_shader->enable();
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_bufID[0]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), 0); // defines layout of buffer, "vbo", for attribute 0 (positions)
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset1()); // defines layout of buffer.
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset2()); // defines layout of buffer.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);

			GLuint offset = 0;
			GLuint num_indices = 0;
			//num_indices = m_numIndices;
			//glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (GLvoid *)offset);
			//std::cout << "num indices: " << num_indices << " offset: " << offset << std::endl;
			
			for (int i = 0; i < m_numShapes; i++) {
				m_shader->setUniformMat4(uniform, m_ptr[i].model_matrix);
				num_indices = m_ptr[i].num_indices;
				//std::cout << m_ptr[i].model_matrix << std::endl;
				glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (GLvoid *)(sizeof(GLuint) * offset));
				//std::cout << "num indices: " << num_indices << " offset: " << offset << std::endl;
				offset += num_indices; 
			}

		}

		void ShapeRenderer::clean() {
			
			if (m_init) {

				glDeleteBuffers(2, m_bufID);
				
				if (m_matrixID != 0)
					glDeleteBuffers(1, &m_matrixID);
				
				if (m_shader != nullptr)
					delete m_shader;

				m_bufID[0] = m_bufID[1] = m_matrixID = 0;
				m_shader = nullptr;
				m_init = false;
			}
		}

	}
}