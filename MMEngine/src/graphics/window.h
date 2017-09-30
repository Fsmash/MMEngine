#pragma once
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <iostream>
#include "../utility/log.h"

namespace mme {
	namespace graphics {

		class Window {

		private:
			const char *m_title;
			int m_width, m_height;		// window size
			int m_fwidth, m_fheight;	// frame buffer size of window
			GLFWwindow *m_window;
			bool m_closed;

			bool init();
			friend void frameBufResize(GLFWwindow *window, int width, int height);
			friend void update_fps_counter(GLFWwindow * window);
			friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		public:
			Window(const char* title, int width, int height);
			~Window();
			void clear() const;
			void update();
			bool closed();
			void frameCounter() const;

			inline bool isClosed() const { return m_closed; }
			inline int getSize() const { return m_width; }
			inline int getHeight() const { return m_height; }
			inline int getFWidth() const { return m_fwidth; }
			inline int getFHeight() const { return m_fheight; }

		};
	}
}