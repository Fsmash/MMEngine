#include "shader.h"

namespace mme {
	namespace graphics {

		// PRIVATE MEMBER FUNCTION
		// Gnerates and compiles shaders. Then attaches and links them to generated shader program.
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
			std::cout << "Vertex shader ID: " << m_vertex << std::endl;
			glShaderSource(m_vertex, 1, &vertex_shader, NULL);	// copy code from c string containing shader
			glCompileShader(m_vertex);	// compiles the shader referenced by the name assigned to vs
			if (gl_shader_error(m_vertex)) return false;

			m_fragment = glCreateShader(GL_FRAGMENT_SHADER);
			std::cout << "Fragment shader ID: " << m_fragment << std::endl;
			glShaderSource(m_fragment, 1, &fragment_shader, NULL);
			glCompileShader(m_fragment);
			if (gl_shader_error(m_fragment)) return false;

			// create shader program then attach and link shader objects
			m_program = glCreateProgram(); // create empty shader program. shader_program index (name) to gpu program
			std::cout << "Program shader ID: " << m_program << std::endl;
			glAttachShader(m_program, m_vertex);	// attach shader to program
			glAttachShader(m_program, m_fragment);
			glLinkProgram(m_program); // link shaders together
		    // After being linked, delete uneeded shaders to free up space
			deleteVertexShader();
			deleteFragmentShader();
		    //glDetachShader(shader_program, fs); done when shader program is deleted.
			m_vertex = m_fragment = -1;
			if (gl_program_error(m_program)) return false;

			return true;
		}
		// END OF PRIVATE MEMBER FUNCTIONS

		// Constructor, initializes shader programs
		Shader::Shader(const char *vert, const char *frag) {
			m_program = m_vertex = m_fragment = -1;
			if (!init(vert, frag)) {
				std::cout << "Something went wrong. Check gl.log." << std::endl;
			}
		}

		// Destructor, deletes shader program
		Shader::~Shader() {
			glDeleteProgram(m_program);
			m_program = m_vertex = m_fragment = -1;
		}

		// "Use" shader program, sets as current in OpenGL context
		void Shader::enable() const {
			glUseProgram(m_program);
		}

		// Unbinds shader program
		void Shader::disable() const {
			glUseProgram(0);
		}

		// Reinitializes shader program
		bool Shader::reloadShader(const char * vert, const char * frag) {
			// Delete previous program to start shader compilation over
			deleteShaderProgram();

			if (!init(vert, frag)) {
				std::cout << "Something went wrong. Check gl.log." << std::endl;
				return false;
			}

			return true;
		}

		void Shader::deleteVertexShader() {
			if (m_vertex != -1) {
				glDeleteShader(m_vertex);
				m_vertex = -1;
			}
		}

		void Shader::deleteFragmentShader() {
			if (m_fragment != -1) {
				glDeleteShader(m_fragment);
				m_fragment = -1;
			}
		}

		// Delete shaders
		void Shader::deleteShaderProgram() {
			if (m_program != -1) {
				glDeleteProgram(m_program);
				m_program = m_vertex = m_fragment = -1;
			}
		}

		// Set uniform functions
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