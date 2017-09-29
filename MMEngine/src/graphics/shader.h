#pragma 
#include <GL/glew.h>
#include <map>
#include "../utility/fileutil.h"
#include "../utility/log.h"

namespace mme {
	namespace graphics {
		
		// "Shader" object that will hold all shader indexes(names) for a corresponding shader program, stored as a index(name) 
		class Shader {

		private:

			// shader indexes (names) and program  index
			GLuint m_vertex;
			GLuint m_fragment;
			//GLuint m_geometry;
			GLuint m_program;
			bool init(const char *vert, const char *frag);

		public:
			
			Shader(const char *vert, const char *frag);
			~Shader();
			void enable() const;
			void disable() const;
			
			/* TO DO
			void setUniform1f(const GLchar* name, float value);
			void setUniform1fv(const GLchar* name, float* value, int count);
			void setUniform1i(const GLchar* name, int value);
			void setUniform1iv(const GLchar* name, int* value, int count);
			void setUniform2f(const GLchar* name, const maths::vec2& vector);
			void setUniform3f(const GLchar* name, const maths::vec3& vector);
			void setUniform4f(const GLchar* name, const maths::vec4& vector);
			void setUniformMat4(const GLchar* name, const maths::mat4& matrix);
			*/

			inline GLuint getVert() const { return m_vertex; }
			inline GLuint getFrag() const { return m_fragment; }
			//inline GLuint getGeom() const { return m_geometry; }
			inline GLuint getProgram() const { return m_program; }
			inline void setVert(GLenum idx) { m_vertex = idx; }
			inline void setFrag(GLenum idx) { m_fragment = idx; }
			//inline void setGeom(GLenum idx) { m_geometry = idx; }
			inline void setProgram(GLenum idx) { m_program = idx; }

		};
	}}