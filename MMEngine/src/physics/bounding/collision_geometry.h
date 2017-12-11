#pragma once
#include "../../math/vec4.h"

namespace mme {
	namespace physics {

		struct Plane {
			math::vec3 normal;
			float dist;
		};

		struct BoundingSphere {
			float radius;

			BoundingSphere() : radius(0) { } 
		};

		struct AABB2D {
			math::vec2 *min;
			math::vec2 *max;

			AABB2D() : min(nullptr), max(nullptr) { }
		};

		struct AABB3D {
			math::vec3 *min;
			math::vec3 *max;

			AABB3D() : min(nullptr), max(nullptr) { }
		};

	}
}
