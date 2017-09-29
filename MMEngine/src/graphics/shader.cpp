#include "shader.h"

namespace mme {
	namespace graphics {

		bool Shader::init(const char *vert, const char *frag) {

			// Shader strings load in from files and coverted to c string
			std::string vertShader = loadShader(vert);
			if (vertShader.empty()) {
				gl_log("Error:", " Unable to load in vert shader from file.");
				return false;
			}
			std::string fragShader = loadShader(frag);
			if (fragShader.empty()) {
				gl_log("Error:", " Unable to load in frag shader from file.");
				return false;
			}

			const char *vertex_shader = vertShader.c_str();
			const char *fragment_shader = fragShader.c_str();

			// load shader strings into GL shaders
			m_vertex = glCreateShader(GL_VERTEX_SHADER);	// create fragment shader and gives it a "name" (index) stored in vs. vs is a shader index
			glShaderSource(m_vertex, 1, &vertex_shader, NULL);	// copy code from c string containing shader
			glCompileShader(m_vertex);	// compiles the shader referenced by the name assigned to vs
			if (gl_shader_error(m_vertex)) return false;

			m_fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(m_fragment, 1, &fragment_shader, NULL);
			glCompileShader(m_fragment);
			if (gl_shader_error(m_fragment)) return false;

			// create shader program then attach and link shader objects
			m_program = glCreateProgram(); // create empty shader program. shader_program index (name) to gpu program
			glAttachShader(m_program, m_vertex);	// attach shader to program
			glAttachShader(m_program, m_fragment);
			glLinkProgram(m_program); // link shaders together
										   // After being linked, delete uneeded shaders to free up space 
										   //glDetachShader(shader_program, fs); done when shader program is deleted.

			if (gl_program_error(m_program)) return false;

			return true;
		}

		Shader::Shader(const char *vert, const char *frag) {
			if (!init(vert, frag)) {
				std::cout << "Something went wrong. Check gl.log." << std::endl;
			}
		}

		Shader::~Shader() {
			glDeleteShader(m_vertex);
			glDeleteShader(m_fragment);
			glDeleteProgram(m_program);
		}

		void Shader::enable() const {
			glUseProgram(m_program);
		}

		void Shader::disable() const {
			glUseProgram(0);
		}

	}
}