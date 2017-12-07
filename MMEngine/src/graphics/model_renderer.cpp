#include "model_renderer.h"

namespace mme {
	namespace graphics {

		ModelRenderer::ModelRenderer(Model *models, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx) {
			m_bufID[0] = m_bufID[1] = m_bufOffset[2] = 0;
			m_matrixID = 0;
			m_matAttribLoc = 0;
			m_matBufSz = 0;
			m_bufOffset[0] = m_bufOffset[1] = 0;
			m_numModels = m_numInstances = m_numIndices = 0;
			m_init = false;
			m_ptr = nullptr;
			submit(models, num, buf, idx);
		}

		ModelRenderer::ModelRenderer(Model &model) {
			m_bufID[0] = m_bufID[1] = m_bufOffset[2] = 0;
			m_matrixID = 0;
			m_matAttribLoc = 0;
			m_matBufSz = 0;
			m_bufOffset[0] = m_bufOffset[1] = 0;
			m_numModels = m_numInstances = m_numIndices = 0;
			m_init = false;
			m_ptr = nullptr;
			submit(model);
		}

		ModelRenderer::~ModelRenderer() {
			if (m_init)
				clean();
		}

		void ModelRenderer::submit(Model *models, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx) {

			if (m_init) {
				std::cout << "Buffers already initialized" << std::endl;
				return;
			}

			m_ptr = models;
			m_numModels = num;
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

			for (int i = 0; i < m_numModels; i++) {

				bufSz = models[i].vertexBufferSize();

				if (m_bufOffset[0] + bufSz > buf) {
					std::cout << "Memory out of bounds. VBO buffer." << std::endl;
					m_init = true;
					clean();
					return;
				}

				idxSz = models[i].indexBufferSize();

				if (m_bufOffset[1] + idxSz > idx) {
					std::cout << "Memory out of bounds. IBO buffer" << std::endl;
					m_init = true;
					clean();
					return;
				}

				glBufferSubData(GL_ARRAY_BUFFER, m_bufOffset[0], bufSz, models[i].vertices);

				if (i != 0) {
					models[i].offsetIdx(elementOffset);
					std::cout << "offsetting by: " << elementOffset << std::endl;
				}

				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_bufOffset[1], idxSz, models[i].indices);

				m_bufOffset[0] += bufSz;
				m_bufOffset[1] += idxSz;

				elementOffset += models[i].num_vertices;
				m_numIndices += models[i].num_indices;
				std::cout << "i: " << i << " num indices: " << m_numIndices << std::endl;
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			m_init = true;
		}

		void ModelRenderer::submit(Model &model) {

			if (m_init) {
				std::cout << "Buffers already initialized" << std::endl;
				return;
			}

			m_ptr = &model;
			m_numModels = 1;
			m_numIndices = model.num_indices;

			glEnableVertexAttribArray(0); // enables generic vertex attribute array (attribute 0, position)
			glEnableVertexAttribArray(1); // enables generic vertex attribute array (attribute 1, color)
			glEnableVertexAttribArray(2); // enables generic vertex attribute array (attribute 2, normal)

			glGenBuffers(2, m_bufID);
			printf("vbo name: %d\n", m_bufID[0]);
			printf("ibo name: %d\n", m_bufID[1]);

			glBindBuffer(GL_ARRAY_BUFFER, m_bufID[0]);
			glBufferData(GL_ARRAY_BUFFER, model.vertexBufferSize(), model.vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indexBufferSize(), model.indices, GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			m_init = true;
		}

		void ModelRenderer::flush() const {

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
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), 0); // defines layout of buffer, "vbo", for attribute 0 (positions)
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), VertexT::offset1()); // defines layout of buffer.
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), VertexT::offset2()); // defines layout of buffer.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);

			glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);

		}

		void ModelRenderer::flushInstanced() const {

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
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), 0); // defines layout of buffer, "vbo", for attribute 0 (positions)
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), VertexT::offset1()); // defines layout of buffer.
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), VertexT::offset2()); // defines layout of buffer.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);

			glDrawElementsInstanced(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr, m_numInstances);

		}

		void ModelRenderer::clean() {

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