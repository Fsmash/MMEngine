#pragma once
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <iostream>
#include "../utility/log.h"
#define MAX_KEYS 1024
#define MAX_BUTTONS 32

namespace mme {
	namespace graphics {

		class Window {

		private:
			const char *m_title;		// title of window
			int m_width, m_height;		// window size
			int m_fwidth, m_fheight;	// frame buffer size of window
			GLFWwindow *m_window;
			bool m_closed;
			bool m_keys[MAX_KEYS];
			bool m_mouse[MAX_BUTTONS];
			double m_xpos, m_ypos;		// mouse positions

			bool init();
			friend static void frameBufResize(GLFWwindow *window, int width, int height);
			friend static void fpsCounter(GLFWwindow * window);
			friend static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
			friend static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
			//friend static void cursorCallback(GLFWwindow *window, double x, double y);

		public:
			Window(const char* title, int width, int height);
			~Window();
			void clear() const;
			void update();
			bool resized();
			bool closed();
			void frameCounter() const;
			bool isKeyPressed(unsigned int key) const;
			bool isMousePressed(unsigned int key) const;

			inline bool isClosed() const { return m_closed; }
			inline int getWidth() const { return m_width; }
			inline int getHeight() const { return m_height; }
			inline int getFWidth() const { return m_fwidth; }
			inline int getFHeight() const { return m_fheight; }
			inline double getX() const { return m_xpos; }
			inline double getY() const { return m_ypos; }

		};
	}
}