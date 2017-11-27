#pragma once
#include "objects/renderable.h"
//#include "buffers/buffer.h"
//#include "buffers/index_buffer.h"
//#include <vector>

namespace mme {
	namespace graphics {

		class Renderer {

		protected:

			GLuint m_bufID[2];
			GLuint m_numIndices;
			bool m_init;
			//GLuint m_vbo;
			//GLuint m_ibo;
			//std::vector<Renderable> mem;

		public:

			Renderer() : m_numIndices(0), m_init(false) {
				m_bufID[0] = m_bufID[1] = 0;
			}
			//virtual void submit(Renderable *buf) = 0;
			virtual void flush() const = 0;
			virtual void clean() = 0;

		};
	}
}