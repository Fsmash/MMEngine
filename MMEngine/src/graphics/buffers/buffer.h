/*	Code by Bryant Dinh for Senior Sem 2017 Project : Misunderstood Magician Engine

	This class, Buffer, holds all the data to involved in creating a vertex buffer object (vbo) in OpenGL
	It provides an abstraction for the process of generating a vbo name (id), binding the vbo, and filling it with data (CPU -> GPU).
	It also has methods for unbinding vbos and freeing up memory by deleting vbos when they are not longer needed.
	Though the name of the buffer object implies it is just for "vertex" data it is essentially just a buffer and can be used to store
	anything data.
*/

#pragma once
#include "GL/glew.h"

namespace mme {
	namespace graphics {
		
		class Buffer {

		private:

			GLuint m_bufferID;		// holds buffer name (id)
			GLsizeiptr m_bufferSz;	// will store size of buffer
			GLsizeiptr m_dataSz;	// hold the size of the data stored in buffer
			bool m_bound;			// true if vbo is currently bound, false otherwise

		public:
			
			Buffer() : m_bufferID(0), m_bufferSz(0), m_dataSz(0), m_bound(false) {}
			Buffer(GLsizeiptr sz);
			Buffer(GLvoid *vertices, GLsizeiptr buf, GLsizeiptr sz);
			~Buffer();

			void generate(GLvoid *vertices, GLsizeiptr buf, GLsizeiptr sz);
			bool add(GLvoid *vertex, GLsizeiptr sz);
			void bind();
			void unbind();
			void release();

			inline GLsizeiptr getBufSize() const { return m_bufferSz; }
			inline GLsizeiptr getDataSize() const { return m_dataSz; }
			inline bool isBound() const { return m_bound; }
		};
	}
}