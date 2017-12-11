#include "model.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace mme {
	namespace graphics {

		Model::Model(const char* file_name) {
			filePath = file_name;
			vertices = nullptr;
			texFilePath = nullptr;
		}

		bool Model::Interleaved() {
			//load a file with assimp and print scene information
			const aiScene *scene = aiImportFile(filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

			if (!scene) {
				fprintf(stderr, "ERROR: reading mesh %s\n", filePath);
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
					vertices[i].pos = *(math::vec3*)(vp);
					//vertices[i].pos.x = (GLfloat)vp->x;
					//vertices[i].pos.y = (GLfloat)vp->y;
					//vertices[i].pos.z = (GLfloat)vp->z;
				}
			}
			if (mesh->HasNormals()) {
				for (int i = 0; i < num_vertices; i++) {
					const aiVector3D *vn = &(mesh->mNormals[i]);
					vertices[i].normal = *(math::vec3*)(vn);
					//vertices[i].normal.x = (GLfloat)vn->x;
					//vertices[i].normal.y = (GLfloat)vn->y;
					//vertices[i].normal.z = (GLfloat)vn->z;
				}
			}
			if (mesh->HasTextureCoords(0)) {
				for (int i = 0; i < num_vertices; i++) {
					const aiVector3D *vt = &(mesh->mTextureCoords[0][i]);
					vertices[i].uv = *(math::vec2*)(vt);
					//vertices[i].uv.x = (GLfloat)vt->x;
					//vertices[i].uv.y = (GLfloat)vt->y;
				}
			}
			if (mesh->HasFaces()) {
				unsigned int num_faces = mesh->mNumFaces;
				num_indices = mesh->mNumFaces * 3;
				
				std::cout << "num_faces = " << num_faces << std::endl;

				indices = new GLuint[num_indices];
				for (unsigned int i = 0; i < num_faces; i++) {	
					aiFace *face = &mesh->mFaces[i];
					indices[i * 3] = face->mIndices[0];
					indices[i * 3 + 1] = face->mIndices[1];
					indices[i * 3 + 2] = face->mIndices[2];
				}
			}

			interleaved = true;
			aiReleaseImport(scene);
			return true;
		}

		void Model::loadTexture(const char* tex_file) {
			texFilePath = tex_file;
		}

		 math::vec3 Model::getCenter() {
			float maxx = 0.0f;
			float maxy = 0.0f;
			float maxz = 0.0f;
			float minx = 2.0f;
			float miny = 2.0f;
			float minz = 2.0f;

			for (int i = 0; i < num_vertices; i++) {
				if (vertices[i].pos.x > maxx) {
					maxx = vertices[i].pos.x;
				}
				if (vertices[i].pos.y > maxy) {
					maxy = vertices[i].pos.y;
				}
				if (vertices[i].pos.z > maxz) {
					maxz = vertices[i].pos.z;
				}
				if (vertices[i].pos.x < minx) {
					minx = vertices[i].pos.x;
				}
				if (vertices[i].pos.y < miny) {
					miny = vertices[i].pos.y;
				}
				if (vertices[i].pos.z < minz) {
					miny = vertices[i].pos.z;
				}
			}

			return math::vec3(((maxx+minx)/2.0f), ((maxy+miny)/2.0f) + 0.0f, ((maxz+minz)/2.0f) - 5.0f);
		}
	}
}