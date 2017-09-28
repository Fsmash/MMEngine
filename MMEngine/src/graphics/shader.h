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

			// holds shader variables (attribues and uniform) with corresponding index
			typedef std::map<std::string, int> VariableMap;
			VariableMap m_map;

			// shader indexes (names) and program  index
			GLuint m_vertex;
			GLuint m_fragment;
			//GLuint m_geometry;
			GLuint m_program;

			// get index of variables in shader program. 
			//GLuint getVariableIndex(const std::string name, bool isUnif);
			bool init(const char *vert, const char *frag);

		public:

			Shader(const char *vert, const char *frag);
			~Shader();
			void enable() const;
			void disable() const;
			inline GLuint getVert() const { return m_vertex; }
			inline GLuint getFrag() const { return m_fragment; }
			//inline GLuint getGeom() const { return m_geometry; }
			inline GLuint getProgram() const { return m_program; }
			inline void setVert(GLenum idx) { m_vertex = idx; }
			inline void setFrag(GLenum idx) { m_fragment = idx; }
			//inline void setGeom(GLenum idx) { m_geometry = idx; }
			inline void setProgram(GLenum idx) { m_program = idx; }

			//GLuint getAttributeIndex(const std::string name);
			//GLuint getUniformIndex(const std::string name);

		};
	}}