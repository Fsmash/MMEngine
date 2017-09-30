#pragma once
#include <time.h>
#include <stdarg.h>	// let functions take a variable number of arguments
#define GL_LOG_FILE "gl.log"

namespace mme {

	// opens log file and prints date and time at top. 
	// print version number of code here
	bool restart_gl_log();
	// main logging function
	bool gl_log(const char *msg, ...);

	// sames as log(), just also prints to stderr stream
	bool gl_error_log(const char *msg, ...);

	// error log for glfw
	void glfw_error_log(int error, const char *desc);

	void log_gl_params();

	void print_shader_info_log(GLuint shader_index);

	void print_program_info_log(GLuint program_index);

	const char* GL_type_to_string(GLenum type);

	void print_all(GLuint programme);

	// checks whether a program is valid or not
	bool is_valid(GLuint programme);

	// error checking for shader objects after compilation
	bool gl_shader_error(GLuint shader_idx);

	// error checking for program object after linking shaders
	bool gl_program_error(GLuint program_idx);
}