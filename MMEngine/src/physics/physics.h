#pragma once
#include "../graphics/objects/shape.h"
#include "../graphics/objects/model.h"
#include <GLFW/glfw3.h>

namespace mme {
	namespace physics {

		class Physics {

		private:

			float m_gravity;
			float m_penalty;
			unsigned int m_numShapes;
			unsigned int m_numModels;
			graphics::Shape *m_shapes;
			graphics::Model *m_models;
			
			bool sweptSphere(const math::vec3 &p0, const math::vec3 &q0, const math::vec3 &p1, const math::vec3 &q1,
							const float rp, const float rq, float &t, math::vec3 &vp, math::vec3 &vq);

		public:

			Physics() : m_gravity(0), m_penalty(1), m_numShapes(0), m_shapes(nullptr), m_models(nullptr) { };

			inline void setGravity(const float grav) { m_gravity = grav; }
			inline float getGravity() const { return m_gravity; }
			inline void setPenalty(const float penalty) { m_penalty = penalty; }
			inline float getPenalty() const { return m_penalty; }

			void submit(graphics::Shape *shapes, const unsigned int num);
			void submit(graphics::Model *models, const unsigned int num);
			void applyForces();
			void AABB3DCollision();
			//void sphereCollision();
			void planeCollision(Plane p);

		};

	}
}