/*	Code by Daniel Medina for Senior Sem 2017 Project : Misunderstood Magician Engine

This model_renderer class inherits from renderable and vertex_t and loads data from a model file to be rendered. This class utilizes Assimp library in order to support a large variety of model file types.
*/

#pragma once
#include "renderer.h"
#include "objects/model.h"

namespace mme {
	namespace graphics {

		class ModelRenderer : public Renderer {

		private:

			GLuint m_numModels;	// number of model objects being rendered
			Model *m_ptr;

		public:

			ModelRenderer() : Renderer(), m_numModels(0), m_ptr(nullptr) {}
			ModelRenderer(Model *models, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx);
			ModelRenderer(Model &model);
			~ModelRenderer();

			void submit(Model *models, const GLuint num, const GLsizeiptr buf, const GLsizeiptr idx);
			void submit(Model &model);
			void flush() const override;
			void flushInstanced() const override;
			void clean() override;

		};
	}
}