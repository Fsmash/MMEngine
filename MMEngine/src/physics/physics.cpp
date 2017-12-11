#include "physics.h"

namespace mme {
	namespace physics {
	
		bool Physics::sweptSphere(const math::vec3 &p0, const math::vec3 &q0, const math::vec3 &p1, const math::vec3 &q1,
			const float rp, const float rq, float &t, math::vec3 &vp, math::vec3 &vq) {

			vp = p1 - p0;
			vq = q1 - q0;

			math::vec3 A = p0 - q0;
			math::vec3 B = vp - vq;

			float a = B.dot(B);
			float b = 2 * A.dot(B);
			float c = A.dot(A) - ((rq + rp) * (rq + rp));

			float disc = b * b - 4 * a * c;

			if (disc > 0) {
				t = (-b - sqrt(disc)) / (2 * a);
				return true;
			}

			return false;
		}


		void Physics::submit(graphics::Shape *shapes, const unsigned int num) {
			
			if (m_shapes == nullptr) {
				m_shapes = shapes;
				m_numShapes = num;
				std::cout << "Initialized shapes" << std::endl;
			}
		}

		void Physics::applyForces() {
			
			math::vec3 vel;

			for (int i = 0; i < m_numShapes; i++) {
				
				vel = m_shapes[i].vel;

				m_shapes[i].pos += vel;
				*(m_shapes[i].col_box3D.max) += vel;
				*(m_shapes[i].col_box3D.min) += vel;

				m_shapes[i].model_matrix.incrementX(vel.x);
				m_shapes[i].model_matrix.incrementY(vel.y);
				m_shapes[i].model_matrix.incrementZ(vel.z);
			}

		}

		void Physics::AABB3DCollision() {
			if (m_shapes == nullptr) return;

			math::vec3 centerVec;
			math::vec3 c1, c2;
			math::vec3 vel;
			bool collide;

			for (int i = 0; i < m_numShapes; i++) {

				//std::cout << "AABB max i: " << i << " " << *(m_shapes[i].col_box3D.max) << std::endl;
				//std::cout << "AABB min i: " << i << " " << *(m_shapes[i].col_box3D.min) << std::endl;

				for (int j = i + 1; j < m_numShapes; j++) {
					
					collide = (m_shapes[i].col_box3D.max->x >= m_shapes[j].col_box3D.min->x) &&
							  (m_shapes[j].col_box3D.max->x >= m_shapes[i].col_box3D.min->x) &&
							  (m_shapes[i].col_box3D.max->y >= m_shapes[j].col_box3D.min->y) &&
							  (m_shapes[j].col_box3D.max->y >= m_shapes[i].col_box3D.min->y) &&
							  (m_shapes[i].col_box3D.max->z <= m_shapes[j].col_box3D.min->z) &&
							  (m_shapes[j].col_box3D.max->z <= m_shapes[i].col_box3D.min->z);
						
					if (collide) {
						
						c1 = m_shapes[i].pos;
						c2 = m_shapes[j].pos;
						
						centerVec = c1 - c2;
						centerVec.normalize();
						vel = m_shapes[i].vel;
						m_shapes[i].vel = vel - centerVec.scale(2 * (vel.dot(centerVec)));
						m_shapes[j].vel.scale(m_penalty);

						centerVec = c2 - c1;
						centerVec.normalize();
						vel = m_shapes[j].vel;
						m_shapes[j].vel = vel - centerVec.scale(2 * (vel.dot(centerVec)));
						m_shapes[j].vel.scale(m_penalty);

						// Crappy pong collision. good enough for now.
						//m_shapes[i].vel.scale(-1 * m_penalty);
						//m_shapes[j].vel.scale(-1 * m_penalty);

					}
				}
			}
		}

		void Physics::planeCollision(Plane p) {
			
			math::vec3 max, vel;

			for (int i = 0; i < m_numShapes; i++) {

				max = *m_shapes[i].col_box3D.max;
				math::vec3 c = m_shapes[i].pos;	// Compute AABB center
				math::vec3 e = max - c;			// Compute positive extents

				// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
				float r = e.x * fabs(p.normal.x) + e.y * fabs(p.normal.y) + e.z * fabs(p.normal.z);
				// Compute distance of box center from plane
				float s = -1 * c.dot(p.normal);

				if (fabs(p.dist - s) <= r) {
					vel = m_shapes[i].vel; 
					m_shapes[i].vel = vel - p.normal.scale(2 * vel.dot(p.normal));
				}
			
			}
		}

	}
}