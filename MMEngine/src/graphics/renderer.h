/*	Code by Bryant Dinh for Senior Sem 2017 Project : Misunderstood Magician Engine
	
	This class, Renderer, is the base class for all renderers.
	A renderer "submits" data for rendering and then renders (draws) with "flush".
	All data submitted and flushed are Renderables.
	Shader initialization and use are also handled in Renderer.
*/

#pragma once
#include "objects/renderable.h"
#include "shader.h"
#include "../math/mat4.h"
#include "GL/glew.h"

namespace mme {
	namespace graphics {

		class Renderer {

		protected:

			GLuint m_bufID[2];
			GLuint m_numVertices;
			GLuint m_numIndices;
			GLuint m_numInstances;
			GLuint m_matrixID;
			GLint m_matAttribLoc;
			GLsizeiptr m_bufSz[2];
			GLsizeiptr m_matBufSz;
			GLsizeiptr m_bufOffset[3];
			Shader *m_shader;
			bool m_init;

		public:

			Renderer() : m_numIndices(0), m_numInstances(0), m_matrixID(0), m_matAttribLoc(0), 
						 m_matBufSz(0), m_shader(nullptr), m_init(false) {
				m_bufID[0] = m_bufID[1] = 0;
				m_bufSz[0] = m_bufSz[1] = 0;
				m_bufOffset[0] = m_bufOffset[1] = m_bufOffset[2] = 0;
			}

			virtual ~Renderer() {
				
				if (m_shader != nullptr)
					delete m_shader;
			
			}
			
			// All virtual functions must be implemented by derived class
			virtual void flush() const = 0;
			virtual void flushInstanced() const = 0;
			virtual void clean() = 0;
			
			inline GLuint getVBO() const { return m_bufID[0]; }
			inline GLuint getIBO() const { return m_bufID[1]; }
			inline GLuint getMBO() const { return m_matrixID;; }

			inline GLuint getNumVertices() const { return m_numVertices; }
			inline GLuint getNumIndices() const { return m_numIndices; }
			inline GLuint getNumInstances() const { return m_numInstances; }
			inline GLint getMatAttribLoc() const { m_matAttribLoc; }

			inline GLsizeiptr getVBOSize() const { return m_bufSz[0]; }
			inline GLsizeiptr getIBOSize() const { return m_bufSz[1]; }
			inline GLsizeiptr getMBOSize() const { return m_matBufSz; }
	
			inline GLsizeiptr getVBODataSize() const { return m_bufOffset[0]; }
			inline GLsizeiptr getIBODataSize() const { return m_bufOffset[1]; }
			inline GLsizeiptr getMBODataSize() const { return m_bufOffset[2]; }
			
			inline GLuint getVertexShaderID() const { m_shader->getVert(); }
			inline GLuint getFragmentShaderID() const { m_shader->getFrag(); }
			inline GLuint getShaderProgramID() const { m_shader->getProgram(); }

			void initShader(const char *vert, const char *frag);
			void enableShader();
			void disableShader();
			bool reloadShader(const char *vert, const char *frag);
			void setUniformMat4(const GLchar * name, const math::mat4 &matrix);
			void setUniform1f(const GLchar * name, const GLfloat value);
			void submitMat(const math::mat4 *matrices, const GLuint num, const GLint attribLoc, const GLsizeiptr buf);

		};
	}
}