#include "model.h"

namespace mme {
	namespace graphics {

		/*
		bool loadMesh(const char *file_name, GLuint *vao, int *point_count) {
			//load a file with assimp and print scene information
			const aiScene *scene = aiImportFile(file_name, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

			if (!scene) {
				fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
				return false;
			}

			printf("  %i animations\n", scene->mNumAnimations);
			printf("  %i cameras\n", scene->mNumCameras);
			printf("  %i lights\n", scene->mNumLights);
			printf("  %i materials\n", scene->mNumMaterials);
			printf("  %i meshes\n", scene->mNumMeshes);
			printf("  %i textures\n", scene->mNumTextures);

			
			//get first mesh in file only
			const aiMesh *mesh = scene->mMeshes[0];
			printf("    %i vertices in mesh[0]\n", mesh->mNumVertices);

			*point_count = mesh->mNumVertices;

			glGenVertexArrays(1, vao);
			glBindVertexArray(*vao);

			//grab model data from assimp
			GLfloat *points = NULL;		 // array of vertex points
			GLfloat *normals = NULL;	 // array of vertex normals
			GLfloat *texcoords = NULL; // array of texture coordinates
			if (mesh->HasPositions()) {
				points = (GLfloat *)malloc(*point_count * 3 * sizeof(GLfloat));
				for (int i = 0; i < *point_count; i++) {
					const aiVector3D *vp = &(mesh->mVertices[i]);
					points[i * 3] = (GLfloat)vp->x;
					points[i * 3 + 1] = (GLfloat)vp->y;
					points[i * 3 + 2] = (GLfloat)vp->z;
				}
			}
			if (mesh->HasNormals()) {
				normals = (GLfloat *)malloc(*point_count * 3 * sizeof(GLfloat));
				for (int i = 0; i < *point_count; i++) {
					const aiVector3D *vn = &(mesh->mNormals[i]);
					normals[i * 3] = (GLfloat)vn->x;
					normals[i * 3 + 1] = (GLfloat)vn->y;
					normals[i * 3 + 2] = (GLfloat)vn->z;
				}
			}
			if (mesh->HasTextureCoords(0)) {
				texcoords = (GLfloat *)malloc(*point_count * 2 * sizeof(GLfloat));
				for (int i = 0; i < *point_count; i++) {
					const aiVector3D *vt = &(mesh->mTextureCoords[0][i]);
					texcoords[i * 2] = (GLfloat)vt->x;
					texcoords[i * 2 + 1] = (GLfloat)vt->y;
				}
			}

			//copy data into vbos
			if (mesh->HasPositions()) {
				GLuint vbo;
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, 3 * *point_count * sizeof(GLfloat), points,
					GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(0);
				free(points);
			}
			if (mesh->HasNormals()) {
				GLuint vbo;
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, 3 * *point_count * sizeof(GLfloat), normals,
					GL_STATIC_DRAW);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(2);
				free(normals);
			}
			if (mesh->HasTextureCoords(0)) {
				GLuint vbo;
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, 2 * *point_count * sizeof(GLfloat), texcoords,
					GL_STATIC_DRAW);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(1);
				free(texcoords);
			}
			if (mesh->HasTangentsAndBitangents()) {
				// NB: could store/print tangents here
			}

			aiReleaseImport(scene);
			printf("mesh loaded\n");

			return true;
		}*/

		Model::Model(const char* file_name) {
			loadModelFile(file_name);
		}

		Model::~Model() {
			if (clean) {

				if (vertices != nullptr) {
					delete[] vertices;
					vertices = nullptr;
				}

				if (indices != nullptr) {
					delete[] indices;
					indices = nullptr;
				}
			}

			num_vertices = num_indices = 0;
		}

		bool Model::loadModelFile(const char *file_name) {
			//load a file with assimp and print scene information
			const aiScene *scene = aiImportFile(file_name, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

			if (!scene) {
				fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
				return false;
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

			num_vertices = mesh->mNumVertices;

			vertices = new VertexT[num_vertices];

			if (mesh->HasPositions()) {
				for (int i = 0; i < num_vertices; i++) {
					const aiVector3D *vp = &(mesh->mVertices[i]);
					vertices[i].pos.x = (GLfloat)vp->x;
					vertices[i].pos.y = (GLfloat)vp->y;
					vertices[i].pos.z = (GLfloat)vp->z;
				}
			}
			if (mesh->HasNormals()) {
				for (int i = 0; i < num_vertices; i++) {
					const aiVector3D *vn = &(mesh->mNormals[i]);
					vertices[i].normal.x = (GLfloat)vn->x;
					vertices[i].normal.y = (GLfloat)vn->y;
					vertices[i].normal.z = (GLfloat)vn->z;
				}
			}
			if (mesh->HasTextureCoords(0)) {
				for (int i = 0; i < num_vertices; i++) {
					const aiVector3D *vt = &(mesh->mTextureCoords[0][i]);
					vertices[i].uv.x = (GLfloat)vt->x;
					vertices[i].uv.y = (GLfloat)vt->y;
				}
			}
			if (mesh->HasFaces()) {
				unsigned int num_faces = mesh->mNumFaces;
				num_indices = mesh->mNumFaces * 3;
				
				//std::cout << "num_indices = " << num_indices << std::endl;
				std::cout << "num_faces = " << num_faces << std::endl;

				indices = new GLuint[num_indices];
				for (unsigned int i = 0; i < num_faces; i++) {	
					aiFace *face = &mesh->mFaces[i];
					//std::cout << "indices #" << i << ": " << face->mIndices[0] << " " << face->mIndices[1] << " " << face->mIndices[2] << std::endl;
					indices[i * 3] = face->mIndices[0];
					indices[i * 3 + 1] = face->mIndices[1];
					indices[i * 3 + 2] = face->mIndices[2];
				}
				//std::cout << "indices: " << indices[2902] << std::endl;
			}

			aiReleaseImport(scene);
			return true;
		}

		/*
		void Model::bufferModel() {
			
			glEnableVertexAttribArray(0); // enables generic vertex attribute array (attribute 0, position)
			glEnableVertexAttribArray(1); // enables generic vertex attribute array (attribute 1, color)
			glEnableVertexAttribArray(2); // enables generic vertex attribute array (attribute 2, normal)

			vertexBufferSize = num_vertices * sizeof(VertexT);
			indexBufferSize = num_indices * sizeof(GLuint);

			glGenBuffers(2, m_bufID);
			printf("vbo name: %d\n", m_bufID[0]);
			printf("ibo name: %d\n", m_bufID[1]);

			glBindBuffer(GL_ARRAY_BUFFER, m_bufID[0]);
			glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indices, GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				
			m_init = true;
			printf("mesh loaded\n");
		}

		void Model::flush() {
			if (m_init) {

				glBindBuffer(GL_ARRAY_BUFFER, m_bufID[0]);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), 0); // defines layout of buffer, "vbo", for attribute 0 (positions)
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), VertexT::offset1()); // defines layout of buffer.
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), VertexT::offset2()); // defines layout of buffer.
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufID[1]);

				glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
			}
		}

		void Model::clean() {
			if (m_init) {

				glDeleteBuffers(2, m_bufID);

				m_bufID[0] = 0;
				m_init = false;
			}

			delete[] vertices;
		} */

	}
}