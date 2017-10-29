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

		bool Shader::reloadShader(const char * vert, const char * frag) {
			// Delete previous program to start shader compilation over
			glDeleteShader(m_vertex);
			glDeleteShader(m_fragment);
			glDeleteProgram(m_program);

			if (!init(vert, frag)) {
				std::cout << "Something went wrong. Check gl.log." << std::endl;
				return false;
			}

			return true;
		}

		void Shader::setUniform1f(const GLchar* name, GLfloat value) {
			GLint unifLoc = glGetUniformLocation(m_program, name); // get location of uniform variable in shader
			if (unifLoc < 0) {
				gl_error_log("Error: could not find uniform location for %s", name);
				return;
			}
			glUniform1f(unifLoc, value);
		}

		void Shader::setUniform1fv(const GLchar* name, GLfloat* value, GLsizei count) {
			GLint unifLoc = glGetUniformLocation(m_program, name); // get location of uniform variable in shader
			if (unifLoc < 0) {
				gl_error_log("Error: could not find uniform location for %s", name);
				return;
			}
			glUniform1fv(unifLoc, count, value);
		}

		void Shader::setUniform1i(const GLchar* name, GLint value) {
			GLint unifLoc = glGetUniformLocation(m_program, name); // get location of uniform variable in shader
			if (unifLoc < 0) {
				gl_error_log("Error: could not find uniform location for %s", name);
				return;
			}
			glUniform1i(unifLoc, value);
		}

		void Shader::setUniform1iv(const GLchar* name, GLint* value, GLsizei count) {
			GLint unifLoc = glGetUniformLocation(m_program, name); // get location of uniform variable in shader
			if (unifLoc < 0) {
				gl_error_log("Error: could not find uniform location for %s", name);
				return;
			}
			glUniform1iv(unifLoc, count, value);
		}

		void Shader::setUniform2f(const GLchar* name, const math::vec2& vector) {
			GLint unifLoc = glGetUniformLocation(m_program, name); // get location of uniform variable in shader
			if (unifLoc < 0) {
				gl_error_log("Error: could not find uniform location for %s", name);
				return;
			}
			glUniform2f(unifLoc, vector.x, vector.y);
		}

		void Shader::setUniform3f(const GLchar* name, const math::vec3& vector) {
			GLint unifLoc = glGetUniformLocation(m_program, name); // get location of uniform variable in shader
			if (unifLoc < 0) {
				gl_error_log("Error: could not find uniform location for %s", name);
				return;
			}
			glUniform3f(unifLoc, vector.x, vector.y, vector.z);
		}

		void Shader::setUniform4f(const GLchar* name, const math::vec4& vector) {
			GLint unifLoc = glGetUniformLocation(m_program, name); // get location of uniform variable in shader
			if (unifLoc < 0) {
				gl_error_log("Error: could not find uniform location for %s", name);
				return;
			}
			glUniform4f(unifLoc, vector.x, vector.y, vector.z, vector.w);
		}

		void Shader::setUniformMat4(const GLchar* name, const math::mat4& mat) {
			GLint unifLoc = glGetUniformLocation(m_program, name); // get location of uniform variable in shader
			if (unifLoc < 0) {
				gl_error_log("Error: could not find uniform location for %s", name);
				return;
			}
			glUniformMatrix4fv(unifLoc, 1, GL_FALSE, mat.matrix);
		}

	}
}