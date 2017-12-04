/*	Code by Daniel Meidna for Senior Sem 2017 Project : Misunderstood Magician Engine

This struct, Shape, inerits from Renderable. This "renderable" will hold an array of VertexC vertices that will interleave attribute data for
basic 2D and 3D shapes. It also has useful member functions to perform operations on the vertices such as updating positions with translation,
scaling, or rotation operations.

This class Model inherits from
*/

#pragma once
#include "renderable.h"
#include "vertex_t.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace mme {
	namespace graphics {

		class Model: public Renderable {
		private:
			VertexT *vertices;
			//const aiScene *scene;
			//const aiMesh *mesh;
			GLuint m_bufID[3];
			GLsizeiptr m_bufOffset[3];
			bool m_init;
			GLsizeiptr vertexBufferSize;
			GLsizeiptr indexBufferSize;

		public:

			Model(const char* file_name);
			~Model();

			bool loadModelFile(const char* file_name);
			void bufferModel();
			void flush();
			void clean();
		};

		bool loadMesh(const char *file_name, GLuint *vao, int *point_count);

	}
}