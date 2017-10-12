#include "window.h"

namespace mme {
	namespace graphics {

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

			// set call back functions here.
			glfwSetWindowUserPointer(m_window, this);
			glfwSetErrorCallback(glfw_error_log);	// register error call-back function for glfw error function
			glfwSetFramebufferSizeCallback(m_window, frameBufResize);	// call back function called when framebuffer is resized
			glfwSetKeyCallback(m_window, keyCallback);	// call back functions for key presses
			glfwSetMouseButtonCallback(m_window, mouseButtonCallback);	// call back functions for key presses
			glfwSetCursorPosCallback(m_window, cursorCallback);

			return true;
		}

		// GLFW CALL BACK FUNCTIONS (FRIENDS)
		void frameBufResize(GLFWwindow *window, int width, int height) {
			glViewport(0, 0, width, height);
		}

		// function that calculates fps and updates counter in glfw window
		void fpsCounter(GLFWwindow * window) {
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

		void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window *win = (Window *)glfwGetWindowUserPointer(window);
			win->m_keys[key] = action != GLFW_RELEASE;

			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, 1);
			}

			// other inputs can go here
		}

		void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
			Window *win = (Window *)glfwGetWindowUserPointer(window);
			win->m_mouse[button] = action != GLFW_RELEASE;
			//std::cout << button << std::endl;
			std::cout << win->m_xpos << std::endl;
			std::cout << win->m_ypos << std::endl;
		}

		void cursorCallback(GLFWwindow *window, double x, double y) {
			Window *win = (Window *)glfwGetWindowUserPointer(window);
			win->m_xpos = x;
			win->m_ypos = y;
			//std::cout << win->m_xpos << std::endl;
			//std::cout << win->m_ypos << std::endl;
		}
		// END OF CALL BACK FUNCTIONS

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

			for (int i = 0; i < MAX_KEYS; i++) {
				m_keys[i] = false;
			}
		}

		Window::~Window() {
			glfwTerminate();
		}

		void Window::clear() const {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear drawing surface
		}

		void Window::update() {
			//glViewport(0, 0, m_fwidth, m_fheight);
			glfwGetFramebufferSize(m_window, &m_fwidth, &m_fheight);
			glfwPollEvents();
			glfwSwapBuffers(m_window); // flip the swap onto the screen, and screen onto nex drawing surface
			//std::cout << m_width << " " << m_height << std::endl;
		}

		bool Window::resized() {
			int old_w = m_width;
			int old_h = m_height;
			glfwGetWindowSize(m_window, &m_width, &m_height);
			return (old_w != m_width) || (old_h != m_height);
		}

		bool Window::closed() {
			m_closed = glfwWindowShouldClose(m_window) == 1;
			return m_closed;
		}

		void Window::frameCounter() const {
			fpsCounter(m_window);
		}

		bool Window::isKeyPressed(unsigned int key) const {
			
			if (key >= MAX_KEYS || key < 0) {
				gl_log("Error: Key pressed out of range. key = %d\n", key);
				return false;
			}

			return m_keys[key];
		}

	}
}