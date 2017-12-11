#include "model_renderer.h"

namespace mme {
	namespace graphics {

		ModelRenderer::ModelRenderer(Model *models, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx) {
			m_bufID[0] = m_bufID[1] = m_bufOffset[2] = 0;
			m_bufSz[0] = m_bufSz[1] = 0;
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
			m_bufSz[0] = m_bufSz[1] = 0;
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

			m_numModels = num;
			m_bufSz[0] = buf;
			m_bufSz[1] = idx;
			GLsizeiptr bufSz = 0;
			GLsizeiptr idxSz = 0;
			GLuint elementOffset = 0;

			indices = new int[num];

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
				indices[i] = models[i].num_indices;
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			m_ptr = models;
			fitTexture();
			m_init = true;
		}

		void ModelRenderer::submit(Model &model) {
			m_ptr = &model;
			m_numModels = 1;

			if (m_init) {
				std::cout << "Buffers already initialized" << std::endl;
				return;
			}
			
			if (model.interleaved == false) {
				const aiScene *scene = aiImportFile(model.filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

				if (!scene) {
					fprintf(stderr, "ERROR: reading mesh %s\n", model.filePath);
				}

				printf("  %i animations\n", scene->mNumAnimations);
				printf("  %i cameras\n", scene->mNumCameras);
				printf("  %i lights\n", scene->mNumLights);
				printf("  %i materials\n", scene->mNumMaterials);
				printf("  %i meshes\n", scene->mNumMeshes);
				printf("  %i textures\n", scene->mNumTextures);

				//get first mesh in file only
				const aiMesh *mesh = scene->mMeshes[0];
				printf("%i vertices in mesh[0]\n", mesh->mNumVertices);
				vertice_count = mesh->mNumVertices;

				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);

				GLfloat *points = NULL;
				GLfloat *normals = NULL;
				GLfloat *texcoords = NULL;
				GLuint *indices = NULL;
				if (mesh->HasPositions()) {
					points = new GLfloat[mesh->mNumVertices * 3];
					for (int i = 0; i < mesh->mNumVertices; i++) {
						const aiVector3D *vp = &(mesh->mVertices[i]);
						points[i * 3] = vp->x;
						points[i * 3 + 1] = vp->y;
						points[i * 3 + 2] = vp->z;
					}
				}
				if (mesh->HasNormals()) {
					normals = new GLfloat[mesh->mNumVertices * 3];
					for (int i = 0; i < mesh->mNumVertices; i++) {
						const aiVector3D *vn = &(mesh->mNormals[i]);
						normals[i * 3] = vn->x;
						normals[i * 3 + 1] = vn->y;
						normals[i * 3 + 2] = vn->z;
					}
				}
				if (mesh->HasTextureCoords(0)) {
					texcoords = new GLfloat[mesh->mNumVertices * 2];
					for (int i = 0; i < mesh->mNumVertices; i++) {
						const aiVector3D *vt = &(mesh->mTextureCoords[0][i]);
						texcoords[i * 2] = vt->x;
						texcoords[i * 2 + 1] = vt->y;
					}
				}
				if (mesh->HasFaces()) {
					m_numIndices = mesh->mNumFaces * 3;
					indices = new GLuint[m_numIndices];

					std::cout << "num_faces = " << mesh->mNumFaces << std::endl;

					for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
						aiFace *face = &mesh->mFaces[i];
						indices[i * 3] = face->mIndices[0];
						indices[i * 3 + 1] = face->mIndices[1];
						indices[i * 3 + 2] = face->mIndices[2];
					}
				}

				if (mesh->HasPositions()) {
					GLuint pVbo;
					glGenBuffers(1, &pVbo);
					glBindBuffer(GL_ARRAY_BUFFER, pVbo);
					glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), points, GL_STATIC_DRAW);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
					glEnableVertexAttribArray(0); // enables generic vertex attribute array (attribute 0, position)
					delete[] points;
				}
				if (mesh->HasNormals()) {
					GLuint nVbo;
					glGenBuffers(1, &nVbo);
					glBindBuffer(GL_ARRAY_BUFFER, nVbo);
					glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);
					glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
					glEnableVertexAttribArray(2); // enables generic vertex attribute array (attribute 0, position)
					delete[] normals;
				}
				if (mesh->HasTextureCoords(0)) {
					GLuint tVbo;
					glGenBuffers(1, &tVbo);
					glBindBuffer(GL_ARRAY_BUFFER, tVbo);
					glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), texcoords, GL_STATIC_DRAW);
					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
					glEnableVertexAttribArray(1); // enables generic vertex attribute array (attribute 0, position)
					delete[] texcoords;
				}
				if (mesh->HasFaces()) {
					GLuint iVbo;
					glGenBuffers(1, &iVbo);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iVbo);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);
					delete[] indices;
				}
				if (mesh->HasTangentsAndBitangents()) {
					//possibly store or print tangents
				}

				aiReleaseImport(scene);
			}
			else {
				m_numIndices = model.num_indices;
				m_bufSz[0] = model.vertexBufferSize();
				m_bufSz[1] = model.indexBufferSize();

				glEnableVertexAttribArray(0); // enables generic vertex attribute array (attribute 0, position)
				glEnableVertexAttribArray(1); // enables generic vertex attribute array (attribute 1, color)
				glEnableVertexAttribArray(2); // enables generic vertex attribute array (attribute 2, normal)

				glGenBuffers(2, m_bufID);
				printf("vbo name: %d\n", m_bufID[0]);
				printf("ibo name: %d\n", m_bufID[1]);

				glBindBuffer(GL_ARRAY_BUFFER, m_bufID[0]);
				glBufferData(GL_ARRAY_BUFFER, m_bufSz[0], model.vertices, GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_bufSz[1], model.indices, GL_STATIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}

			fitTexture();
			m_init = true;
		}

		void ModelRenderer::add(Model *models, const GLuint num) {
			m_ptr = models;

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

				bufSz = models[i].vertexBufferSize();

				if (m_bufOffset[0] + bufSz > m_bufSz[0]) {
					std::cout << "Memory out of bounds. VBO buffer." << std::endl;
					m_init = true;
					clean();
					return;
				}

				idxSz = models[i].indexBufferSize();

				if (m_bufOffset[1] + idxSz > m_bufSz[1]) {
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
				m_numModels++;

				std::cout << "i: " << i << " offset: " << models[i].num_indices << " num indices: " << m_numIndices << std::endl;
			}

			m_numVertices = elementOffset;

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
			if (m_ptr[0].interleaved == false) {
				glBindVertexArray(vao);
				glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);
			} else {
				glBindBuffer(GL_ARRAY_BUFFER, m_bufID[0]);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), 0); // defines layout of buffer, "vbo", for attribute 0 (positions)
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), VertexT::offset1()); // defines layout of buffer.
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), VertexT::offset2()); // defines layout of buffer.
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);

				glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);
			}
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
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), VertexT::offset1()); // defines layout of buffer.
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), VertexT::offset2()); // defines layout of buffer.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);

			glDrawElementsInstanced(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr, m_numInstances);

		}

		void ModelRenderer::flushDynamic(const char *uniform) const {

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
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), VertexT::offset1()); // defines layout of buffer.
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), VertexT::offset2()); // defines layout of buffer.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);

			GLuint offset = 0;
			GLuint num_indices = 0;
			//num_indices = m_numIndices;
			//glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (GLvoid *)offset);
			//std::cout << "num indices: " << num_indices << " offset: " << offset << std::endl;

			for (int i = 0; i < m_numModels; i++) {
				m_shader->setUniformMat4(uniform, m_ptr[i].model_matrix);
				num_indices = indices[i];
				//std::cout << m_ptr[i].model_matrix << std::endl;
				glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (GLvoid *)(sizeof(GLuint) * offset));
				//std::cout << "num indices: " << num_indices << " offset: " << offset << std::endl;
				offset += num_indices;
			}

		}

		void ModelRenderer::fitTexture() {
			int x, y, n;
			int force_channels = 4;

			unsigned char *img_data = stbi_load(m_ptr->texFilePath, &x, &y, &n, force_channels);

			if (!img_data) {
				fprintf(stderr, "ERROR: could not load image data %s\n", m_ptr->texFilePath);
			}
			else {
				fprintf(stdout, "Image width %d\nImage height %d\n# of 8 bit components per pixel %d\n", x, y, n);
			}
			if (x & (x - 1) != 0 || y & (y - 1) != 0) {
				fprintf(stderr, "Image %s not a power of two. Could potentially be not supported by older graphics cards.", m_ptr->texFilePath);
			}
			
			// image loaded in upside down most of the time. images difine 0 of y axis at the to left corner.
			int width_in_bytes = x * 4;
			unsigned char *top = NULL;
			unsigned char *bottom = NULL;
			unsigned char temp = 0;
			int half_height = y / 2;

			for (int row = 0; row < half_height; row++) {
				top = img_data + row * width_in_bytes;
				bottom = img_data + (y - row - 1) * width_in_bytes;
				for (int col = 0; col < width_in_bytes; col++) {
					temp = *top;
					*top = *bottom;
					*bottom = temp;
					top++;
					bottom++;
				}
			}

			// Texture Buffer
			GLuint tex = 0;
			glGenTextures(1, &tex);	// generate texture id (name) used to reference texture
			glActiveTexture(GL_TEXTURE0);	// set active texture slot to be texture 0, by default 0 anyways
			glBindTexture(GL_TEXTURE_2D, tex);	// bind
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			
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
				delete[] indices;
			}
		}

	}
}