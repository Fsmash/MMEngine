#include "window.h"

namespace mme { namespace graphics { 

	bool Window::init() {

		if (!glfwInit()) {
			fprintf(stderr, "Error: could no initialize GLFW3\n");
			return false;
		}

		glfwWindowHint(GLFW_SAMPLES, 16); // anti aliasing hint. sample 4 times
		m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);

		if (!m_window) {
			fprintf(stderr, "Error: could not open window with GLFW3\n");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_window);	// makes created window current. this HAS to be done before glewinit
		glewExperimental = GL_TRUE;		// enables support for newer versions of openGL

		if (glewInit() != GLEW_OK) {	// initialize glew
			fprintf(stderr, "Error: could not initialize glew\n");
			glfwTerminate();
			return false;
		}

		// call back functions here.
		glfwSetErrorCallback(glfw_error_log);	// register error call-back function for glfw error function
		glfwSetFramebufferSizeCallback(m_window, frameBufResize);	// call back function called when framebuffer is resized

		return true;
	}

	void frameBufResize(GLFWwindow *window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	// function that calculates fps and updates counter in glfw window
	void update_fps_counter(GLFWwindow * window) {
		static double prev_sec = 0;
		static int frame_count = 0;
		double cur_sec;
		double elapsed_sec;
		cur_sec = glfwGetTime();
		elapsed_sec = cur_sec - prev_sec;
		// limits text update to 4 per second
		if (elapsed_sec > 0.25) {
			prev_sec = cur_sec;
			char tmp[128];
			double fps = (double)frame_count / elapsed_sec;
			sprintf(tmp, "opengl @ fps: %.2f. millisec per frame: %.2f", fps, (1 / fps) * 1000.0);
			glfwSetWindowTitle(window, tmp);
			frame_count = 0;
		}
		frame_count++;
	}

	Window::Window(const char *title, int width, int height) {
		m_title = title;
		m_width = width;
		m_height = height;
		if (!init()) {
			glfwTerminate();
			m_closed = true;
		}
		else
			m_closed = false;
	}

	Window::~Window() {
		glfwTerminate();
	}

	void Window::clear() const {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear drawing surface
	}

	void Window::update() {
		//glViewport(0, 0, m_fwidth, m_fheight);
		glfwGetWindowSize(m_window, &m_width, &m_height);
		glfwGetFramebufferSize(m_window, &m_fwidth, &m_fheight);
		glfwPollEvents();
		glfwSwapBuffers(m_window); // flip the swap onto the screen, and screen onto nex drawing surface 
	}

	bool Window::closed() {
		m_closed = glfwWindowShouldClose(m_window) == 1;
		return m_closed;
	}

	void Window::frameCounter() const {
		update_fps_counter(m_window);
	}

} }